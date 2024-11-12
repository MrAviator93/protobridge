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

v1::BusController::BusController( const std::string& busName )
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

v1::BusController::~BusController()
{
	::close( m_fd );
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

bool v1::BusController::configure( Mode mode, Speed speed, BitsPerWord bitsPerWord )
{
	if( !m_open.load() )
	{
		setLastError( "SPI device is not open." );
		return false;
	}

	std::uint8_t modeValue = toSpiMode( mode );
	if( ::ioctl( m_fd, SPI_IOC_WR_MODE, &modeValue ) == -1 )
	{
		setLastError( "Failed to set SPI mode." );
		return false;
	}

	std::uint8_t bitsValue = static_cast< uint8_t >( bitsPerWord );
	if( ::ioctl( m_fd, SPI_IOC_WR_BITS_PER_WORD, &bitsValue ) == -1 )
	{
		setLastError( "Failed to set bits per word." );
		return false;
	}

	std::uint32_t speedValue = static_cast< uint32_t >( speed );
	if( ::ioctl( m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speedValue ) == -1 )
	{
		setLastError( "Failed to set SPI speed." );
		return false;
	}

	return true;
}

void v1::BusController::reportError()
{
	auto e = errno;
	std::array< char, 256 > err;

	std::lock_guard _{ m_lastErrMtx };
	m_lastError = ::strerror_r( e, err.data(), err.size() );
}

} // namespace pbl::spi
