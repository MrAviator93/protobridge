#ifndef PBL_SERIAL_SERIAL_PORT_HPP__
#define PBL_SERIAL_SERIAL_PORT_HPP__

#include <utils/Result.hpp>

// C++
#include <span>
#include <cstdint>

namespace pbl::serial
{

inline namespace v1
{

class SerialPort final
{
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

	/// Default dtor, closes the serial connection internally (if the connection is open).
	~SerialPort();

	/// Closes the serial connection internally (if the connection is open).
	void close();

	/// Data available for read.
	[[nodiscard]] Result< bool > dataAvailable();

	/// TBW
	[[nodiscard]] Result< std::uint32_t > read( ByteSpan data );

	/// TBW
	[[nodiscard]] Result< std::uint32_t > write( const ConstByteSpan data );

	/// TBW
	[[nodiscard]] static Result< SerialPort > open();

private:
	SerialPort( const SerialPort& ) = delete;
	SerialPort& operator=( const SerialPort& ) = delete;
	SerialPort( SerialPort&& ) = delete;
	SerialPort& operator=( SerialPort&& ) = delete;
};

} // namespace v1
} // namespace pbl::serial
#endif // PBL_SERIAL_SERIAL_PORT_HPP__
