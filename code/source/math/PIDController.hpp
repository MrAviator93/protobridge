#ifndef PBL_MATH_PID_CONTROLLERS_HPP__
#define PBL_MATH_PID_CONTROLLERS_HPP__

// C++
#include <utility>
#include <concepts>
#include <algorithm>
#include <type_traits>

namespace PBL::Math
{

template < std::floating_point T >
class ControllerBase
{
public:
	using BaseType = T;
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
	constexpr PController( T Kp ) noexcept
		: m_Kp{ Kp }
	{ }

	/**
	 * @brief Calculates the output signal based on the proportional
	 * controller theory.
	 * 
	 * @param desiredValue is the set-point
	 * @param currentValue 
	 * @return constexpr T 
	 */
	[[nodiscard]] constexpr T calculateControlSignal( T desiredValue, T currentValue ) const noexcept
	{
		// Calculate error
		T error = desiredValue - currentValue;

		// Calculate proportional term
		T proportionalTermOut = m_Kp * error;

		return proportionalTermOut;
	}

	[[nodiscard]] constexpr T operator()( T desiredValue, T currentValue ) const noexcept
	{
		return calculateControlSignal( desiredValue, currentValue );
	}

private:
	T m_Kp; //!< Proportional term gain
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
	constexpr PIController( T dt, T Kp, T Ki ) noexcept
		: m_dt{ dt }
		, m_Kp{ Kp }
		, m_Ki{ Ki }
		, m_integral{ static_cast< T >( 0.0 ) }
	{ }

	/**
	 * @brief Calculates the output signal based on the proportional
	 * and integral combined controller theory.
	 * 
	 * @param desiredValue is the set-point
	 * @param currentValue 
	 * @return T 
	 */
	[[nodiscard]] constexpr T calculateControlSignal( T desiredValue, T currentValue ) noexcept
	{
		// Calculate error
		T error = desiredValue - currentValue;

		// Calculate proportional term
		T proportionalTermOut = m_Kp * error;

		// Calculate integral term
		m_integral += error * m_dt;
		T integralTermOut = m_Ki * m_integral;

		// Calculate total output
		T controllerOutput = proportionalTermOut + integralTermOut;

		return controllerOutput;
	}

	[[nodiscard]] constexpr T operator()( T desiredValue, T currentValue ) noexcept
	{
		return calculateControlSignal( desiredValue, currentValue );
	}

	/// Update the integrals time-step, remember we are integrating over time.
	constexpr void updateTimeStep( T timeStep ) noexcept { m_dt = timeStep; }

private:
	T m_dt; //!< Loop interval time (regulation period)
	T m_Kp; //!< Proportional controller gain
	T m_Ki; //!< Integral controller gain
	T m_integral; //!< TBW
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
	constexpr PIDController( T dt, T max, T min, T Kp, T Kd, T Ki ) noexcept
		: m_dt{ dt }
		, m_max{ max }
		, m_min{ min }
		, m_Kp{ Kp }
		, m_Kd{ Kd }
		, m_Ki{ Ki }
		, m_integral{ static_cast< T >( 0.0 ) }
		, m_previousError{ static_cast< T >( 0.0 ) }
	{ }

	/**
	 * @brief 
	 * 
	 * @param desiredValue is the set-point
	 * @param currentValue 
	 * @return T 
	 */
	[[nodiscard]] constexpr T calculateControlSignal( T desiredValue, T currentValue ) noexcept
	{
		// Calculate error
		T error = desiredValue - currentValue;

		// Calculate proportional term
		T proportionalTermOut = m_Kp * error;

		// Calculate integral term
		m_integral += error * m_dt;
		T integralTermOut = m_Ki * m_integral;

		// ****** NOTE: One of the anti-windup techniques is to cap the integral terms to some limits
		// this->cap(integralTermOut, -400, 400); // also need to reset the previous error

		// Calculate derivative term
		T derivative = ( error - m_previousError ) / m_dt;
		T derivativeTermOut = m_Kd * derivative;

		// Calculate total output
		T controllerOutput = proportionalTermOut + integralTermOut + derivativeTermOut;

		// Apply limits
		cap( controllerOutput, m_min, m_max );

		// Note down the current error
		m_previousError = error;

		return controllerOutput;
	}

	[[nodiscard]] constexpr T operator()( T desiredValue, T currentValue ) noexcept
	{
		return calculateControlSignal( desiredValue, currentValue );
	}

	/// Update the integrals time-step, remember we are integrating over time.
	constexpr void updateTimeStep( T timeStep ) noexcept { m_dt = timeStep; }

	/// Cap clamp value between min and max values
	constexpr void cap( T& value, T min, T max ) noexcept { value = std::clamp( value, min, max ); }

private:
	T m_dt; //!< Loop interval time

	T m_max; //!< TBW, do we need them here ???
	T m_min; //!< TBW, do we need them here ???

	T m_Kp; //!< Proportional controller gain
	T m_Kd; //!< Derivative controller gain
	T m_Ki; //!< Integral controller gain

	T m_previousError; //!< TBW
	T m_integral; //!< TBW
};

} // namespace PBL::Math
#endif // PBL_MATH_PID_CONTROLLERS_HPP__
