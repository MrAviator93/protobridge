#include "ADS1015Controller.hpp"
#include "BusController.hpp"

// C++
#include <array>

namespace pbl::i2c
{

namespace
{

// ADS1015 Registers
constexpr std::uint8_t kPointerConversion = 0x00;
constexpr std::uint8_t kPointerConfig = 0x01;

// ADS1015 Configuration Register Masks
constexpr std::uint16_t kOsSingle = 0x8000; // Single-shot start conversion
constexpr std::uint16_t kMuxMask = 0x7000; // MUX Mask
constexpr std::uint16_t kGainMask = 0x0E00; // Gain Mask
constexpr std::uint16_t kModeMask = 0x0100; // Mode Mask
constexpr std::uint16_t kDataRateMask = 0x00E0; // Data rate bits mask (bits 7-5)
constexpr std::uint16_t kCompQueDisable = 0x0003; // Disable comparator

// Configuration Register Template (default)
constexpr std::uint16_t kDefaultconfigRegister = kOsSingle | kDataRateMask | kCompQueDisable;

} // namespace

v1::ADS1015Controller::ADS1015Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

auto v1::ADS1015Controller::setGain( Gain gain ) -> Result< void >
{
	return readConfig()
		.transform( [ gain ]( std::uint16_t config ) -> std::uint16_t {
			// Clear the gain bits (11-9) and set the new gain
			config &= ~kGainMask;

			// Set gain bits based on the provided gain
			config |= static_cast< std::uint16_t >( gain );
			return config;
		} )
		.and_then( [ this ]( std::uint16_t config ) { return writeConfig( config ); } );
}

auto v1::ADS1015Controller::setSampleRate( SampleRate rate ) -> Result< void >
{
	return readConfig()
		.transform( [ rate ]( std::uint16_t config ) -> std::uint16_t {
			// Clear the sample rate bits (7-5) and set the new sample rate
			config &= ~kDataRateMask;

			// Set rate bits based on the provided rate
			config |= static_cast< std::uint16_t >( rate );
			return config;
		} )
		.and_then( [ this ]( std::uint16_t config ) { return writeConfig( config ); } );
}

auto v1::ADS1015Controller::readConfig() -> Result< std::uint16_t >
{
	std::array< std::uint8_t, 2 > data{};
	if( read( kPointerConfig, data.data(), 2 ) == 2 )
	{
		// Combine high and low bytes to form the 16-bit config value
		std::uint16_t config = ( data[ 0 ] << 8 ) | data[ 1 ];
		return config;
	}

	return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
}

auto v1::ADS1015Controller::writeConfig( std::uint16_t config ) -> Result< void >
{
	// Split the 16-bit config into two bytes for transmission (big-endian format)
	std::array< std::uint8_t, 2 > data = {
		static_cast< std::uint8_t >( config >> 8 ), // High byte
		static_cast< std::uint8_t >( config & 0xFF ) // Low byte
	};

	// Use the ICBase write method to send the configuration to the CONFIG register
	if( !write( kPointerConfig, data ) )
	{
		// If the write failed, return an error
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	// Update the internal configuration state if the write was successful
	// currentConfig = config;
	return utils::MakeSuccess();
}

// // Helper to configure channel for single-ended reading
// void configureChannel( Channel channel )
// {
// 	configRegister_ = ( configRegister_ & ~kMuxMask ) | ( static_cast< std::uint16_t >( channel ) << 12 );
// 	writeConfig();
// }

// // Helper to configure differential reading
// void configureDifferential( Channel positiveChannel, Channel negativeChannel )
// {
// 	std::uint16_t mux;
// 	if( positiveChannel == Channel::CH0 && negativeChannel == Channel::CH1 )
// 		mux = 0x0000;
// 	else if( positiveChannel == Channel::CH0 && negativeChannel == Channel::CH3 )
// 		mux = 0x1000;
// 	else if( positiveChannel == Channel::CH1 && negativeChannel == Channel::CH3 )
// 		mux = 0x2000;
// 	else if( positiveChannel == Channel::CH2 && negativeChannel == Channel::CH3 )
// 		mux = 0x3000;
// 	else
// 		throw std::invalid_argument( "Invalid differential channel pair" );

// 	configRegister_ = ( configRegister_ & ~kMuxMask ) | mux;
// 	writeConfig();
// }

// // Helper to initiate conversion (Single-shot mode)
// void initiateConversion()
// {
// 	configRegister_ |= kOsSingle;
// 	writeConfig();
// }

// // Helper to read the conversion result
// std::int16_t readConversionResult()
// {
// 	std::uint8_t rawData[ 2 ];
// 	if( read( kPointerConversion, rawData, 2 ) != 2 )
// 	{
// 		throw std::runtime_error( "Failed to read conversion result" );
// 	}
// 	return static_cast< std::int16_t >( ( rawData[ 0 ] << 8 ) | rawData[ 1 ] ) >> 4;
// }

// // Helper to write configuration register
// void writeConfig()
// {
// 	std::uint8_t configData[ 2 ] = { static_cast< std::uint8_t >( configRegister_ >> 8 ),
// 									 static_cast< std::uint8_t >( configRegister_ & 0xFF ) };
// 	if( !write( kPointerConfig, configData, 2 ) )
// 	{
// 		throw std::runtime_error( "Failed to write config register" );
// 	}
// }

} // namespace pbl::i2c