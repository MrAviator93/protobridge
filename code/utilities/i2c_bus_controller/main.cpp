
// I2C
#include <i2c/MCP23017Controller.hpp>
#include <i2c/BusController.hpp>
#include <i2c/BMP180Controller.hpp>
#include <i2c/LM75Controller.hpp>
#include <i2c/BME680Controller.hpp>
#include <i2c/BME680ControllerV2.hpp>

// C++
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

	PBL::I2C::BusController busController{ deviceName };
	PBL::I2C::LM75Controller lm75{ busController };

	if( const auto temp = lm75.getTemperatureC(); temp.has_value() )
	{
		std::cout << std::format( "Temperature: {}°C", temp.value() ) << std::endl;
	}

	// We will use MCP here to control LED's, configure ports A & B as output
	PBL::I2C::MCP23017Controller mcp23017{ busController, 0x00, 0x00 };

	mcp23017.setOnPortA( PBL::I2C::MCP23017Controller::Pins::PIN_8,
						 PBL::I2C::MCP23017Controller::Pins::PIN_7,
						 PBL::I2C::MCP23017Controller::Pins::PIN_6 );

	mcp23017.setOffPortA( PBL::I2C::MCP23017Controller::Pins::PIN_8 );

	PBL::I2C::BMP180Controller bmp180{ busController, PBL::I2C::BMP180Controller::ULTRA_HIGH_RESOLUTION };

	if( const auto temp = bmp180.getTrueTemperatureC(); temp.has_value() )
	{
		std::cout << std::format( "True temp: {} C", temp.value() ) << std::endl;
	}

	if( const auto truePress = bmp180.getTruePressurePa(); truePress.has_value() )
	{
		std::cout << std::format( "True pressure: {} Pa", truePress.value() ) << std::endl;
	}

	if( const auto alt = bmp180.getAbsoluteAltitude(); alt.has_value() )
	{
		std::cout << std::format( "Absolute altitude: {} m", alt.value() ) << std::endl;
	}

	PBL::I2C::BME680ControllerV2 bme680{ busController };

	while( true )
	{
		bme680.printMeasurements();
		busController.sleep( std::chrono::milliseconds( 100 ) );
	}

	return 0;
}
