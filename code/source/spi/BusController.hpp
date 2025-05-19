#ifndef PBL_SPI_BUS_CONTROLLER_HPP__
#define PBL_SPI_BUS_CONTROLLER_HPP__

// C++
#include <mutex>
#include <chrono>
#include <atomic>
#include <string>
#include <cstdint>
#include <shared_mutex>

namespace pbl::spi
{

inline namespace v1
{

class BusController
{
public:
	/// SPI mode
	enum class Mode : std::uint8_t
	{
		MODE_0,
		MODE_1,
		MODE_2,
		MODE_3
	};

	/// SPI speed
	enum class Speed : std::uint32_t
	{
		SPEED_500KHZ = 500000,
		SPEED_1MHZ = 1000000,
		SPEED_5MHZ = 5000000,
		SPEED_10MHZ = 10000000
	};

	/// Bits per word
	enum class BitsPerWord : std::uint8_t
	{
		BITS_8 = 8,
		BITS_16 = 16
	};

	/// Default ctor opens a file descriptor.
	explicit BusController( const std::string& busName );

	/// Default dtor, closes file m_fd file.
	virtual ~BusController();

	/// Returns the OS name of the physical bus name
	[[nodiscard]] auto& bus() const { return m_busName; }

	/// Puts asleep calling thread for specified sleep time in milliseconds
	void sleep( const std::chrono::milliseconds sleepTimeMs );

	/// Puts asleep calling thread for specified sleep time in microseconds
	void sleep( const std::chrono::microseconds sleepTimeUs );

private:
	// This class is non-copyable and non-movable
	BusController( const BusController& ) = delete;
	BusController( BusController&& ) = delete;
	BusController operator=( const BusController& ) = delete;
	BusController operator=( BusController&& ) = delete;

	/// Configures the SPI bus
	[[nodiscard]] bool configure( Mode mode, Speed speed, BitsPerWord bitsPerWord );

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

} // namespace v1
} // namespace pbl::spi
#endif // PBL_SPI_BUS_CONTROLLER_HPP__
