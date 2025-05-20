#include "MCP23017Controller.hpp"

namespace pbl::spi
{

// auto v1::MCP3008Controller::open( const std::string& devPath ) -> Result< MCP3008Controller >
// {
// 	auto result = Device::open( devPath, Mode::MODE_0, Speed::SPEED_1MHZ, BitsPerWord::BITS_8 );
// 	if( !result ) return result.error();
// 	return MCP3008Controller( std::move( result.value() ) );
// }

// auto v1::MCP3008Controller::readVoltage( Channel channel, float vref ) -> Result< float >
// {
// 	// Build command to send to MCP3008
// 	std::uint8_t startBit = 0x01;
// 	std::uint8_t commandBits =
// 		static_cast< std::uint8_t >( ( 0x08 | ( static_cast< std::uint8_t >( channel ) & 0x07 ) ) << 4 );
// 	std::array< std::uint8_t, 3 > tx = { startBit, commandBits, 0x00 };
// 	std::array< std::uint8_t, 3 > rx = {};

// 	auto result = transfer( tx, rx );
// 	if( !result )
// 	{
// 		return result.error();
// 	}

// 	// Parse 10-bit ADC result from response
// 	const int adcValue = ( ( rx[ 1 ] & 0x03 ) << 8 ) | rx[ 2 ];
// 	const float voltage = static_cast< float >( adcValue ) * vref / 1023.0f;

// 	return voltage;
// }

} // namespace pbl::spi