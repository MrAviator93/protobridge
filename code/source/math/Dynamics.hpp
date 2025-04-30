#ifndef PBL_MATH_DYNAMICS_HPP__
#define PBL_MATH_DYNAMICS_HPP__

// C++
#include <math>
#include <type_traits>

namespace pbl::math
{

/**
 * @brief Convert atmospheric pressure to altitude (meters) using barometric formula.
 * @param pressurePa True pressure in pascals
 * @param  referencePressurePa Sea-level or ground-level pressure in pascals
 * @return Estimated altitude in meters
 */
template < typename T >
[[nodiscard]] inline constexpr T pressureToAltitude( T pressurePa, T referencePressurePa )
{
	static_assert( std::is_floating_point_v< T >, "pressureToAltitude requires a floating-point type" );

	constexpr T exponent = static_cast< T >( 1.0 ) / static_cast< T >( 5.255 );

	const T ratio = pressurePa / referencePressurePa;
	const T r = static_cast< T >( 1.0 ) - std::pow( ratio, exponent );
	return static_cast< T >( 44330.0 ) * r;
}

/**
 * @brief Calculate the dew point temperature (in Celsius) based on ambient temperature and relative humidity.
 * 
 * Uses the Magnus-Tetens approximation to estimate the dew point. Useful for weather applications,
 * 
 * HVAC control, and climate monitoring.
 * 
 * @tparam T Floating-point type (float, double, etc.)
 * @param temperatureC Ambient temperature in Celsius
 * @param humidityPercent Relative humidity (0–100%)
 * @return Dew point temperature in Celsius
 */
template < typename T >
[[nodiscard]] inline constexpr T dewPoint( T temperatureC, T humidityPercent )
{
	static_assert( std::is_floating_point_v< T >, "Requires floating-point" );

	constexpr T a = 17.62;
	constexpr T b = 243.12;
	const T gamma =
		( a * temperatureC ) / ( b + temperatureC ) + std::log( humidityPercent / static_cast< T >( 100.0 ) );
	return ( b * gamma ) / ( a - gamma );
}

/**
 * @brief Estimate the heat index ("feels like" temperature) in Celsius.
 * 
 * Converts Celsius to Fahrenheit internally, applies the Rothfusz regression
 * formula, then converts back to Celsius. Suitable for weather dashboards and
 * human comfort metrics.
 * 
 * @note Accurate only for temperatures > 27°C and RH > 40%.
 * 
 * @tparam T Floating-point type (float, double, etc.)
 * @param tempC Ambient temperature in Celsius
 * @param humidityPercent Relative humidity (0–100%)
 * @return Heat index in Celsius
 */
template < typename T >
[[nodiscard]] inline T heatIndexCelsius( T tempC, T humidityPercent )
{
	static_assert( std::is_floating_point_v< T >, "Requires floating-point" );
	// Convert to Fahrenheit for classic HI equation
	const T tempF = celsiusToFahrenheit( tempC );
	const T RH = humidityPercent;

	// Rothfusz regression
	T hi = -42.379f + 2.04901523f * tempF + 10.14333127f * RH - 0.22475541f * tempF * RH - 0.00683783f * tempF * tempF -
		   0.05481717f * RH * RH + 0.00122874f * tempF * tempF * RH + 0.00085282f * tempF * RH * RH -
		   0.00000199f * tempF * tempF * RH * RH;

	return ( hi - 32.0f ) * 5.0f / 9.0f; // Return in Celsius
}

} // namespace pbl::math
#endif // PBL_MATH_DYNAMICS_HPP__
