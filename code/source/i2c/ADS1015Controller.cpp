#include "ADS1015Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

namespace
{

// ADS1015 Registers
constexpr std::uint8_t POINTER_CONVERSION = 0x00;
constexpr std::uint8_t POINTER_CONFIG = 0x01;

// ADS1015 Configuration Register Masks
constexpr std::uint16_t OS_SINGLE = 0x8000; // Single-shot start conversion
constexpr std::uint16_t MUX_MASK = 0x7000; // MUX Mask
constexpr std::uint16_t GAIN_MASK = 0x0E00; // Gain Mask
constexpr std::uint16_t MODE_MASK = 0x0100; // Mode Mask
constexpr std::uint16_t DATA_RATE = 0x0080; // Data rate default setting (1600 SPS)
constexpr std::uint16_t COMP_QUE_DISABLE = 0x0003; // Disable comparator

// Configuration Register Template (default)
constexpr std::uint16_t kDefaultconfigRegister = OS_SINGLE | DATA_RATE | COMP_QUE_DISABLE;

} // namespace

ADS1015Controller::ADS1015Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

// // Helper to configure channel for single-ended reading
// void configureChannel( Channel channel )
// {
// 	configRegister_ = ( configRegister_ & ~MUX_MASK ) | ( static_cast< std::uint16_t >( channel ) << 12 );
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

// 	configRegister_ = ( configRegister_ & ~MUX_MASK ) | mux;
// 	writeConfig();
// }

// // Helper to initiate conversion (Single-shot mode)
// void initiateConversion()
// {
// 	configRegister_ |= OS_SINGLE;
// 	writeConfig();
// }

// // Helper to read the conversion result
// std::int16_t readConversionResult()
// {
// 	std::uint8_t rawData[ 2 ];
// 	if( read( POINTER_CONVERSION, rawData, 2 ) != 2 )
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
// 	if( !write( POINTER_CONFIG, configData, 2 ) )
// 	{
// 		throw std::runtime_error( "Failed to write config register" );
// 	}
// }

} // namespace pbl::i2c