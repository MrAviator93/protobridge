// PBL
#include <i2c/BusController.hpp>

// Third Party
#include <gtest/gtest.h>

// C++
#include <cerrno>
#include <cstdarg>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

// Linux
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

namespace
{

constexpr std::string_view kBusName{ "/test/protobridge/i2c" };

struct Syscalls
{
	int descriptor{ 41 };
	int openError{};
	int queryError{};
	unsigned long functions{ I2C_FUNC_I2C };
	int openFlags{};
	int queryCount{};
	int transferCount{};
	std::vector< int > closedDescriptors;
};

Syscalls* activeSyscalls{};

} // namespace

// Linker wrapping keeps the production API independent of the test implementation.
extern "C" int __real_open( const char*, int, ... );
extern "C" int __real_close( int );
extern "C" int __real_ioctl( int, unsigned long, ... );

extern "C" int __wrap_open( const char* path, int flags, ... )
{
	if( activeSyscalls && path == kBusName )
	{
		activeSyscalls->openFlags = flags;
		if( activeSyscalls->openError )
		{
			errno = activeSyscalls->openError;
			return -1;
		}
		return activeSyscalls->descriptor;
	}

	if( flags & O_CREAT || ( flags & O_TMPFILE ) == O_TMPFILE )
	{
		va_list args;
		va_start( args, flags );
		const auto mode = va_arg( args, mode_t );
		va_end( args );
		return __real_open( path, flags, mode );
	}
	return __real_open( path, flags );
}

extern "C" int __wrap_close( int descriptor )
{
	if( activeSyscalls && ( descriptor == activeSyscalls->descriptor || descriptor == -1 ) )
	{
		activeSyscalls->closedDescriptors.push_back( descriptor );
		// Cleanup must not overwrite the error returned by the factory.
		errno = EBADF;
		return 0;
	}
	return __real_close( descriptor );
}

extern "C" int __wrap_ioctl( int descriptor, unsigned long request, ... )
{
	va_list args;
	va_start( args, request );
	void* argument = va_arg( args, void* );
	va_end( args );

	if( !activeSyscalls || descriptor != activeSyscalls->descriptor )
	{
		return __real_ioctl( descriptor, request, argument );
	}
	if( request == I2C_FUNCS )
	{
		++activeSyscalls->queryCount;
		if( activeSyscalls->queryError )
		{
			errno = activeSyscalls->queryError;
			return -1;
		}
		*static_cast< unsigned long* >( argument ) = activeSyscalls->functions;
		return 0;
	}
	if( request == I2C_RDWR )
	{
		++activeSyscalls->transferCount;
		return static_cast< int >( static_cast< i2c_rdwr_ioctl_data* >( argument )->nmsgs );
	}
	ADD_FAILURE() << "Unexpected ioctl request";
	errno = EINVAL;
	return -1;
}

namespace pbl::i2c
{
namespace
{

static_assert( !std::is_constructible_v< BusController, const std::string& > );
static_assert( !std::is_copy_constructible_v< BusController > );
static_assert( std::is_nothrow_move_constructible_v< BusController > );
static_assert( !std::is_move_assignable_v< BusController > );

class BusControllerTest : public testing::Test
{
protected:
	void SetUp() override { activeSyscalls = &syscalls; }
	void TearDown() override { activeSyscalls = nullptr; }

