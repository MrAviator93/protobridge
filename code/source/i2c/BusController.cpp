#include "BusController.hpp"

// C++
#include <array>
#include <thread>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

// TODO: Debug remove
#include <iostream>

// C
extern "C" {
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
}

namespace pbl::i2c
{

namespace
{

[[nodiscard]] bool check( std::uint64_t fs, const std::pair< std::uint64_t, std::string_view >& element )
{
	if( fs & element.first )
	{
		std::ostringstream oss;
		oss << element.second << " SUPPORTED" << std::endl;
		return true;
	}

	return false;
}

constexpr std::array< std::pair< std::size_t, std::string_view >, 20 > kFuncsToCheck = {
	std::pair{ I2C_FUNC_I2C, "I2C_FUNC_I2C" },
	std::pair{ I2C_FUNC_10BIT_ADDR, "I2C_FUNC_10BIT_ADDR" },
	std::pair{ I2C_FUNC_PROTOCOL_MANGLING, "I2C_FUNC_PROTOCOL_MANGLING" },
	std::pair{ I2C_FUNC_SMBUS_PEC, "I2C_FUNC_SMBUS_PEC" },
	std::pair{ I2C_FUNC_NOSTART, "I2C_FUNC_NOSTART" },
	std::pair{ I2C_FUNC_SLAVE, "I2C_FUNC_SLAVE" },
	std::pair{ I2C_FUNC_SMBUS_BLOCK_PROC_CALL, "I2C_FUNC_SMBUS_BLOCK_PROC_CALL" },
	std::pair{ I2C_FUNC_SMBUS_QUICK, "I2C_FUNC_SMBUS_QUICK" },
	std::pair{ I2C_FUNC_SMBUS_READ_BYTE, "I2C_FUNC_SMBUS_READ_BYTE" },
	std::pair{ I2C_FUNC_SMBUS_WRITE_BYTE, "I2C_FUNC_SMBUS_WRITE_BYTE" },
	std::pair{ I2C_FUNC_SMBUS_READ_BYTE_DATA, "I2C_FUNC_SMBUS_READ_BYTE_DATA" },
	std::pair{ I2C_FUNC_SMBUS_WRITE_BYTE_DATA, "I2C_FUNC_SMBUS_WRITE_BYTE_DATA" },
	std::pair{ I2C_FUNC_SMBUS_READ_WORD_DATA, "I2C_FUNC_SMBUS_READ_WORD_DATA" },
	std::pair{ I2C_FUNC_SMBUS_WRITE_WORD_DATA, "I2C_FUNC_SMBUS_WRITE_WORD_DATA" },
	std::pair{ I2C_FUNC_SMBUS_PROC_CALL, "I2C_FUNC_SMBUS_PROC_CALL" },
	std::pair{ I2C_FUNC_SMBUS_READ_BLOCK_DATA, "I2C_FUNC_SMBUS_READ_BLOCK_DATA" },
	std::pair{ I2C_FUNC_SMBUS_WRITE_BLOCK_DATA, "I2C_FUNC_SMBUS_WRITE_BLOCK_DATA" },
	std::pair{ I2C_FUNC_SMBUS_READ_I2C_BLOCK, "I2C_FUNC_SMBUS_READ_I2C_BLOCK" },
	std::pair{ I2C_FUNC_SMBUS_WRITE_I2C_BLOCK, "I2C_FUNC_SMBUS_WRITE_I2C_BLOCK" },
	std::pair{ I2C_FUNC_SMBUS_HOST_NOTIFY, "I2C_FUNC_SMBUS_HOST_NOTIFY" } };

template < std::size_t N >
[[nodiscard]] bool
readN( const int fd, const std::uint8_t deviceAddr, const std::uint8_t reg, std::array< std::uint8_t, N >& result )
{
	std::uint8_t outbuf[ 2 ];
	outbuf[ 0 ] = reg;

	i2c_msg msgs[ 2 ];
	msgs[ 0 ].addr = deviceAddr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = outbuf;

	msgs[ 1 ].addr = deviceAddr;
	msgs[ 1 ].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[ 1 ].len = N;
	msgs[ 1 ].buf = result.data();

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].msgs = msgs;
	msgset[ 0 ].nmsgs = 2;

