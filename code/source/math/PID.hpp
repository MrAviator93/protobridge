#ifndef PBL_MATH_PID_HPP__
#define PBL_MATH_PID_HPP__

#include "Math.hpp"
#include "Modifiers.hpp"

// C++
#include <utility>

namespace pbl::math
{

template < std::floating_point T >
class ControllerBase
{
public:
	using ValueType = T;
};

/**
 * @class PController
 * @brief Proportional controller template
 * 
 * @tparam T 
 */
template < std::floating_point T >
class PController : public ControllerBase< T >
{
public:
	using Desired = T;
	using Current = T;

	constexpr PController( T Kp ) noexcept
		: m_Kp{ Kp }
	{ }

	/// Calculates the output signal based on the proportional controller theory.
	[[nodiscard]] constexpr PController& update( const T desiredValue, const T currentValue ) noexcept
	{
		// Calculate error
		const T error = desiredValue - currentValue;

		// Calculate proportional term
		m_signal = m_Kp * error;

		return *this;
	}

	[[nodiscard]] constexpr PController& update( const std::pair< Desired, Current >& values ) noexcept
	{
		return update( values.first, values.second );
	}

	[[nodiscard]] constexpr PController& operator()( const T desiredValue, const T currentValue ) noexcept
	{
		return update( desiredValue, currentValue );
	}

	[[nodiscard]] constexpr PController& operator()( const std::pair< Desired, Current >& values ) noexcept
	{
		return update( values );
	}

	/// TBW
	template < typename Functor >
	[[nodiscard]] PController& operator|( Functor func )
	{
		m_signal = func( m_signal );
		return *this;
	}

	/// TBW
	[[nodiscard]] operator T() const { return m_signal; }

private:
	T m_Kp; //!< Proportional term gain
	T m_signal{}; //!< TBW
};

/**
 * @class PIController
 * @brief Combined of proportional and integral controller template.
 * 
 * @tparam T 
 */
template < std::floating_point T >
class PIController : public ControllerBase< T >
{
public:
	using Desired = T;
	using Current = T;

	constexpr PIController( T Kp, T Ki ) noexcept
		: m_Kp{ Kp }
		, m_Ki{ Ki }
	{ }

	/// Calculates the output signal based on the proportional and integral combined controller theory
	[[nodiscard]] constexpr PIController& update( const T dt, const T desiredValue, const T currentValue ) noexcept
	{
		// Calculate error
		const T error = desiredValue - currentValue;

		// Calculate proportional term
		const T proportionalTermOut = m_Kp * error;

		// Calculate integral term
		m_integral += error * dt;
		const T integralTermOut = m_Ki * m_integral;

		// Calculate total output
		m_signal = proportionalTermOut + integralTermOut;

		return *this;
	}

	[[nodiscard]] constexpr PIController& update( const T dt, const std::pair< Desired, Current >& values ) noexcept
	{
		return update( dt, values.first, values.second );
	}

	[[nodiscard]] constexpr PIController& operator()( const T dt, const T desiredValue, const T currentValue ) noexcept
	{
		return update( dt, desiredValue, currentValue );
	}

	[[nodiscard]] constexpr PIController& operator()( const T dt, const std::pair< Desired, Current >& values ) noexcept
	{
		return update( dt, values );
	}

	/// TBW
	template < typename Functor >
	[[nodiscard]] PIController& operator|( Functor func )
	{
		m_signal = func( m_signal );
		return *this;
	}

	/// TBW
	[[nodiscard]] operator T() const { return m_signal; }

private:
	T m_Kp; //!< Proportional controller gain
	T m_Ki; //!< Integral controller gain
	T m_integral{}; //!< TBW
	T m_signal{}; //!< TBW
};

/**
 * @brief Proportional, integral and derivative combined controller.
 * 
 * @note Where simpler controllers P or PI can be used, use the above defined templates
 * as they will be better performance wise, for micro controller.
 * 
 * @tparam T 
 */
template < std::floating_point T >
class PIDController : public ControllerBase< T >
{
public:
	using Desired = T;
	using Current = T;

	struct Input
	{
		Desired desired{};
		Current current{};
	};

	/**
	 * @brief Construct a new CPIDControllerT object
	 * 
	 * @note If it is only required to use proportional term, set the proportional 
	 * gain to the desired value, and the rest gains to zero. This will leave only
	 * proportional term in the PID controller.
	 * 
	 * @note If it is only required to use PI controller, then set the proportional 
	 * and integral terms to the desired values and set the derivative gain as zero.
	 * 
	 * @param dt 
	 * @param max 
	 * @param min 
	 * @param Kp 
	 * @param Kd 
	 * @param Ki 
	 */
	constexpr PIDController( T Kp, T Kd, T Ki ) noexcept
		: m_Kp{ Kp }
		, m_Kd{ Kd }
		, m_Ki{ Ki }
	{ }

	/// TBW
	[[nodiscard]] constexpr PIDController& update( const T dt, const T desiredValue, const T currentValue ) noexcept
	{
		// Calculate error
		const T error = desiredValue - currentValue;

		// Calculate proportional term
		const T proportionalTermOut = m_Kp * error;

		// Calculate integral term
		m_integral += error * dt;
		const T integralTermOut = m_Ki * m_integral;

		// Calculate derivative term
		const T derivative = ( error - m_previousError ) / dt;
		const T derivativeTermOut = m_Kd * derivative;

		// Calculate total output
		m_signal = proportionalTermOut + integralTermOut + derivativeTermOut;

		// Note down the current error
		m_previousError = error;

		return *this;
	}

	[[nodiscard]] constexpr PIDController& update( const T dt, const Input& input ) noexcept
	{
		return update( dt, input.desired, input.current );
	}

	[[nodiscard]] constexpr PIDController& operator()( const T dt, const T desiredValue, const T currentValue ) noexcept
	{
		return update( dt, desiredValue, currentValue );
	}

	[[nodiscard]] constexpr PIDController& operator()( const T dt, const Input& input ) noexcept
	{
		return update( dt, input );
	}

	/// TBW
	template < typename Functor >
	[[nodiscard]] PIDController& operator|( Functor func )
	{
		m_signal = func( m_signal );
		return *this;
	}

	/// TBW
	[[nodiscard]] operator T() const { return m_signal; }

private:
	T m_Kp; //!< Proportional controller gain
	T m_Kd; //!< Derivative controller gain
	T m_Ki; //!< Integral controller gain

	T m_previousError{}; //!< TBW
	T m_integral{}; //!< TBW
	T m_signal{}; //!< TBW
};

} // namespace pbl::math
#endif // PBL_MATH_PID_HPP__
