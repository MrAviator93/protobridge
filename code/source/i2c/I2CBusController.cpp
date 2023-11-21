#include "I2CBusController.hpp"

// C++
#include <array>
#include <thread>
#include <vector>
#include <sstream>
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

namespace PBL::I2C
{
namespace
{
bool check( std::uint64_t fs, std::uint64_t f, char const* what )
{
	if( fs & f )
	{
		std::ostringstream oss;
		oss << what << " SUPPORTED" << std::endl;
		return true;
	}

	return false;
}
} // namespace

I2CBusController::I2CBusController( const std::string& busName )
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

I2CBusController::~I2CBusController()
{
	::close( m_fd );
	m_open = false;
}

bool I2CBusController::read( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t& result )
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
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = &outbuf[ 0 ];

	msgs[ 1 ].addr = slave_addr;
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

std::int16_t
I2CBusController::read( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t* pData, std::uint16_t dataSize )
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
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = outbuf;

	msgs[ 1 ].addr = slave_addr;
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

bool I2CBusController::write( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t data )
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
	msgs[ 0 ].addr = slave_addr;
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

bool I2CBusController::write( std::uint8_t slaveAddr, std::uint8_t reg, std::span< std::uint8_t > data )
{
	return write( slaveAddr, reg, data.data(), data.size() );
}

bool I2CBusController::write( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t size )
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
	msgs[ 0 ].addr = slave_addr;
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

void I2CBusController::sleep( std::chrono::milliseconds sleepTimeMs )
{
	std::this_thread::sleep_for( sleepTimeMs );
}

void I2CBusController::sleep( std::chrono::microseconds sleepTimeUs )
{
	std::this_thread::sleep_for( sleepTimeUs );
}

void I2CBusController::reportError()
{
	auto e = errno;
	std::array< char, 256 > err{};

	std::lock_guard _{ m_lastErrMtx };
	m_lastError = ::strerror_r( e, err.data(), err.size() );
}

void I2CBusController::checkFunc()
{
	std::uint64_t funcs{};

	if( ::ioctl( m_fd, I2C_FUNCS, &funcs ) < 0 )
	{
		return reportError();
	}

	std::cerr << "Supported funcions are: " << std::hex << funcs << std::endl;
	check( funcs, I2C_FUNC_I2C, "I2C_FUNC_I2C" );
	check( funcs, I2C_FUNC_10BIT_ADDR, "I2C_FUNC_10BIT_ADDR" );
	check( funcs, I2C_FUNC_PROTOCOL_MANGLING, "I2C_FUNC_PROTOCOL_MANGLING" );
	check( funcs, I2C_FUNC_SMBUS_PEC, "I2C_FUNC_SMBUS_PEC" );
	check( funcs, I2C_FUNC_NOSTART, "I2C_FUNC_NOSTART" );
	check( funcs, I2C_FUNC_SLAVE, "I2C_FUNC_SLAVE" );
	check( funcs, I2C_FUNC_SMBUS_BLOCK_PROC_CALL, "I2C_FUNC_SMBUS_BLOCK_PROC_CALL" );
	check( funcs, I2C_FUNC_SMBUS_QUICK, "I2C_FUNC_SMBUS_QUICK" );
	check( funcs, I2C_FUNC_SMBUS_READ_BYTE, "I2C_FUNC_SMBUS_READ_BYTE" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BYTE, "I2C_FUNC_SMBUS_WRITE_BYTE" );
	check( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA, "I2C_FUNC_SMBUS_READ_BYTE_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA, "I2C_FUNC_SMBUS_WRITE_BYTE_DATA" );
	check( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA, "I2C_FUNC_SMBUS_READ_WORD_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA, "I2C_FUNC_SMBUS_WRITE_WORD_DATA" );
	check( funcs, I2C_FUNC_SMBUS_PROC_CALL, "I2C_FUNC_SMBUS_PROC_CALL" );
	check( funcs, I2C_FUNC_SMBUS_READ_BLOCK_DATA, "I2C_FUNC_SMBUS_READ_BLOCK_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BLOCK_DATA, "I2C_FUNC_SMBUS_WRITE_BLOCK_DATA" );
	check( funcs, I2C_FUNC_SMBUS_READ_I2C_BLOCK, "I2C_FUNC_SMBUS_READ_I2C_BLOCK" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_I2C_BLOCK, "I2C_FUNC_SMBUS_WRITE_I2C_BLOCK" );
	check( funcs, I2C_FUNC_SMBUS_HOST_NOTIFY, "I2C_FUNC_SMBUS_HOST_NOTIFY" );
}

static_assert( std::is_same_v< __u8, std::uint8_t >, "__u8 definition differs from std::uint8_t definition." );
static_assert( std::is_same_v< unsigned int, std::uint32_t >,
			   "unsigned int definition differs from std::uint32_t definition." );

} // namespace PBL::I2C
