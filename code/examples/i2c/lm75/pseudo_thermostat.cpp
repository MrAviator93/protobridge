
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// Output
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>
#include <format>
#include <concepts>
#include <iostream>
#include <string_view>

namespace
{

template < std::floating_point T >
struct cap
{
	T lower{};
	T upper{};

	T operator()( T value ) const { return std::clamp( value, lower, upper ); }
};

struct sqr
{
	template < std::floating_point T >
	T operator()( T value ) const
	{
		return value * value;
	}
};

template < std::floating_point T >
struct dead_zone
{
	T threshold;
	T operator()( T value ) const { return ( std::abs( value ) < threshold ) ? T{} : value; }
};

template < std::floating_point T >
struct saturation
{
	T minVal;
	T maxVal;
	T operator()( T value ) const { return std::clamp( value, minVal, maxVal ); }
};

template < std::floating_point T >
struct integral_windup_guard
{
	T maxIntegral;
	T operator()( T integralComponent ) const { return std::clamp( integralComponent, -maxIntegral, maxIntegral ); }
};

template < std::floating_point T >
struct rate_limiter
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
struct exponential_scaling
{
	T exponent;
	T operator()( T value ) const { return std::pow( value, exponent ); }
};

// Introduces a small oscillation or noise to the output,
// useful in overcoming static friction in some mechanical systems
template < std::floating_point T >
struct dither
{
	T amplitude;
	T operator()( T value ) { return value + ( ( std::rand() % 2 == 0 ? 1.0f : -1.0f ) * amplitude ); }
};

class PIDController
{
public:
	[[nodiscard]] PIDController& update( float reading ) noexcept
	{
		// For example purposes
		lastOutput = reading;
		return *this;
	}

	template < typename Functor >
	PIDController& operator|( Functor func )
	{
		lastOutput = func( lastOutput );
		return *this;
	}

	operator float() const { return lastOutput; }

private:
	float lastOutput{};
};

class ThermostatController
{
public:
	std::expected< void, std::string > adjust( float value )
	{
		std::cout << "Adjust " << value << std::endl;
		return {};
	}
};

} // namespace

int main( const int argc, const char* const* const argv )
{
	std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );

	// Default name of i2c bus on RPI 4
	std::string deviceName{ "/dev/i2c-1" };

	if( args.size() >= 2 )
	{
		deviceName = args[ 1 ];
	}

	// Create a bus controller for the I2C bus
	PBL::I2C::BusController busController{ deviceName };

	// Check if the I2C bus is open and accessible
	if( !busController.isOpen() )
	{
		std::cerr << "Failed to open I2C device" << std::endl;
		return 1;
	}

	// Create an LM75 controller, attached to the bus controller
	PBL::I2C::LM75Controller lm75{ busController };

	PIDController pidController;
	ThermostatController thermostat;

	// This requires all controllers to implement std::expected<bool, std::string> isActive()
	// if(auto check = CheckActive{ lm75, pidController, thermostat }, !check )
	// {
	// 	std::cerr << check.error() << std::endl;
	// 	return 1;
	// }

	// The CheckActive object could be even more complicated
	// template < typename... Args >
	// struct CheckActive
	// {
	// 	CheckActive( Args&... args )
	// 		: m_objects( std::ref( args )... )
	// 	{ }

	// 	// Something like this:
	// 	// bool check() const
	// 	// {
	// 	// 	// Implement the logic to check each object.
	// 	// 	// This is just a placeholder for demonstration.
	// 	// 	return std::apply( []( auto&&... args ) { return ( ... && args.get().isActive() ); }, m_objects );
	// 	// }

	// 	std::tuple< std::reference_wrapper< Args >... > m_objects;
	// };

	while( true )
	{
		auto rslt = lm75.getTemperatureC()
						.and_then( [ &pidController ]( float temp ) {
							std::cout << std::format( "Temperature: {}°C", temp ) << std::endl;
							float controlSignalInput = pidController.update( temp ) | cap{ 0.0f, 10.0f } | sqr{};
							return std::expected< float, std::string >{ controlSignalInput };
						} )
						.and_then( [ &thermostat ]( float controlSignalInput ) {
							return thermostat.adjust( controlSignalInput );
						} )
						.or_else( []( const std::string& error ) {
							// Do something with error?

							// Propagate the error
							return std::expected< void, std::string >{ std::unexpect, error };
						} );

		if( !rslt )
		{
			std::cerr << rslt.error() << std::endl;
			return 1;
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	return 0;
}

// class Thermostat
// {
// public:
// 	std::expected< void, std::string > update( float dt )
// 	{
// 		auto rslt = lm75.getTemperatureC()
// 						.and_then( [ &pidController ]( float temp ) {
// 							std::cout << std::format( "Temperature: {}°C", temp ) << std::endl;
// 							float controlSignalInput = pidController.update( temp ) | cap{ 0.0f, 10.0f } | sqr{};
// 							return std::expected< float, std::string >{ controlSignalInput };
// 						} )
// 						.and_then( [ &thermostat ]( float controlSignalInput ) {
// 							return thermostat.adjust( controlSignalInput );
// 						} )
// 						.or_else( []( const std::string& error ) {
// 							// Do something with error?

// 							// Propagate the error
// 							return std::expected< void, std::string >{ std::unexpect, error };
// 						} );

// 		if( !rslt )
// 		{
// 			return std::unexpected{ rslt.error() };
// 		}

// 		return {};
// 	}

// private:
// 	PBL::I2C::LM75Controller lm75;
// 	PIDController pidController;
// 	ThermostatController thermostat;
// };
