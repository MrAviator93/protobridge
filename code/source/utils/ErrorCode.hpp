#ifndef PBL_UTILS_ERROR_CODE_HPP__
#define PBL_UTILS_ERROR_CODE_HPP__

// C++
#include <string>
#include <cstdint>
#include <optional>
#include <string_view>

namespace pbl::utils
{

/**
 * @brief Enum representing possible error codes for ProtoBridge library.
 * Grouped by category to improve readability and maintainability.
 * 
 * @todo Consider to rename to StatusCode
 */
enum class ErrorCode : std::uint8_t
{
	/// @name Read/Write Errors
	/// Errors related to reading and writing operations.
	//@{
	FAILED_TO_READ, ///< General failure during read operation.
	FAILED_TO_WRITE, ///< General failure during write operation.
	//@}

	/// @name Device and Hardware Errors
	/// Errors caused by device or hardware malfunctions.
	//@{
	DEVICE_NOT_FOUND, ///< Device is not present or not visible to the system (e.g. no /dev entry).
	DEVICE_NOT_RESPONDING, ///< Device detected but not responding to communication or commands.
	HARDWARE_NOT_AVAILABLE, ///< Hardware is present but access failed (e.g. open or init failed).
	HARDWARE_FAILURE, ///< General low-level hardware malfunction not tied to specific operation.
	//@}

	/// @name Protocol and Communication Errors
	/// Errors specific to protocol-level communication and signaling.
	//@{
	BUS_BUSY, ///< Bus is currently busy or locked.
	NACK_RECEIVED, ///< No acknowledgment received from the device.
	TIMEOUT, ///< Communication with the device timed out.
	INVALID_ADDRESS, ///< Invalid device address specified.
	DATA_OVERRUN, ///< Data overrun error; data exceeds buffer capacity.
	ARBITRATION_LOST, ///< Arbitration lost during communication.
	//@}

	/// @name Access and Permission Errors
	/// Errors related to access control and permission restrictions.
	//@{
	ACCESS_DENIED, ///< Insufficient permissions to access the device.
	//@}

	/// @name GPIO and Peripheral Errors
	/// Errors related specifically to GPIO chip and lines.
	//@{
	GPIO_CHIP_NOT_AVAILABLE, ///< Failed to open or access GPIO chip (e.g., gpiochip0).
	INVALID_GPIO_PIN, ///< GPIO pin is out of supported range or not mapped.
	//@}

	/// @name General Operation Errors
	/// General-purpose errors that do not fit specific categories.
	//@{
	UNSUPPORTED_OPERATION, ///< Operation not supported by the device.
	INVALID_DATA, ///< Data corruption or format error detected.
	INVALID_ARGUMENT, ///< Provided argument is not valid in this context.
	RETRY_LIMIT_EXCEEDED, ///< Exceeded maximum retry attempts for the operation.
	UNEXPECTED_ERROR, ///< Any unexpected or unknown error.
	NOT_IMPLEMENTED ///!< Not implemented.
	//@}

};

/// Converts an ErrorCode to its string representation.
[[nodiscard]] constexpr std::string_view toStringView( const ErrorCode error ) noexcept
{
	constexpr std::string_view kUndefined{ "UNDEFINED" };

	switch( error )
	{
		using enum ErrorCode;
		case FAILED_TO_READ: return "FAILED_TO_READ";
		case FAILED_TO_WRITE: return "FAILED_TO_WRITE";
		case DEVICE_NOT_FOUND: return "DEVICE_NOT_FOUND";
		case HARDWARE_FAILURE: return "HARDWARE_FAILURE";
		case DEVICE_NOT_RESPONDING: return "DEVICE_NOT_RESPONDING";
		case HARDWARE_NOT_AVAILABLE: return "HARDWARE_NOT_AVAILABLE";
		case BUS_BUSY: return "BUS_BUSY";
		case NACK_RECEIVED: return "NACK_RECEIVED";
		case TIMEOUT: return "TIMEOUT";
		case INVALID_ADDRESS: return "INVALID_ADDRESS";
		case DATA_OVERRUN: return "DATA_OVERRUN";
		case ARBITRATION_LOST: return "ARBITRATION_LOST";
		case ACCESS_DENIED: return "ACCESS_DENIED";
		case GPIO_CHIP_NOT_AVAILABLE: return "GPIO_CHIP_NOT_AVAILABLE";
		case INVALID_GPIO_PIN: return "INVALID_GPIO_PIN";
		case UNSUPPORTED_OPERATION: return "UNSUPPORTED_OPERATION";
		case INVALID_DATA: return "INVALID_DATA";
		case INVALID_ARGUMENT: return "INVALID_ARGUMENT";
		case RETRY_LIMIT_EXCEEDED: return "RETRY_LIMIT_EXCEEDED";
		case UNEXPECTED_ERROR: return "UNEXPECTED_ERROR";
		case NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
		default: return kUndefined;
	}

	// To make the compiler happy
	return kUndefined;
}

} // namespace pbl::utils
#endif // PBL_UTILS_ERROR_CODE_HPP__
