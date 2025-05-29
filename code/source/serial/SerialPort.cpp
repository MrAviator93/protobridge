#include "SerialPort.hpp"

// C++
#include <format>
#include <unordered_map>

#if defined( __linux__ )
extern "C" {
#include <linux/serial.h>
#include <sys/ioctl.h>
}
#endif

extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
}

namespace pbl::serial
{

namespace
{

const std::unordered_map< std::uint32_t, std::uint32_t > kBrToLinuxBrTable = {
	{ static_cast< std::uint32_t >( BaudRate::BR_110 ), B110 },
	{ static_cast< std::uint32_t >( BaudRate::BR_300 ), B300 },
	{ static_cast< std::uint32_t >( BaudRate::BR_600 ), B600 },
	{ static_cast< std::uint32_t >( BaudRate::BR_1200 ), B1200 },
	{ static_cast< std::uint32_t >( BaudRate::BR_2400 ), B2400 },
	{ static_cast< std::uint32_t >( BaudRate::BR_4800 ), B4800 },
	{ static_cast< std::uint32_t >( BaudRate::BR_9600 ), B9600 },
	{ static_cast< std::uint32_t >( BaudRate::BR_19200 ), B19200 },
	{ static_cast< std::uint32_t >( BaudRate::BR_38400 ), B38400 },
	{ static_cast< std::uint32_t >( BaudRate::BR_56000 ), B57600 },
	{ static_cast< std::uint32_t >( BaudRate::BR_115200 ), B115200 } };

[[maybe_unused]] void setParityFlag( ::termios& tty, SerialPort::Parity p )
{
	switch( p )
	{
		case SerialPort::Parity::EVEN_PARITY:
			tty.c_cflag &= ~PARODD;
			tty.c_cflag |= ( PARENB );
			break;
		case SerialPort::Parity::ODD_PARITY: {
			tty.c_cflag |= ( PARENB | PARODD );
			break;
		}
		default: {
			// Clear parity bit, disabling parity
			tty.c_cflag &= ~( PARENB | PARODD );
		}
	}
}

[[maybe_unused]] void setStopBitsFlag( ::termios& tty, SerialPort::StopBits s )
{
	switch( s )
	{
		case SerialPort::StopBits::ONE_STOP_BIT: {
			tty.c_cflag &= ~CSTOPB;
			break;
		}
		case SerialPort::StopBits::ONE_HALF_STOP_BIT: {
			// ONE POINT FIVE same as TWO.. there is no POSIX support for 1.5
			tty.c_cflag |= CSTOPB;
			break;
		}
		case SerialPort::StopBits::TWO_STOP_BITS: {
			tty.c_cflag |= CSTOPB;
			break;
		}
		default: {
			tty.c_cflag &= ~CSTOPB;
		}
	}
}

[[maybe_unused]] void setNoOfBitsPByte( ::termios& tty, SerialPort::SuppNoOfBits s )
{
	tty.c_cflag &= ~CSIZE; // Clear the size bits
	switch( s )
	{
		case SerialPort::SuppNoOfBits::SB5: {
			tty.c_cflag |= CS5; // 5 bits per byte
			break;
		}
		case SerialPort::SuppNoOfBits::SB6: {
			tty.c_cflag |= CS6; // 6 bits per byte
			break;
		}
		case SerialPort::SuppNoOfBits::SB7: {
			tty.c_cflag |= CS7; // 7 bits per byte
			break;
		}
		default: {
			tty.c_cflag |= CS8; // 8 bits per byte
		}
	}
}

} // namespace

auto v1::SerialPort::open( [[maybe_unused]] const std::string& device, Settings settings ) -> Result< SerialPort >
{
	int fd = ::open( device.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK );
	if( fd < 0 ) [[unlikely]]
	{
		// return utils::MakeError( utils::ErrorCode::FAILED_TO_OPEN );
		return utils::MakeError( utils::ErrorCode::UNEXPECTED_ERROR );
	}

	// Clear NONBLOCK flag now that we're open
	int flags = ::fcntl( fd, F_GETFL, 0 );
	::fcntl( fd, F_SETFL, flags & ~O_NONBLOCK );

	// Configure the serial (configure the termios struct)
	::termios tty{};
	if( ::tcgetattr( fd, &tty ) != 0 ) [[unlikely]]
	{
		::close( fd );
		// return utils::MakeError( utils::Error::FAILED_TO_CONFIGURE );
		return utils::MakeError( utils::ErrorCode::UNEXPECTED_ERROR );
	}

	// Set-up parity - most serial communications do not use a parity bit
	setParityFlag( tty, settings.parity );

	// Set-up stop bits - most serial communications only use one stop bit
	setStopBitsFlag( tty, settings.stopBits );

	// Set number of bits per byte
	setNoOfBitsPByte( tty, settings.dataBits );

	// Flow control (CRTSCTS)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON; // Disable canonical mode
	tty.c_lflag &= ~ECHO; // Disable echo
	// tty.c_lflag &= ~ECHOK; // Disable ...
	// tty.c_lflag &= ~ECHOKE; // Disable ...
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	// tty.c_lflag &= ~IEXTEN; // Disable ...

	tty.c_iflag &= ~( IXON | IXOFF | IXANY ); // Turn off s/w flow ctrl
	tty.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
					  ICRNL ); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)

	// tty.c_cc[ VTIME ] = 10; // Wait for up to 1s (10 deci-seconds), returning as soon as any data is received.
	// tty.c_cc[ VMIN ] = 0;

	tty.c_cc[ VTIME ] = 0;
	tty.c_cc[ VMIN ] = 0;

	// Try to set the baud rates
	const auto it = kBrToLinuxBrTable.find( settings.baudRate.value() );
	if( it == kBrToLinuxBrTable.cend() ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
	}

	::cfsetispeed( &tty, it->second );
	::cfsetospeed( &tty, it->second );

	if( ::tcsetattr( fd, TCSANOW, &tty ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
	}

	return utils::MakeSuccess< SerialPort >( std::in_place, fd, std::move( settings ), PrivateTag{} );
}

