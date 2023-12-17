#ifndef PBL_MATH_PID_MODIFIERS_HPP__
#define PBL_MATH_PID_MODIFIERS_HPP__

// C++
#include <algorithm>
#include <type_traits>

namespace PBL::Math
{

// TODO: Add Min and Max

template < std::floating_point T >
struct Min
{
	T min;

	T operator()( T value ) const { return std::min( min, value ); }
};

template < std::floating_point T >
struct Max
{
	T max;

	T operator()( T value ) const { return std::max( max, value ); }
};

template < std::floating_point T >
struct Cap
{
	T lower;
	T upper;

	T operator()( T value ) const { return std::clamp( value, lower, upper ); }
};

struct Pow2
{
	template < std::floating_point T >
	T operator()( T value ) const
	{
		return value * value;
	}
};

template < std::floating_point T >
struct DeadZone
{
	T threshold;
	T operator()( T value ) const { return ( std::abs( value ) < threshold ) ? T{} : value; }
};

template < std::floating_point T >
struct Saturation
{
	T minVal;
	T maxVal;
	T operator()( T value ) const { return std::clamp( value, minVal, maxVal ); }
};

template < std::floating_point T >
struct IntegralWindupGuard
{
	T maxIntegral;
	T operator()( T integralComponent ) const { return std::clamp( integralComponent, -maxIntegral, maxIntegral ); }
};

template < std::floating_point T >
struct RateLimiter
{
	T lastValue{};
	T maxRate;

	T operator()( T value )
	{
		T limitedValue = std::clamp( value, lastValue - maxRate, lastValue + maxRate );
		lastValue = limitedValue;
		return limitedValue;
	}
};

template < std::floating_point T >
struct ExponentialScaling
{
	T exponent;
	T operator()( T value ) const { return std::pow( value, exponent ); }
};

// Introduces a small oscillation or noise to the output,
// useful in overcoming static friction in some mechanical systems
template < std::floating_point T >
struct Sither
{
	T amplitude;
	T operator()( T value ) { return value + ( ( std::rand() % 2 == 0 ? 1.0f : -1.0f ) * amplitude ); }
};

} // namespace PBL::Math
#endif // PBL_MATH_PID_MODIFIERS_HPP__