	Syscalls syscalls;
};

struct OpenFailure
{
	int error;
	utils::ErrorCode expected;
	const char* name;
};

class BusOpenFailureTest : public BusControllerTest, public testing::WithParamInterface< OpenFailure >
{ };

TEST_P( BusOpenFailureTest, MapsOpeningFailureAndPreservesContext )
{
	// Arrange
	const auto failure = GetParam();
	syscalls.openError = failure.error;

	// Act
	const auto result = BusController::open( std::string{ kBusName } );

	// Assert
	ASSERT_FALSE( result );
	EXPECT_EQ( static_cast< utils::ErrorCode >( result.error() ), failure.expected );
	ASSERT_TRUE( result.error().message() );
	EXPECT_NE( result.error().message()->find( kBusName ), std::string::npos );
	EXPECT_EQ( syscalls.queryCount, 0 );
	EXPECT_TRUE( syscalls.closedDescriptors.empty() );
}

INSTANTIATE_TEST_SUITE_P( OperatingSystemErrors,
						  BusOpenFailureTest,
						  testing::Values( OpenFailure{ ENOENT, utils::ErrorCode::DEVICE_NOT_FOUND, "ENOENT" },
										   OpenFailure{ EACCES, utils::ErrorCode::ACCESS_DENIED, "EACCES" },
										   OpenFailure{ EPERM, utils::ErrorCode::ACCESS_DENIED, "EPERM" },
										   OpenFailure{ EBUSY, utils::ErrorCode::BUS_BUSY, "EBUSY" },
										   OpenFailure{ ENODEV, utils::ErrorCode::HARDWARE_NOT_AVAILABLE, "ENODEV" },
										   OpenFailure{ EINVAL, utils::ErrorCode::INVALID_ARGUMENT, "EINVAL" },
										   OpenFailure{ ENXIO, utils::ErrorCode::DEVICE_NOT_RESPONDING, "ENXIO" },
										   OpenFailure{ EIO, utils::ErrorCode::HARDWARE_FAILURE, "EIO" },
										   OpenFailure{ EMFILE, utils::ErrorCode::UNEXPECTED_ERROR, "EMFILE" } ),
						  []( const testing::TestParamInfo< OpenFailure >& info ) {
							  return std::string{ info.param.name };
						  } );

TEST_F( BusControllerTest, OpensBusAndClosesDescriptorOnce )
{
	// Arrange
	bool opened{};
	std::string name;

	// Act
	{
		const auto result = BusController::open( std::string{ kBusName } );
		if( result )
		{
			opened = result->isOpen();
			name = result->bus();
		}
	}

	// Assert
	EXPECT_TRUE( opened );
	EXPECT_EQ( name, kBusName );
	EXPECT_EQ( syscalls.queryCount, 1 );
	EXPECT_EQ( syscalls.openFlags & O_ACCMODE, O_RDWR );
	EXPECT_NE( syscalls.openFlags & O_CLOEXEC, 0 );
	EXPECT_EQ( syscalls.closedDescriptors, ( std::vector< int >{ syscalls.descriptor } ) );
}

TEST_F( BusControllerTest, ClosesDescriptorAfterCapabilityQueryFails )
{
	// Arrange
	syscalls.queryError = EIO;

	// Act
	const auto result = BusController::open( std::string{ kBusName } );

	// Assert
	ASSERT_FALSE( result );
	EXPECT_EQ( static_cast< utils::ErrorCode >( result.error() ), utils::ErrorCode::HARDWARE_FAILURE );
	ASSERT_TRUE( result.error().message() );
	EXPECT_NE( result.error().message()->find( kBusName ), std::string::npos );
	EXPECT_EQ( syscalls.closedDescriptors, ( std::vector< int >{ syscalls.descriptor } ) );
}

TEST_F( BusControllerTest, RejectsAdapterWithoutRawI2CSupport )
{
	// Arrange
	syscalls.functions = I2C_FUNC_SMBUS_READ_BYTE;

	// Act
	const auto result = BusController::open( std::string{ kBusName } );

	// Assert
	ASSERT_FALSE( result );
	EXPECT_EQ( static_cast< utils::ErrorCode >( result.error() ), utils::ErrorCode::UNSUPPORTED_OPERATION );
	EXPECT_EQ( syscalls.closedDescriptors, ( std::vector< int >{ syscalls.descriptor } ) );
}

TEST_F( BusControllerTest, MoveTransfersOwnershipAndLeavesSourceClosed )
{
	// Arrange
	std::optional< BusController > destination;
	bool sourceClosed{};
	bool destinationOpened{};
	bool transferSucceeded{};
	std::string name;
	const std::array< std::uint8_t, 1 > data{ 0x01 };

	// Act
	{
		auto result = BusController::open( std::string{ kBusName } );
		if( result )
		{
			destination.emplace( std::move( *result ) );
			sourceClosed = !result->isOpen();
			destinationOpened = destination->isOpen();
			name = destination->bus();
			transferSucceeded = destination->write( 0x77, data ).has_value();
		}
	}
	const auto closesBeforeDestinationDestruction = syscalls.closedDescriptors.size();
	destination.reset();

	// Assert
	EXPECT_TRUE( sourceClosed );
	EXPECT_TRUE( destinationOpened );
	EXPECT_TRUE( transferSucceeded );
	EXPECT_EQ( name, kBusName );
	EXPECT_EQ( syscalls.transferCount, 1 );
	EXPECT_EQ( closesBeforeDestinationDestruction, 0 );
	EXPECT_EQ( syscalls.closedDescriptors, ( std::vector< int >{ syscalls.descriptor } ) );
}

TEST_F( BusControllerTest, MovedFromBusRejectsTransfersWithoutCallingIoctl )
{
	// Arrange
	auto result = BusController::open( std::string{ kBusName } );
	ASSERT_TRUE( result );
	BusController destination{ std::move( *result ) };
	const std::array< std::uint8_t, 1 > data{ 0x01 };

	// Act
	const auto transfer = result->write( 0x77, data );

	// Assert
	ASSERT_FALSE( transfer );
	EXPECT_EQ( static_cast< utils::ErrorCode >( transfer.error() ), utils::ErrorCode::DEVICE_NOT_FOUND );
	EXPECT_EQ( syscalls.transferCount, 0 );
}

TEST_F( BusControllerTest, OwnsDescriptorZero )
{
	// Arrange
	syscalls.descriptor = 0;
	bool opened{};

	// Act
	{
		const auto result = BusController::open( std::string{ kBusName } );
		opened = result && result->isOpen();
	}

	// Assert
	EXPECT_TRUE( opened );
	EXPECT_EQ( syscalls.closedDescriptors, ( std::vector< int >{ 0 } ) );
}

} // namespace
} // namespace pbl::i2c
