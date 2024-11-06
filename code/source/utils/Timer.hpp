#ifndef PBL_UTILS_TIMER_HPP__
#define PBL_UTILS_TIMER_HPP__

#include "TimeTypes.hpp"

// C++
#include <type_traits>

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
	using Dt = double;
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

	/// Returns the time elapsed since the timer was last set.
	[[nodiscard]] Duration elapsedSinceSet() const
	{
		return std::chrono::duration_cast< Duration >( Clock::now() - m_startTime );
	}

	/// Returns the time elapsed since the timer was last set in seconds float.
	[[nodiscard]] float elapsedSinceSetInSeconds() const
	{
		const auto elapsedTime = Clock::now() - m_startTime;
		return std::chrono::duration< float >( elapsedTime ).count();
	}

	/// TODO: This needs a bit more work, maybe we want to return
	/// std::expected<bool, utils::ErrorCode or utils::Error>
	template < typename Callback >
		requires std::invocable< Callback, Dt >
	auto onTick( Callback&& callback ) -> std::invoke_result_t< Callback, Dt >
	{
		if( hasElapsed() )
		{
			Dt dt = elapsedSinceSetInSeconds();

			// If lambda returns void
			if constexpr( std::is_void_v< std::invoke_result_t< Callback, Dt > > )
			{
				std::forward< Callback >( callback )( dt );
				set();
			}
			else // We return the result of invocation
			{
				auto result = std::forward< Callback >( callback )( dt );
				set();
				return result;
			}
		}

		return {};
	}

private:
	Clock::time_point m_startTime;
	Duration m_elapsedDuration;
};

} // namespace pbl::utils
#endif // PBL_UTILS_TIMER_HPP__
