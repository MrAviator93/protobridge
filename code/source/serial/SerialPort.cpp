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

// const static std::unordered_map< BaudRateType, std::uint32_t > BR_TO_LINUX_BR_TABLE = {
// 	{ BaudRateType::BR_110, B110 },
// 	{ BaudRateType::BR_300, B300 },
// 	{ BaudRateType::BR_600, B600 },
// 	{ BaudRateType::BR_1200, B1200 },
// 	{ BaudRateType::BR_2400, B2400 },
// 	{ BaudRateType::BR_4800, B4800 },
// 	{ BaudRateType::BR_9600, B9600 },
// 	{ BaudRateType::BR_19200, B19200 },
// 	{ BaudRateType::BR_38400, B38400 },
// 	{ BaudRateType::BR_56000, B57600 },
// 	{ BaudRateType::BR_115200, B115200 } };

} // namespace

v1::SerialPort::~SerialPort()
{
	close();
}

void v1::SerialPort::close()
{
	// TODO: Close the connection
}

auto v1::SerialPort::dataAvailable() -> Result< bool >
{
	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::SerialPort::read( ByteSpan data ) -> Result< std::uint32_t >
{
	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::SerialPort::write( const ConstByteSpan data ) -> Result< std::uint32_t >
{
	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::SerialPort::open() -> Result< SerialPort >
{
	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

} // namespace pbl::serial
