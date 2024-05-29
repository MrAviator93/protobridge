#ifndef PBL_I2C_ERROR_CODE_HPP__
#define PBL_I2C_ERROR_CODE_HPP__

// C++
#include <cstdint>
#include <string_view>

namespace pbl::i2c
{

/// TODO: Consider to move to utils lib.
enum class ErrorCode : std::uint8_t
{
	FAILED_TO_READ, // General failure to read
	FAILED_TO_WRITE, // General failure to write
	DEVICE_NOT_FOUND, // Device not found on the bus
	BUS_BUSY, // Bus is busy or locked
	NACK_RECEIVED, // No acknowledgment received
	TIMEOUT, // Communication timed out
	INVALID_ADDRESS, // Invalid address specified
	DATA_OVERRUN, // Data overrun error
	ARBITRATION_LOST, // Arbitration lost during communication
	UNEXPECTED_ERROR // Any unexpected error
};

constexpr std::string_view toStringView( const ErrorCode error ) noexcept
{
	switch( error )
	{
		using enum ErrorCode;
		case FAILED_TO_READ: return "FAILED_TO_READ";
		case FAILED_TO_WRITE: return "FAILED_TO_WRITE";
		case DEVICE_NOT_FOUND: return "DEVICE_NOT_FOUND";
		case BUS_BUSY: return "BUS_BUSY";
		case NACK_RECEIVED: return "NACK_RECEIVED";
		case TIMEOUT: return "TIMEOUT";
		case INVALID_ADDRESS: return "INVALID_ADDRESS";
		case DATA_OVERRUN: return "DATA_OVERRUN";
		case ARBITRATION_LOST: return "ARBITRATION_LOST";
		case UNEXPECTED_ERROR: return "UNEXPECTED_ERROR";
		default: return "UNDEFINED";
	}

	return "UNDEFINED";
}

} // namespace pbl::i2c
#endif // PBL_I2C_ERROR_CODE_HPP__
