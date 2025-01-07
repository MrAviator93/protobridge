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
	DEVICE_NOT_FOUND, ///< Device not found on the bus.
	HARDWARE_FAILURE, ///< General hardware malfunction, not specific to read/write.
	DEVICE_NOT_RESPONDING, ///< Device detected but not responding to commands.
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

	/// @name General Operation Errors
	/// General-purpose errors that do not fit specific categories.
	//@{
	UNSUPPORTED_OPERATION, ///< Operation not supported by the device.
	INVALID_DATA, ///< Data corruption or format error detected.
	RETRY_LIMIT_EXCEEDED, ///< Exceeded maximum retry attempts for the operation.
	UNEXPECTED_ERROR ///< Any unexpected or unknown error.
	//@}
};

/// Converts an ErrorCode to its string representation.
constexpr std::string_view toStringView( const ErrorCode error ) noexcept
{
	constexpr std::string_view kUndefined{"UNDEFINED"};

	switch( error )
	{
		using enum ErrorCode;
		case FAILED_TO_READ: return "FAILED_TO_READ";
		case FAILED_TO_WRITE: return "FAILED_TO_WRITE";
		case DEVICE_NOT_FOUND: return "DEVICE_NOT_FOUND";
		case HARDWARE_FAILURE: return "HARDWARE_FAILURE";
		case DEVICE_NOT_RESPONDING: return "DEVICE_NOT_RESPONDING";
		case BUS_BUSY: return "BUS_BUSY";
		case NACK_RECEIVED: return "NACK_RECEIVED";
		case TIMEOUT: return "TIMEOUT";
		case INVALID_ADDRESS: return "INVALID_ADDRESS";
		case DATA_OVERRUN: return "DATA_OVERRUN";
		case ARBITRATION_LOST: return "ARBITRATION_LOST";
		case ACCESS_DENIED: return "ACCESS_DENIED";
		case UNSUPPORTED_OPERATION: return "UNSUPPORTED_OPERATION";
		case INVALID_DATA: return "INVALID_DATA";
		case RETRY_LIMIT_EXCEEDED: return "RETRY_LIMIT_EXCEEDED";
		case UNEXPECTED_ERROR: return "UNEXPECTED_ERROR";
		default: return kUndefined;
	}

	// To make the compiler happy
	return kUndefined;
}

/**
 * @brief Represents an error with an associated error code and optional message.
 * 
 * @todo Consider to rename to Status.
 */
class Error final
{
public:
	explicit Error( ErrorCode code, std::optional< std::string > message = std::nullopt )
		: m_code{ code }
		, m_message{ std::move( message ) }
	{ }

	static const Error FAILED_TO_READ;
	static const Error FAILED_TO_WRITE;
	static const Error DEVICE_NOT_FOUND;
	static const Error HARDWARE_FAILURE;
	static const Error DEVICE_NOT_RESPONDING;
	static const Error BUS_BUSY;
	static const Error NACK_RECEIVED;
	static const Error TIMEOUT;
	static const Error INVALID_ADDRESS;
	static const Error DATA_OVERRUN;
	static const Error ARBITRATION_LOST;
	static const Error ACCESS_DENIED;
	static const Error UNSUPPORTED_OPERATION;
	static const Error INVALID_DATA;
	static const Error RETRY_LIMIT_EXCEEDED;
	static const Error UNEXPECTED_ERROR;

	/// Implicit conversion to ErrorCode.
	[[nodiscard]] operator ErrorCode() const noexcept { return m_code; }

	/// Returns the optional error message.
	[[nodiscard]] auto& message() const noexcept { return m_message; }

private:
	ErrorCode m_code;
	std::optional< std::string > m_message;
};

// TODO: Move these declarations to ErrorCode.cpp file.
inline const Error Error::FAILED_TO_READ{ ErrorCode::FAILED_TO_READ };
inline const Error Error::FAILED_TO_WRITE{ ErrorCode::FAILED_TO_WRITE };
inline const Error Error::DEVICE_NOT_FOUND{ ErrorCode::DEVICE_NOT_FOUND };
inline const Error Error::HARDWARE_FAILURE{ ErrorCode::HARDWARE_FAILURE };
inline const Error Error::DEVICE_NOT_RESPONDING{ ErrorCode::DEVICE_NOT_RESPONDING };
inline const Error Error::BUS_BUSY{ ErrorCode::BUS_BUSY };
inline const Error Error::NACK_RECEIVED{ ErrorCode::NACK_RECEIVED };
inline const Error Error::TIMEOUT{ ErrorCode::TIMEOUT };
inline const Error Error::INVALID_ADDRESS{ ErrorCode::INVALID_ADDRESS };
inline const Error Error::DATA_OVERRUN{ ErrorCode::DATA_OVERRUN };
inline const Error Error::ARBITRATION_LOST{ ErrorCode::ARBITRATION_LOST };
inline const Error Error::ACCESS_DENIED{ ErrorCode::ACCESS_DENIED };
inline const Error Error::UNSUPPORTED_OPERATION{ ErrorCode::UNSUPPORTED_OPERATION };
inline const Error Error::INVALID_DATA{ ErrorCode::INVALID_DATA };
inline const Error Error::RETRY_LIMIT_EXCEEDED{ ErrorCode::RETRY_LIMIT_EXCEEDED };
inline const Error Error::UNEXPECTED_ERROR{ ErrorCode::UNEXPECTED_ERROR };

} // namespace pbl::utils
#endif // PBL_UTILS_ERROR_CODE_HPP__
