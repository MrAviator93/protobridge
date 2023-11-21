
#ifndef I2C_UTILS_TIME_TYPES_HPP__
#define I2C_UTILS_TIME_TYPES_HPP__

#include <chrono>

namespace I2C
{

using Clock = std::chrono::high_resolution_clock;
using TimePointS = std::chrono::time_point< Clock, std::chrono::seconds >;
using TimePointMs = std::chrono::time_point< Clock, std::chrono::milliseconds >;
using TimePointUs = std::chrono::time_point< Clock, std::chrono::microseconds >;
using TimePointNs = std::chrono::time_point< Clock, std::chrono::nanoseconds >;

using Seconds = std::chrono::seconds; // Signed integer type of at least 35 bits
using Milliseconds = std::chrono::milliseconds; // Signed integer type of at least 45 bits
using Microseconds = std::chrono::microseconds; // Signed integer type of at least 55 bits
using Nanoseconds = std::chrono::nanoseconds; // Signed integer type of at least 64 bits

} // namespace I2C
#endif // I2C_UTILS_TIME_TYPES_HPP__
