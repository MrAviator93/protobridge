#ifndef PBL_I2C_IC_ERROR_HPP__
#define PBL_I2C_IC_ERROR_HPP__

// C++
#include <cstdint>

namespace pbl::i2c
{

enum class ICError : std::uint8_t
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

} // namespace pbl::i2c
#endif // PBL_I2C_IC_ERROR_HPP__
