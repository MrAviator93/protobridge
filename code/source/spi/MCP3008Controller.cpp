#include "MCP23017Controller.hpp"

namespace pbl::spi
{

auto v1::MCP3008Controller::open( const std::string& devPath ) -> Result< MCP3008Controller >
{
	auto result = Device::open( devPath, Mode::MODE_0, Speed::SPEED_1MHZ, BitsPerWord::BITS_8 );
	if( !result ) return result.error();
	return MCP3008Controller( std::move( result.value() ) );
}

auto v1::MCP3008Controller::readVoltage( Channel channel, float vref ) -> Result< float >
{
	// Build command to send to MCP3008
	std::uint8_t startBit = 0x01;
	std::uint8_t commandBits =
		static_cast< std::uint8_t >( ( 0x08 | ( static_cast< std::uint8_t >( channel ) & 0x07 ) ) << 4 );
	std::array< std::uint8_t, 3 > tx = { startBit, commandBits, 0x00 };
	std::array< std::uint8_t, 3 > rx = {};

	const auto result = transfer( tx, rx );
	if( !result )
	{
		return result.error();
	}

	// Parse 10-bit ADC result from response
	const int adcValue = ( ( rx[ 1 ] & 0x03 ) << 8 ) | rx[ 2 ];
	const float voltage = static_cast< float >( adcValue ) * vref / 1023.0f;

	return voltage;
}

auto v1::MCP3008Controller::readDifferential( DiffDirection pair, float vref ) -> Result< float >
{
	std::uint8_t startBit = 0x01;

	// Differential: no single-ended bit set
	std::uint8_t commandBits = static_cast< std::uint8_t >( std::static_cast< uint8_t >( pair ) << 4 );

	std::array< std::uint8_t, 3 > tx = { startBit, commandBits, 0x00 };
	std::array< std::uint8_t, 3 > rx = {};

	const auto result = transfer( tx, rx );
	if( !result )
	{
		return result.error();
	}

	const int adcValue = ( ( rx[ 1 ] & 0x03 ) << 8 ) | rx[ 2 ];
	return static_cast< float >( adcValue ) * vref / 1023.0f;
}

auto v1::MCP3008Controller::readAllVoltages( float vref ) -> Result< std::array< float, 8 > >
{
	std::array< float, 8 > voltages{};

	for( auto i{ 0u }; i < 8u; ++i )
	{
		auto result = readVoltage( static_cast< Channel >( i ), vref );
		if( !result ) return result.error();
		voltages[ i ] = result.value();
	}

	return voltages;
}

} // namespace pbl::spi