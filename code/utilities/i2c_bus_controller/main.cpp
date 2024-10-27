
// I2C
#include <i2c/Controllers.hpp>

// C++
#include <print>
#include <vector>
#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

int main( const int argc, const char* const* const argv )
{
	std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );

	std::string deviceName{ "/dev/i2c-1" };

	if( args.size() >= 2 )
	{
		deviceName = args[ 1 ];
	}

	pbl::i2c::BusController busController{ deviceName };
	pbl::i2c::LM75Controller lm75{ busController };

	if( const auto temp = lm75.getTemperatureC(); temp.has_value() )
	{
		std::println( "Temperature: {}°C", temp.value() );
	}

	// We will use MCP here to control LED's, configure ports A & B as output
	pbl::i2c::MCP23017Controller mcp23017{ busController, 0x00, 0x00 };

	mcp23017.setOnPortA( pbl::i2c::MCP23017Controller::Pins::PIN_8,
						 pbl::i2c::MCP23017Controller::Pins::PIN_7,
						 pbl::i2c::MCP23017Controller::Pins::PIN_6 );

	mcp23017.setOffPortA( pbl::i2c::MCP23017Controller::Pins::PIN_8 );

	pbl::i2c::BMP180Controller bmp180{ busController,
									   pbl::i2c::BMP180Controller::DEFAULT,
									   pbl::i2c::BMP180Controller::ULTRA_HIGH_RESOLUTION };

	if( const auto temp = bmp180.getTrueTemperatureC(); temp.has_value() )
	{
		std::println( "True temp: {} C", temp.value() );
	}

	if( const auto truePress = bmp180.getTruePressurePa(); truePress.has_value() )
	{
		std::println( "True pressure: {} Pa", truePress.value() );
	}

	if( const auto alt = bmp180.getAbsoluteAltitude(); alt.has_value() )
	{
		std::println( "Absolute altitude: {} m", alt.value() );
	}

	// pbl::i2c::BME680ControllerV2 bme680{ busController };

	// while( true )
	// {
	// 	bme680.printMeasurements();
	// 	busController.sleep( std::chrono::milliseconds( 100 ) );
	// }

	return 0;
}
