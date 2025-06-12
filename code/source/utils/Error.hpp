#ifndef PBL_UTILS_ERROR_HPP__
#define PBL_UTILS_ERROR_HPP__

#include "ErrorCode.hpp"

namespace pbl::utils
{

/**
 * @brief Represents an error with a structured error code and optional message.
 *
 * This class is intended for use with `std::expected<T, Error>` (aliased as `Result<T>`) to provide
 * expressive error handling across the ProtoBridge library. It provides both standardized
 * error codes and human-readable messages to support diagnostics and user-facing reporting.
 *
 * Example:
 * @code
 * Result<int> readResult = readFile("config.txt");
 * if (!readResult) {
 *     std::cerr << "Read failed: " << readResult.error().description() << '\n';
 * }
 * @endcode
 */
class Error final
{
public:
	/**
	 * @brief Constructs an error with the given code and optional message.
	 *
	 * @param code    The error code representing the type of failure.
	 * @param message Optional additional context or description.
	 */
	explicit Error( ErrorCode code, std::optional< std::string > message = std::nullopt )
		: m_code{ code }
		, m_message{ std::move( message ) }
	{ }

	/// Common predefined error instances for reuse throughout the library.
	static const Error END_OF_FILE;
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
	static const Error NOT_IMPLEMENTED;

	/// Implicit conversion to ErrorCode.
	[[nodiscard]] operator ErrorCode() const noexcept { return m_code; }

	/// Returns the optional user-defined error message.
	[[nodiscard]] auto& message() const noexcept { return m_message; }

	/// Returns a formatted string containing the error code and optional user-defined message.
	[[nodiscard]] std::string description() const;

private:
	ErrorCode m_code;
	std::optional< std::string > m_message;
};

} // namespace pbl::utils
#endif // PBL_UTILS_ERROR_HPP__
