#include "Error.hpp"

namespace pbl::utils
{

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