v1::SerialPort::~SerialPort()
{
	close();
}

void v1::SerialPort::close()
{
	if( isOpen() ) [[likely]]
	{
		std::lock_guard _{ m_deviceMtx };
		::close( m_fd );

		m_isOpen.clear( std::memory_order_release );
	}
}

auto v1::SerialPort::dataAvailable() -> Result< bool >
{
	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::SerialPort::read( ByteSpan data, std::chrono::microseconds timeoutUs ) -> Result< std::uint32_t >
{
	if( !isOpen() ) [[unlikely]]
	{
		return utils::MakeError( utils::Error::FAILED_TO_READ );
	}

	std::lock_guard _{ m_deviceMtx };
	::fd_set set;
	FD_ZERO( &set ); // Clear the set
	FD_SET( m_fd, &set ); // Add the file descriptor to the set

	::timeval timeout;
	timeout.tv_sec = static_cast< time_t >( timeoutUs.count() / 1'000'000 );
	timeout.tv_usec = static_cast< suseconds_t >( timeoutUs.count() % 1'000'000 );

	const int rv = ::select( m_fd + 1, &set, nullptr, nullptr, &timeout );
	if( rv == -1 ) [[unlikely]]
	{
		// Error has occurred
		// setLastError( std::move( getLastSysError() ) );
		return utils::MakeError( utils::Error::FAILED_TO_READ );
	}
	else if( rv == 0 ) [[unlikely]]
	{
		// Timeout has occurred
		return utils::MakeError( utils::Error::TIMEOUT );
	}

	// There is something to read
	const auto readRslt = ::read( m_fd, data.data(), data.size() );
	if( readRslt == -1 ) [[unlikely]]
	{
		// Error has occurred
		return utils::MakeError( utils::Error::FAILED_TO_READ );
	}
	else if( readRslt == 0 ) [[unlikely]]
	{
		return utils::MakeError( utils::Error::END_OF_FILE );
	}

	return utils::MakeSuccess( static_cast< std::uint32_t >( readRslt ) );
}

auto v1::SerialPort::write( const ConstByteSpan data ) -> Result< std::uint32_t >
{
	if( !isOpen() ) [[unlikely]]
	{
		return utils::MakeError( utils::Error::FAILED_TO_WRITE );
	}

	std::lock_guard _{ m_deviceMtx };
	const auto rslt = ::write( m_fd, data.data(), data.size() );
	if( rslt == -1 ) [[unlikely]]
	{
		return utils::MakeError( utils::Error::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess( static_cast< std::uint32_t >( rslt ) );
}

} // namespace pbl::serial
