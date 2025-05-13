#ifndef PBL_UTILS_ERROR_HPP__
#define PBL_UTILS_ERROR_HPP__

#include "ErrorCode.hpp"

namespace pbl::utils
{

/// Represents an error with an associated error code and optional message.
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



} // namespace pbl::utils
#endif // PBL_UTILS_ERROR_HPP__
