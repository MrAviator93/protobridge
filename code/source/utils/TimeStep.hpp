
#ifndef I2C_UTILS_TIME_STEP_HPP__
#define I2C_UTILS_TIME_STEP_HPP__

#include "TimeTypes.hpp"

namespace PBL::Utils
{

/**
 * @brief Encapsulates a time duration in a type-safe manner.
 * 
 * This class abstracts time durations, promoting type safety and code readability. 
 * It is designed to be used throughout the project wherever fixed-duration time steps 
 * are required, replacing less descriptive primitive types (e.g., raw doubles).
 * 
 * @tparam T The specific duration type (e.g., Milliseconds) this TimeStep represents.
 */
template < typename T = Milliseconds >
class TimeStep final
{
public:
	/// Constructs a default TimeStep with zero duration
	TimeStep()
		: m_duration{}
	{ }

	/// Constructs TimeStep with the specified duration.
	TimeStep( const T& duration )
		: m_duration( duration )
	{ }

	/// Explicitly returns the internal duration
	[[nodiscard]] explicit operator T() const { return m_duration; }

	/// TBW
	[[nodiscard]] auto duration() const { return m_duration; }

	/// TBW
	void setDuration( const T& duration ) { m_duration = duration; }

	/// Provides the duration value in seconds as a floating-point number
	[[nodiscard]] double toSeconds() const
	{
		// No need for duration_cast; duration's constructor does the conversion.
		// This avoids rounding to whole milliseconds, microseconds, etc.
		return std::chrono::duration< double, std::ratio< 1 > >( m_duration ).count();
	}

	/// Assignment operator that accepts a std::chrono::duration of the same type
	TimeStep& operator=( const T& duration )
	{
		m_duration = duration;
		return *this;
	}

	/**
     * @brief Assigns a new duration to the current object (different type).
     * 
     * Performs an internal duration cast to match the type stored by TimeStep.
     * 
     * @tparam DurationType - Duration type which may differ from the underlying type T.
     * @param duration - The new duration value to assign.
     * @return TimeStep& - Reference to the current object.
     */
	template < typename DurationType >
	TimeStep& operator=( const DurationType& duration )
	{
		m_duration = std::chrono::duration_cast< T >( duration );
		return *this;
	}

	/// Returns the sum of this TimeStep's duration and another's
	TimeStep operator+( const TimeStep& other ) const { return TimeStep( m_duration + other.m_duration ); }

	/// Adds another TimeStep's duration to this TimeStep
	TimeStep& operator+=( const TimeStep& other )
	{
		m_duration += other.m_duration;
		return *this;
	}

	/// Returns the difference between this TimeStep's duration and another's
	TimeStep operator-( const TimeStep& other ) const { return TimeStep( m_duration - other.m_duration ); }

	/// Subtracts another TimeStep's duration from this TimeStep
	TimeStep& operator-=( const TimeStep& other )
	{
		m_duration -= other.m_duration;
		return *this;
	}

	/// Checks equality of durations between this and another TimeStep
	[[nodiscard]] bool operator==( const TimeStep& other ) const { return m_duration == other.m_duration; }

	/// Checks inequality of durations between this and another TimeStep
	[[nodiscard]] bool operator!=( const TimeStep& other ) const { return !( *this == other ); }

private:
	T m_duration;
};

} // namespace PBL::Utils
#endif // I2C_UTILS_TIME_STEP_HPP__
