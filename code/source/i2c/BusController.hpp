#ifndef PBL_I2C_BUS_CONTROLLER_HPP__
#define PBL_I2C_BUS_CONTROLLER_HPP__

#include "Transport.hpp"
#include <utils/Result.hpp>
#include <utils/Counter.hpp>

// C++
#include <bit>
#include <span>
#include <array>
#include <mutex>
#include <vector>
#include <atomic>
#include <string>
#include <chrono>
#include <cstdint>
#include <shared_mutex>

namespace pbl::i2c
{

inline namespace v1
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
class BusController : public Transport, public utils::Counter< BusController >
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	/// Transfer ownership before attaching controllers; moving requires exclusive access.
	BusController( BusController&& other ) noexcept;

	/// A bus may already have controllers borrowing it, so ownership cannot be replaced.
	BusController& operator=( BusController&& ) = delete;

	/// Closes the owned descriptor, including after a failed initialization.
	~BusController() override;

	/// Opens a bus supporting raw I2C transfers, or returns the OS failure with context.
	[[nodiscard]] static Result< BusController > open( const std::string& busName );

	/// Returns the OS name of the physical bus name (TODO: Consider renaming to isActive or isReady)
	[[nodiscard]] auto& bus() const { return m_busName; }

	/// Returns whether the I2C is open on the device.
	[[nodiscard]] bool isOpen() const noexcept { return m_open.load(); }

	/**
     * @brief Read a single byte from specified register
     * 
     * @param deviceAddr 
     * @param reg 
     * @param result 
     * @return true 
     * @return false 
     */
	[[nodiscard]] bool read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::uint8_t& result );

	/**
     * @brief Read a two byte array from specified register
     * 
     * @param deviceAddr 
     * @param reg 
     * @param result 
     * @return true 
     * @return false 
     */
	[[nodiscard]] bool
	read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::array< std::uint8_t, 2 >& result );

	/**
     * @brief Read a four byte array from specified register
     * 
     * @param deviceAddr 
     * @param reg 
     * @param result 
     * @return true 
     * @return false 
     */
	[[nodiscard]] bool
	read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::array< std::uint8_t, 4 >& result );

	/// TODO: To be introduced, default endian in i2c comms is big, if mismatch convert to native
	[[nodiscard]] bool read( const std::uint8_t deviceAddr,
							 const std::uint8_t reg,
							 std::int16_t& value,
							 const std::endian endian = std::endian::big );

	/// TODO: To be introduced, default endian in i2c comms is big, if mismatch convert to native
	[[nodiscard]] bool read( const std::uint8_t deviceAddr,
							 const std::uint8_t reg,
							 std::int32_t& value,
							 const std::endian endian = std::endian::big );

	/**
     * @brief Read a data buffer from specified register (raw pointer version)
	 * 
	 * @todo There is a mismatch in types dataSize is uint16_t but the real read size is int16_t
	 * this needs to be fixed.
     * 
     * @param deviceAddr 
     * @param reg 
     * @param pData 
     * @param dataSize
     * @return std::int16_t 
     */
	[[nodiscard]] std::int16_t
	read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::uint8_t* pData, std::uint16_t dataSize );

	/**
     * @brief Read a data buffer from specified register (std::vector version)
	 * 
	 * @param deviceAdd 
	 * @param reg 
	 * @param data 
	 * @return std::int16_t 
	 */
	[[nodiscard]] std::int16_t
	read( const std::uint8_t deviceAdd, const std::uint8_t reg, std::vector< std::uint8_t >& data )
	{
		return read( deviceAdd, reg, data.data(), static_cast< std::uint16_t >( data.size() ) );
	}

	/// Reads the complete byte buffer.
	utils::Result< void > read( Address deviceAddr, std::span< std::uint8_t > data ) override;

	/// Writes the complete byte buffer.
	utils::Result< void > write( Address deviceAddr, std::span< const std::uint8_t > data ) override;

	utils::Result< void > writeRead( Address address, std::span< const std::uint8_t > request,
		std::span< std::uint8_t > response ) override;

	/**
     * @brief Write a single byte to the specified register
     * 
     * @param deviceAddr 
     * @param reg 
     * @param data 
     * @return true 
     * @return false 
     */
	bool write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::uint8_t data );

	/**
	 * @brief Write a data buffer to specified register
	 * 
	 * @param deviceAddr 
	 * @param reg 
	 * @param data 
	 * @return true 
	 * @return false 
	 */
	virtual bool
	write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::span< const std::uint8_t > data );

	/**
     * @brief Write a data buffer to specified register
     * 
     * @param deviceAddr 
     * @param reg 
     * @param data 
     * @param size 
     * @return true 
     * @return false 
     */
	bool
	write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::uint8_t* data, const std::uint8_t size );

	/// An accessor to the last error
	[[nodiscard]] std::string lastError() const
	{
		std::shared_lock _{ m_lastErrMtx };
		return m_lastError;
	}

	/// Puts asleep calling thread for specified sleep time in milliseconds
	void sleep( const std::chrono::milliseconds sleepTimeMs );

	/// Puts asleep calling thread for specified sleep time in microseconds
	void sleep( const std::chrono::microseconds sleepTimeUs );

private:
	// This class is non-copyable
	explicit BusController( std::string busName );

	BusController( const BusController& ) = delete;
	BusController& operator=( const BusController& ) = delete;

	/// Retrieves error buffer
	void reportError();

	/// Requesting the bus for capabilities/features/functionality
	[[nodiscard]] Result< void > checkFunc();

	void setLastError( std::string&& errorMessage )
	{
		std::lock_guard _{ m_lastErrMtx };
		m_lastError = std::move( errorMessage );
	}

private:
	std::string m_busName; //!< I2C Bus name, i.e. "/dev/i2c-1"
	std::atomic_bool m_open{ false }; //!< Indicates whether the I2C bus is open

	mutable std::mutex m_fdMtx; //!< Locks the read write operations
	int m_fd{ -1 }; //!< TBW

	mutable std::shared_mutex m_lastErrMtx; //!< Locks the last_error string operations
	std::string m_lastError; //<! TBW
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_BUS_CONTROLLER_HPP__
