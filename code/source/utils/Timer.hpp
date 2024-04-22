#ifndef PBL_UTILS_TIMER_HPP__
#define PBL_UTILS_TIMER_HPP__

#include "TimeTypes.hpp"

namespace pbl::utils
{

/**
 * @class Timer
 * @brief Based on high_resolution_clock clock.
 * 
 * @note Conversion units:
 * 1s = 1000ms 		(milli-seconds)
 * 1s = 1000000us 	(micro-seconds)
 * 1s = 1e+9ns		(nano-seconds)
 */
class Timer final
{
public:
	/// Default ctor, starts the timer.
	Timer();

	/// Explicit call to start the timer.
	void start();

	/// Resets the timer ( calls start() internally )
	void reset();

	/// Returns elapsed time from the "start() call or timer object construction" in seconds.
	[[nodiscard]] double elapsedTimeS() const;

	/// Returns elapsed time from the "start() call or timer object construction" in milliseconds.
	[[nodiscard]] std::int64_t elapsedTimeMs() const;

	/// Returns elapsed time from the "start() call or timer object construction" in microseconds.
	[[nodiscard]] std::int64_t elapsedTimeUs() const;

	/// Returns elapsed time from the "start() call or timer object construction" in nanoseconds.
	[[nodiscard]] std::int64_t elapsedTimeNs() const;

private:
	Clock m_clock; //!< high_resolution_clock
	TimePointNs m_startTP; //!< Starting time stamp
};

} // namespace pbl::utils
#endif // PBL_UTILS_TIMER_HPP__