	::memset( result.data(), 0x00, N );

	if( ::ioctl( fd, I2C_RDWR, &msgset ) < 0 )
	{
		return false;
	}

	return true;
}

} // namespace

v1::BusController::BusController( const std::string& busName )
	: m_busName{ busName }
{
	m_fd = ::open( m_busName.c_str(), O_RDWR | O_NONBLOCK );
	if( m_fd < 0 )
	{
		reportError();
		return;
	}

	checkFunc();

	m_open = true;
}

v1::BusController::~BusController()
{
	::close( m_fd );
	m_open = false;
}

bool v1::BusController::read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::uint8_t& result )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return false;
	}

	std::lock_guard _{ m_fdMtx };

	std::uint8_t outbuf[ 1 ];
	std::uint8_t inbuf[ 1 ];

	i2c_msg msgs[ 2 ];
	msgs[ 0 ].addr = deviceAddr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = &outbuf[ 0 ];

	msgs[ 1 ].addr = deviceAddr;
	msgs[ 1 ].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[ 1 ].len = 1;
	msgs[ 1 ].buf = inbuf;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].msgs = msgs;
	msgset[ 0 ].nmsgs = 2;

	outbuf[ 0 ] = reg;

	inbuf[ 0 ] = 0;

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	result = inbuf[ 0 ];

	return true;
}

bool v1::BusController::read( const std::uint8_t deviceAddr,
							  const std::uint8_t reg,
							  std::array< std::uint8_t, 2 >& result )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return false;
	}

	std::lock_guard _{ m_fdMtx };

	if( !readN( m_fd, deviceAddr, reg, result ) )
	{
		reportError();
		return false;
	}

	return true;
}

bool v1::BusController::read( const std::uint8_t deviceAddr,
							  const std::uint8_t reg,
							  std::array< std::uint8_t, 4 >& result )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return false;
	}

	std::lock_guard _{ m_fdMtx };

	if( !readN( m_fd, deviceAddr, reg, result ) )
	{
		reportError();
		return false;
	}

	return true;
}

bool v1::BusController::read( const std::uint8_t deviceAddr,
							  const std::uint8_t reg,
							  std::int16_t& value,
							  const std::endian endian )
{
	std::array< std::uint8_t, 2 > raw{};
	if( !read( deviceAddr, reg, raw ) )
	{
		return false;
	}

	if( endian == std::endian::native )
	{
		// Native endianness: directly interpret the raw bytes
		value = ( raw[ 0 ] << 8 ) | raw[ 1 ];
	}
	else if( endian == std::endian::little )
	{
		value = ( raw[ 1 ] << 8 ) | raw[ 0 ];
	}
	else if( endian == std::endian::big )
	{
		value = ( raw[ 0 ] << 8 ) | raw[ 1 ];
	}
	else
	{
		return false;
	}

	return true;
}

bool v1::BusController::read( const std::uint8_t deviceAddr,
							  const std::uint8_t reg,
							  std::int32_t& value,
							  const std::endian endian )
{
	std::array< std::uint8_t, 4 > raw{};
	if( !read( deviceAddr, reg, raw ) )
	{
		return false;
	}

	if( endian == std::endian::native )
	{
		// Native endianness: directly interpret the raw bytes
		value = ( raw[ 0 ] << 24 ) | ( raw[ 1 ] << 16 ) | ( raw[ 2 ] << 8 ) | raw[ 3 ];
	}
	else if( endian == std::endian::little )
	{
		value = ( raw[ 3 ] << 24 ) | ( raw[ 2 ] << 16 ) | ( raw[ 1 ] << 8 ) | raw[ 0 ];
	}
	else if( endian == std::endian::big )
	{
		value = ( raw[ 0 ] << 24 ) | ( raw[ 1 ] << 16 ) | ( raw[ 2 ] << 8 ) | raw[ 3 ];
	}
	else
	{
		return false;
	}

	return true;
}

