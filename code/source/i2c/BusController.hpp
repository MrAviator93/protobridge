#ifndef PBL_I2C_BUS_CONTROLLER_HPP__
#define PBL_I2C_BUS_CONTROLLER_HPP__

// C++
#include <span>
#include <mutex>
#include <vector>
#include <atomic>
#include <string>
#include <chrono>
#include <cstdint>
#include <shared_mutex>

namespace pbl::i2c
{

/**
 * @class BusController
 * @brief An interface wrapper for I2C bus.
 * To list the I2C buses available: i2cdetect -l or you may use also: ls /dev/i2c*
 * Say, 0 & 1 are available.
 * Then, each bus could be scanned to see what all device addresses exist on each bus.
 * i2cdetect -y 0 or with i2cdetect -y -r 0
 * 
 * @todo Rename to BusController
 * 
 * @author AK aka MrAviator93
 */
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

	/**
     * @brief Read a single byte from specified register
     * 
     * @param slaveAddr 
     * @param reg 
     * @param result 
     * @return true 
     * @return false 
     */
	[[nodiscard]] bool read( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t& result );

	/**
     * @brief Read a data buffer from specified register (raw pointer version)
	 * 
	 * @todo There is a mismatch in types dataSize is uint16_t but the real read size is int16_t
	 * this needs to be fixed.
     * 
     * @param slaveAddr 
     * @param reg 
     * @param pData 
     * @param dataSize
     * @return std::int16_t 
     */
	[[nodiscard]] std::int16_t
	read( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t* pData, std::uint16_t dataSize );

	/**
     * @brief Read a data buffer from specified register (std::vector version)
	 * 
	 * @param slaveAdd 
	 * @param reg 
	 * @param data 
	 * @return std::int16_t 
	 */
	[[nodiscard]] std::int16_t read( std::uint8_t slaveAdd, std::uint8_t reg, std::vector< std::uint8_t >& data )
	{
		return read( slaveAdd, reg, data.data(), static_cast< std::uint16_t >( data.size() ) );
	}

	/**
     * @brief Write a single byte to the specified register
     * 
     * @param slaveAddr 
     * @param reg 
     * @param data 
     * @return true 
     * @return false 
     */
	bool write( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t data );

	/**
	 * @brief Write a data buffer to specified register
	 * 
	 * @param slaveAddr 
	 * @param reg 
	 * @param data 
	 * @return true 
	 * @return false 
	 */
	virtual bool write( std::uint8_t slaveAddr, std::uint8_t reg, std::span< std::uint8_t > data );

	/**
     * @brief Write a data buffer to specified register
     * 
     * @param slaveAddr 
     * @param reg 
     * @param data 
     * @param size 
     * @return true 
     * @return false 
     */
	bool write( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t* data, std::uint8_t size );

	/// An accessor to the last error
	[[nodiscard]] std::string lastError() const
	{
		std::shared_lock _{ m_lastErrMtx };
		return m_lastError;
	}

	/// Puts asleep calling thread for specified sleep time in milliseconds
	void sleep( std::chrono::milliseconds sleepTimeMs );

	/// Puts asleep calling thread for specified sleep time in microseconds
	void sleep( std::chrono::microseconds sleepTimeUs );

private:
	// This class is non-copyable and non-movable
	BusController( const BusController& ) = delete;
	BusController( BusController&& ) = delete;
	BusController operator=( const BusController& ) = delete;
	BusController operator=( BusController&& ) = delete;

	/// Retrieves error buffer
	void reportError();

	/// Requesting the bus for capabilities/features/functionality
	void checkFunc();

	void setLastError( std::string&& errorMessage )
	{
		std::lock_guard _{ m_lastErrMtx };
		m_lastError = std::move( errorMessage );
	}

	// The idea is to keep track of what IC's are we driving, haven't
	// we by accident assigned the same address to 2 or more ICs?
	// void attach(ICBase& ic);
	// void detach(ICBase& ic);

private:
	const std::string m_busName; //!< I2C Bus name, i.e. "/dev/i2c-1"
	std::atomic_bool m_open{ false }; //!< Indicates whether the I2C bus is open

	mutable std::mutex m_fdMtx; //!< Locks the read write operations
	int m_fd{ -1 }; //!< TBW

	mutable std::shared_mutex m_lastErrMtx; //!< Locks the last_error string operations
	std::string m_lastError; //<! TBW
};

} // namespace pbl::i2c
#endif // PBL_I2C_BUS_CONTROLLER_HPP__
