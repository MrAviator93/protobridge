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
#endif // PBL_UTILS_ERROR_HPP__
