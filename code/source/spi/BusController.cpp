#include "BusController.hpp"

// C++
#include <array>
#include <vector>
#include <thread>
#include <sstream>

// C
extern "C" {
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
}

namespace pbl::spi
{

namespace
{

[[nodiscard]] constexpr std::uint8_t toSpiMode( BusController::Mode mode ) noexcept
{
	switch( mode )
	{
		using enum BusController::Mode;
		case MODE_0: return SPI_MODE_0;
		case MODE_1: return SPI_MODE_1;
		case MODE_2: return SPI_MODE_2;
		case MODE_3: return SPI_MODE_3;
		default: return SPI_MODE_0;
	}
	return SPI_MODE_0;
}

} // namespace

auto v1::BusController::open( const std::string& device, Mode mode, Speed speed, BitsPerWord bits )
	-> Result< BusController >
{
	BusController bus{ device };
	bus.m_fd = ::open( device.c_str(), O_RDWR | O_CLOEXEC );
	if( bus.m_fd < 0 ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::DEVICE_NOT_FOUND, getError() );
	}

	if( auto rslt = bus.configure( mode, speed, bits ); !rslt ) [[unlikely]]
	{
		return utils::MakeError( rslt.error() );
	}

	bus.m_open = true;
	return Result< BusController >{ std::move( bus ) };
}

v1::BusController::BusController( const std::string& busName )
	: m_busName{ busName }
{ }

v1::BusController::~BusController()
{
	if( m_open )
	{
		::close( m_fd );
	}
	m_open = false;
}

void v1::BusController::sleep( const std::chrono::milliseconds sleepTimeMs )
{
	std::this_thread::sleep_for( sleepTimeMs );
}

void v1::BusController::sleep( const std::chrono::microseconds sleepTimeUs )
{
	std::this_thread::sleep_for( sleepTimeUs );
}

auto v1::BusController::configure( Mode mode, Speed speed, BitsPerWord bitsPerWord ) -> Result< void >
{
	if( !m_open.load() )
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, "SPI Device is not open" );
	}

	std::uint8_t modeValue = toSpiMode( mode );
	if( ::ioctl( m_fd, SPI_IOC_WR_MODE, &modeValue ) == -1 )
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, "Failed to set SPI mode" );
	}

	std::uint8_t bitsValue = static_cast< uint8_t >( bitsPerWord );
	if( ::ioctl( m_fd, SPI_IOC_WR_BITS_PER_WORD, &bitsValue ) == -1 )
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, "Failed to set bits per word" );
	}

	std::uint32_t speedValue = static_cast< uint32_t >( speed );
	if( ::ioctl( m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speedValue ) == -1 )
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, "Failed to set SPI speed" );
	}

	return utils::MakeSuccess();
}

std::string v1::BusController::getError()
{
	int e = errno;
	std::array< char, 256 > buffer{};

	// strerror_r is thread-safe and POSIX-compliant
	::strerror_r( e, buffer.data(), buffer.size() );
	return std::string{ buffer.data() };
}

} // namespace pbl::spi
