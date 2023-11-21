#include "BusController.hpp"

// C++
#include <array>
#include <vector>
#include <sstream>

// C
extern "C" {
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
}

namespace PBL::SPI
{

BusController::BusController( const std::string& busName )
	: m_busName{ busName }
{
	m_fd = ::open( m_busName.c_str(), O_RDWR | O_NONBLOCK );

	if( m_fd < 0 )
	{
		reportError();
		return;
	}

	m_open = true;
}

BusController::~BusController()
{
	::close( m_fd );
	m_open = false;
}

void SPIBusController::reportError()
{
	auto e = errno;
    std::array< char, 256 > err;

	std::lock_guard _{ m_lastErrMtx };
	m_lastError = ::strerror_r( e, err.data(), err.size() );
}

} // namespace PBL::SPI
