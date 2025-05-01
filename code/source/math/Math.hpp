#ifndef PBL_MATH_MATH_HPP__
#define PBL_MATH_MATH_HPP__

#include <cmath>
#include <type_traits>

namespace pbl::math
{

constexpr double PI{ 3.14159265358979323846 };

/**
 * @brief Converts degrees to radians.
 * 
 * @tparam T Floating point type of the angle.
 * @param deg Angle in degrees.
 * @return Angle in radians.
 * 
 * @note Assumes the input is in degrees and returns the corresponding angle in radians.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T degreesToRadians( T degrees ) noexcept
{
	return ( degrees * PI / 180.0 );
}

/**
 * @brief Converts radians to degrees.
 * 
 * @tparam T Floating point type of the angle.
 * @param rad Angle in radians.
 * @return Angle in degrees.
 * 
 * @note Assumes the input is in radians and returns the corresponding angle in degrees.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T radiansToDegrees( T radians ) noexcept
{
	return ( radians * 180.0 / PI );
}

/**
 * @brief Computes the square of the sine of the input value.
 * 
 * This function calculates sin(value)^2.  
 * 
 * @note As of C++26, the std::sin function has not yet been made constexpr. 
 * Therefore, this function is not marked as constexpr either.
 * 
 * @tparam T The data type of the input, typically a floating-point type.
 * @param value The angle in radians for which to compute the squared sine.
 * @return T The squared sine value of the input.
 */
template < std::floating_point T >
[[nodiscard]] T sin2( T value )
{
	return std::pow( std::sin( value ), 2 );
}

/**
 * @brief Computes the square of the cosine of the input value.
 * 
 * This function calculates cos(value)^2.  
 * 
 * @note As of C++26, the std::cos function has not yet been made constexpr. 
 * Therefore, this function is not marked as constexpr either.
 * 
 * @tparam T The data type of the input, typically a floating-point type.
 * @param value The angle in radians for which to compute the squared cosine.
 * @return T The squared cosine value of the input.
 */
template < std::floating_point T >
[[nodiscard]] T cos2( T value )
{
	return std::pow( std::cos( value ), 2 );
}

/**
 * @brief Maps a value from one range to another.
 * 
 * This function takes an input value within a specified input range 
 * (`inMin` to `inMax`) and maps it proportionally to an output range 
 * (`outMin` to `outMax`).
 * 
 * @tparam T The data type of the input values and the output. Typically, this is a floating-point type.
 * 
 * @param value The input value to be mapped from the input range to the output range.
 * @param inMin The lower bound of the input range.
 * @param inMax The upper bound of the input range.
 * @param outMin The lower bound of the output range.
 * @param outMax The upper bound of the output range.
 * 
 * @return T The value mapped to the output range, maintaining the proportion 
 * relative to the input range.
 * 
 * @note Ensure that (inMax - inMin) is not zero to avoid division by zero errors.
 * 
 * @example 
 * To map the value 5 from an input range of [0, 10] to an output range of [0, 100]:
 * double result = map(5.0, 0.0, 10.0, 0.0, 100.0);
 * This would set `result` to 50.0.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T map( T value, T inMin, T inMax, T outMin, T outMax ) noexcept
{
	if( inMax == inMin )
	{
		// Return outMin if input range is zero
		return outMin;
	}

	return ( value - inMin ) * ( outMax - outMin ) / ( inMax - inMin ) + outMin;
}

/**
 * @brief Computes a linearly interpolated value between two points.
 * 
 * Given two points `(x1, y1)` and `(x2, y2)`, this function calculates the value `y` 
 * for a given `x` by linearly interpolating between the two points.
 * 
 * @tparam T The data type of the input values and the output. Typically, it's a numeric type.
 * 
 * @param y1  The y-coordinate of the first known point.
 * @param y2  The y-coordinate of the second known point.
 * @param x   The x-coordinate for which we want to compute the interpolated y value.
 * @param x1  The x-coordinate of the first known point.
 * @param x2  The x-coordinate of the second known point.
 * 
 * @return T The linearly interpolated y value corresponding to x.
 * 
 * @note Ensure that (x2 - x1) is not zero to avoid division by zero errors.
 * 
 * @example 
 * Given two points (1, 10) and (3, 30), to find the y value at x = 2:
 * double y = lerp(10.0, 30.0, 2.0, 1.0, 3.0);
 * This would set y to 20.0.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T lerp( T y1, T y2, T x, T x1, T x2 ) noexcept
{
	return ( ( y2 - y1 ) * ( x - x1 ) / ( x2 - x1 ) ) + y1;
}

/**
 * @brief Computes a cubic (Hermite) interpolated value between two points.
 * 
 * Given two points `(x1, y1)` and `(x2, y2)` along with their derivatives `dy1` and `dy2`,
 * this function calculates the value `y` for a given `x` by cubic interpolating between
 * the two points.
 * 
 * @tparam T The data type of the input values and the output. This should be a floating-point type.
 * 
 * @param y1  The y-coordinate of the first known point.
 * @param y2  The y-coordinate of the second known point.
 * @param dy1 The derivative of the first point.
 * @param dy2 The derivative of the second point.
 * @param x   The x-coordinate for which we want to compute the interpolated y value.
 * @param x1  The x-coordinate of the first known point.
 * @param x2  The x-coordinate of the second known point.
 * 
 * @return T The cubic interpolated y value corresponding to x.
 * 
 * @note Ensure that (x2 - x1) is not zero to avoid division by zero errors.
 * 
 * @example 
 * Given two points (1, 10) and (3, 30), and their derivatives at these points are 20 and 60, respectively.
 * To find the y value at x = 2:
 * double y = cubicInterpolate(10.0, 30.0, 20.0, 60.0, 2.0, 1.0, 3.0);
 * This would compute y using cubic interpolation.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T cubic( T y1, T y2, T dy1, T dy2, T x, T x1, T x2 ) noexcept
{
	// Calculate normalized parameter t
	T t = ( x - x1 ) / ( x2 - x1 );

	// Calculate the Hermite basis functions for t
	T h00 = ( 1.0 + 2.0 * t ) * ( 1.0 - t ) * ( 1.0 - t );
	T h10 = t * ( 1.0 - t ) * ( 1.0 - t );
	T h01 = t * t * ( 3.0 - 2.0 * t );
	T h11 = t * t * ( t - 1.0 );

	// Compute the interpolated y value
	return ( h00 * y1 ) + ( h10 * ( x2 - x1 ) * dy1 ) + ( h01 * y2 ) + ( h11 * ( x2 - x1 ) * dy2 );
}

/**
 * @brief Converts a temperature from Celsius to Fahrenheit.
 * 
 * @tparam T Numeric floating point type.
 * @param celsius Temperature in degrees Celsius.
 * @return The temperature in degrees Fahrenheit.
 * 
 * @note This function assumes the input is in degrees Celsius.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T celsiusToFahrenheit( const T celsius ) noexcept
{
	return ( celsius * T{ 1.8 } ) + T{ 32.0 };
}

/**
 * @brief Converts a temperature from Fahrenheit to Celsius.
 * 
 * @tparam T Numeric floating point type.
 * @param fahrenheit Temperature in degrees Fahrenheit.
 * @return The temperature in degrees Celsius.
 * 
 * @note This function assumes the input is in degrees Fahrenheit.
 */
template < std::floating_point T >
[[nodiscard]] constexpr T fahrenheitToCelsius( const T fahrenheit ) noexcept
{
	return ( fahrenheit - T{ 32.0 } ) / T{ 0.5556 };
}

} // namespace pbl::math
#endif // PBL_MATH_MATH_HPP__
