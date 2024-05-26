#ifndef PBL_UTILS_TIMER_HPP__
#define PBL_UTILS_TIMER_HPP__

#include "TimeTypes.hpp"

namespace pbl::utils
{

/**
 * @brief A simple timer class to manage time intervals.
 * 
 * Allows setting a duration and checking if the duration has elapsed.
 */
class Timer final
{
public:
	using Duration = std::chrono::microseconds;

	/**
     * @brief Constructs a Timer with an optional duration.
     * 
     * @param duration The duration for the timer (default is 1 minute).
     */
	Timer( const std::chrono::microseconds duration = std::chrono::minutes( 1 ) );

	/// Sets the elapsed duration for the timer.
	void setElapsed( Duration duration ) { m_elapsedDuration = duration; }

	// Resets the start time to the current time.
	void set() { m_startTime = Clock::now(); }

	/// Returns true if the elapsed duration has passed since the start time.
	[[nodiscard]] bool hasElapsed() const;

	/// Returns the remaining time before the duration elapses.
	[[nodiscard]] Duration remainingTime() const;

private:
	Clock::time_point m_startTime;
	Duration m_elapsedDuration;
};

} // namespace pbl::utils
#endif // PBL_UTILS_TIMER_HPP__