std::int16_t v1::BusController::read( const std::uint8_t deviceAddr,
									  const std::uint8_t reg,
									  std::uint8_t* pData,
									  std::uint16_t dataSize )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return {};
	}

	std::lock_guard _{ m_fdMtx };

	std::uint8_t outbuf[ 2 ];
	outbuf[ 0 ] = reg;

	i2c_msg msgs[ 2 ];
	msgs[ 0 ].addr = deviceAddr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = outbuf;

	msgs[ 1 ].addr = deviceAddr;
	msgs[ 1 ].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[ 1 ].len = dataSize;
	msgs[ 1 ].buf = pData;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].msgs = msgs;
	msgset[ 0 ].nmsgs = 2;

	::memset( pData, 0x00, dataSize );

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return -1;
	}

	return dataSize;
}

bool v1::BusController::write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::uint8_t data )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return {};
	}

	std::lock_guard _{ m_fdMtx };

	std::uint8_t outbuf[ 2 ];
	outbuf[ 0 ] = reg;
	outbuf[ 1 ] = data;

	i2c_msg msgs[ 1 ];
	msgs[ 0 ].addr = deviceAddr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 2;
	msgs[ 0 ].buf = outbuf;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].nmsgs = 1;
	msgset[ 0 ].msgs = msgs;

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	return true;
}

bool BusController::write( const std::uint8_t deviceAddr,
						   const std::uint8_t reg,
						   const std::span< const std::uint8_t > data )
{
	return write( deviceAddr, reg, data.data(), data.size() );
}

bool v1::BusController::write( const std::uint8_t deviceAddr,
							   const std::uint8_t reg,
							   const std::uint8_t* data,
							   const std::uint8_t size )
{
	if( !isOpen() )
	{
		setLastError( "I2C bus is closed" );
		return false;
	}

	std::lock_guard _{ m_fdMtx };

	// TODO: Consider using std::array and fixed maximum buffer size
	std::vector< std::uint8_t > dataBuffer;
	dataBuffer.resize( size + 1 ); // + 1 for register

	// Fill the data buffer
	dataBuffer[ 0 ] = reg;
	for( int i = 1; i < size + 1; i++ )
	{
		dataBuffer[ i ] = data[ i - 1 ];
	}

	i2c_msg msgs[ 1 ];
	msgs[ 0 ].addr = deviceAddr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = dataBuffer.size();
	msgs[ 0 ].buf = dataBuffer.data();

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].nmsgs = 1;
	msgset[ 0 ].msgs = msgs;

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	return true;
}

void v1::BusController::sleep( const std::chrono::milliseconds sleepTimeMs )
{
	std::this_thread::sleep_for( sleepTimeMs );
}

void v1::BusController::sleep( const std::chrono::microseconds sleepTimeUs )
{
	std::this_thread::sleep_for( sleepTimeUs );
}

void v1::BusController::reportError()
{
	auto e = errno;
	std::array< char, 256 > err{};

	std::lock_guard _{ m_lastErrMtx };
	m_lastError = ::strerror_r( e, err.data(), err.size() );
}

void v1::BusController::checkFunc()
{
	std::uint64_t funcs{};

	if( ::ioctl( m_fd, I2C_FUNCS, &funcs ) < 0 )
	{
		return reportError();
	}

	std::cerr << "Supported funcions are: " << std::hex << funcs << std::endl;

	const auto checkLambda = [ &funcs ]( const auto& element ) { return check( funcs, element ); };
	const bool rslt = std::ranges::all_of( kFuncsToCheck, checkLambda );

	if( !rslt )
	{
		std::cerr << "Some functions are not supported" << std::endl;
	}
}

static_assert( std::is_same_v< __u8, std::uint8_t >, "__u8 definition differs from std::uint8_t definition." );
static_assert( std::is_same_v< unsigned int, std::uint32_t >,
			   "unsigned int definition differs from std::uint32_t definition." );

} // namespace pbl::i2c
