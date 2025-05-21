#ifndef PBL_SERIAL_SERIAL_PORT_HPP__
#define PBL_SERIAL_SERIAL_PORT_HPP__

#include "BaudRate.hpp"
#include <utils/Result.hpp>

// C++
#include <span>
#include <mutex>
#include <atomic>
#include <cstdint>

namespace pbl::serial
{

inline namespace v1
{

/**
 * @class SerialPort
 * @brief Serial port class wrapper, that wraps serial port read/write complexities.
 * 
 * @note Baud rate must match the other device's settings being communicated to.
 * If it does not match, the communication can't be established, at both ends
 * either corrupted or no data will be received!
 * 
 * @note Use this class when it's needed to communicate with external devices through serial.
 * It can be usefull to implement higher level abstractions and internally utilise this class.
 * 
 * @note For Linux system users it's necessary that the user <user_name> group is also added
 * to the dialout group, i.e. sudo adduser <user_name> dialout.
 * or the following way: sudo usermod -a -G dialout <user_name>
 * To check type groups <user_name>
 * Also for Linux with dmesg you may check to which tty the serial is attached to.
 */
class SerialPort final
{
	struct PrivateTag
	{ };

public:
	using ByteSpan = std::span< std::uint8_t >;
	using ConstByteSpan = std::span< const std::uint8_t >;

	template < typename T >
	using Result = utils::Result< T >;

	/**
     * @brief Enumerates the valid data bit lengths for serial transmission.
     * 
     * SB5 - Uses 5 data bits per character, typically found in legacy systems like teleprinters using Baudot code.
     * SB6 - Utilizes 6 data bits per character; uncommon and seldom used in modern applications.
     * SB7 - Encodes characters with 7 data bits, aligning with standard ASCII; primarily used in older communication devices.
     * SB8 - Assigns 8 data bits per character, equivalent to one byte; this is the most prevalent format in contemporary systems.
     */
	enum class SuppNoOfBits
	{
		SB5,
		SB6,
		SB7,
		SB8
	};

	/**
     * @brief Defines available parity modes for serial communication.
     * 
     * NO_PARITY - Disables parity bit usage; commonly used where error checking is handled at a higher protocol level.
     * EVEN_PARITY - Ensures the total number of 1 bits (including the parity bit) is even for each character.
     * ODD_PARITY - Guarantees the count of 1 bits (including the parity bit) is odd, often used to maintain signal transitions.
     * Note: Other forms like SPACE_PARITY and MARK_PARITY exist but are not supported here.
     */
	enum class Parity
	{
		NO_PARITY,
		EVEN_PARITY,
		ODD_PARITY
	};

	/**
     * @brief Lists supported configurations for stop bits in serial data frames.
     * 
     * NONE - No stop bit is transmitted (rarely used).
     * ONE_STOP_BIT - A single stop bit is appended; standard in most serial communication protocols.
     * ONE_HALF_STOP_BIT - One and a half stop bits; this setting is specific to the Windows platform.
     * TWO_STOP_BITS - Two stop bits used for more reliable separation between characters, especially at lower baud rates.
     */
	enum class StopBits
	{
		NONE,
		ONE_STOP_BIT,
		ONE_HALF_STOP_BIT,
		TWO_STOP_BITS
	};

	using enum SuppNoOfBits;
	using enum Parity;
	using enum StopBits;

	struct Settings
	{
		BaudRate baudRate{ BaudRate::BR_9600 }; // Default to 9600
		Parity parity{ Parity::NO_PARITY }; // No parity
		StopBits stopBits{ StopBits::ONE_STOP_BIT }; // 1 stop bit
		SuppNoOfBits dataBits{ SuppNoOfBits::SB8 }; // 8 data bits
	};

	SerialPort( int fd, Settings settings, PrivateTag )
		: m_fd{ fd }
		, m_settings{ std::move( settings ) }
	{
		m_isOpen.test_and_set( std::memory_order_release );
	}

	SerialPort( SerialPort&& ) noexcept = default;
	SerialPort& operator=( SerialPort&& ) noexcept = default;

	/// Default dtor, closes the serial connection internally (if the connection is open).
	~SerialPort();

	/// TBW
	[[nodiscard]] static Result< SerialPort > open( const std::string& device, Settings settings );

	/// Closes the serial connection internally (if the connection is open).
	void close();

	/// Returns true if the serial port is currently open.
	[[nodiscard]] bool isOpen() const noexcept { return m_isOpen.test( std::memory_order_relaxed ); }

	/// Returns true if there is data available to read on the serial port or error.
	[[nodiscard]] Result< bool > dataAvailable();

	/**
	 * @brief Reads data from the serial port into the provided buffer.
	 * 
	 * @param data The buffer to store the read bytes.
	 * @param timeoutUs Timeout in microseconds to wait for data before aborting.
	 * @return Result<std::uint32_t> Number of bytes successfully read or an error.
	 */
	[[nodiscard]] Result< std::uint32_t > read( ByteSpan data, std::uint32_t timeoutUs );

	/**
	 * @brief Writes data to the serial port.
	 * 
	 * @param data Span of bytes to send.
	 * @return Result<std::uint32_t> Number of bytes written or an error.
	 */
	[[nodiscard]] Result< std::uint32_t > write( const ConstByteSpan data );

private:
	SerialPort( const SerialPort& ) = delete;
	SerialPort& operator=( const SerialPort& ) = delete;

private:
	std::atomic_flag m_isOpen{ ATOMIC_FLAG_INIT };

	std::mutex m_deviceMtx{};
	int m_fd{ -1 };

	Settings m_settings;
};

} // namespace v1
} // namespace pbl::serial
#endif // PBL_SERIAL_SERIAL_PORT_HPP__
