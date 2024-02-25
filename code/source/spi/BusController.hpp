#ifndef PBL_SPI_BUS_CONTROLLER_HPP__
#define PBL_SPI_BUS_CONTROLLER_HPP__

// C++
#include <mutex>
#include <atomic>
#include <string>
#include <cstdint>
#include <shared_mutex>

namespace PBL::SPI
{

class BusController
{
public:
	/// Default ctor opens a file descriptor.
	explicit BusController( const std::string& busName );

	/// Default dtor, closes file m_fd file.
	virtual ~BusController();

	/// Returns the OS name of the physical bus name
	[[nodiscard]] auto& bus() const { return m_busName; }

	/// Returns whether the I2C is open on the device.
	[[nodiscard]] bool isOpen() const { return m_open.load(); }

private:
	/// Retrieves error buffer and outputs it into the logger using make_error method.
	void reportError();

	void setLastError( std::string&& lastError )
	{
		std::lock_guard _{ m_lastErrMtx };
		m_lastError = std::move( lastError );
	}

private:
	const std::string m_busName; //!< I2C Bus name, i.e. "/dev/i2c-1"
	std::atomic_bool m_open{ false }; //!< Indicates whether the I2C bus is open

	mutable std::mutex m_fdMtx; //!< Locks the read write operations
	int m_fd{ -1 }; //!< File/device descriptor

	mutable std::shared_mutex m_lastErrMtx; //!< Locks the last_error string operations
	std::string m_lastError; //<! Stores the string of the last occurred error.
};

} // namespace PBL::SPI
#endif // PBL_SPI_BUS_CONTROLLER_HPP__
