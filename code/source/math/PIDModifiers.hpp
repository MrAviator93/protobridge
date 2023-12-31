#ifndef PBL_MATH_PID_MODIFIERS_HPP__
#define PBL_MATH_PID_MODIFIERS_HPP__

// C++
#include <algorithm>
#include <type_traits>

namespace PBL::Math
{

template < std::floating_point T >
struct Min
{
	T min;

	[[nodiscard]] T operator()( T value ) const { return std::min( min, value ); }
};

template < std::floating_point T >
struct Max
{
	T max;

	[[nodiscard]] T operator()( T value ) const { return std::max( max, value ); }
};

template < std::floating_point T >
struct Cap
{
	T lower;
	T upper;

	[[nodiscard]] T operator()( T value ) const { return std::clamp( value, lower, upper ); }
};

struct Pow2
{
	template < std::floating_point T >
	[[nodiscard]] T operator()( T value ) const
	{
		return value * value;
	}
};

struct Pow3
{
	template < std::floating_point T >
	[[nodiscard]] T operator()( T value ) const
	{
		return value * value * value;
	}
};

struct Pow4
{
	template < std::floating_point T >
	[[nodiscard]] T operator()( T value ) const
	{
		return value * value * value * value;
	}
};

template < std::floating_point T >
struct DeadZone
{
	T threshold;

	[[nodiscard]] T operator()( T value ) const { return ( std::abs( value ) < threshold ) ? T{} : value; }
};

template < std::floating_point T >
struct Saturation
{
	T minVal;
	T maxVal;

	[[nodiscard]] T operator()( T value ) const { return std::clamp( value, minVal, maxVal ); }
};

template < std::floating_point T >
struct IntegralWindupGuard
{
	T maxIntegral;

	[[nodiscard]] T operator()( T integralComponent ) const
	{
		return std::clamp( integralComponent, -maxIntegral, maxIntegral );
	}
};

template < std::floating_point T >
struct RateLimiter
{
	T lastValue{};
	T maxRate;

	[[nodiscard]] T operator()( T value )
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

	[[nodiscard]] T operator()( T value ) const { return std::pow( value, exponent ); }
};

// Introduces a small oscillation or noise to the output,
// useful in overcoming static friction in some mechanical systems
template < std::floating_point T >
struct Sither
{
	T amplitude;

	[[nodiscard]] T operator()( T value ) { return value + ( ( std::rand() % 2 == 0 ? 1.0f : -1.0f ) * amplitude ); }
};

} // namespace PBL::Math
#endif // PBL_MATH_PID_MODIFIERS_HPP__
