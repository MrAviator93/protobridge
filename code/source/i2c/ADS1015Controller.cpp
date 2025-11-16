#include "ADS1015Controller.hpp"
#include "BusController.hpp"

// C++
#include <array>
#include <chrono>

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
constexpr std::uint16_t kCompQueMask = 0x0003; // Comparator queue bits mask
constexpr std::uint16_t kCompQueDisable = 0x0003; // Disable comparator
constexpr int kMaxConversionPolls = 32;
constexpr auto kPollInterval = std::chrono::microseconds( 500 );

} // namespace

v1::ADS1015Controller::ADS1015Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

auto v1::ADS1015Controller::setGain( Gain gain ) -> Result< void >
{
	if( m_activeMode.has_value() )
	{
		return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION,
								 "Gain can be changed only through the active ADS1015 session" );
	}

	return applyGainSetting( gain );
}

auto v1::ADS1015Controller::setSampleRate( SampleRate rate ) -> Result< void >
{
	if( m_activeMode.has_value() )
	{
		return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION,
								 "Sample rate can be changed only through the active ADS1015 session" );
	}

	return applySampleRateSetting( rate );
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

auto v1::ADS1015Controller::applyGainSetting( Gain gain ) -> Result< void >
{
	return readConfig()
		.transform( [ gain ]( std::uint16_t config ) -> std::uint16_t {
			config &= ~kGainMask;
			config |= static_cast< std::uint16_t >( gain );
			return config;
		} )
		.and_then( [ this ]( std::uint16_t config ) { return writeConfig( config ); } );
}

auto v1::ADS1015Controller::applySampleRateSetting( SampleRate rate ) -> Result< void >
{
	return readConfig()
		.transform( [ rate ]( std::uint16_t config ) -> std::uint16_t {
			config &= ~kDataRateMask;
			config |= static_cast< std::uint16_t >( rate );
			return config;
		} )
		.and_then( [ this ]( std::uint16_t config ) { return writeConfig( config ); } );
}

v1::ADS1015Controller::ModeSession::ModeSession( ADS1015Controller& controller, Mode mode ) noexcept
	: m_pController{ &controller }
	, m_mode{ mode }
{ }

v1::ADS1015Controller::ModeSession::ModeSession( ModeSession&& other ) noexcept
	: m_pController{ std::exchange( other.m_pController, nullptr ) }
	, m_mode{ other.m_mode }
{ }

auto v1::ADS1015Controller::ModeSession::operator=( ModeSession&& other ) noexcept -> ModeSession&
{
	if( this != &other )
	{
		release();
		m_pController = std::exchange( other.m_pController, nullptr );
		m_mode = other.m_mode;
	}
	return *this;
}

v1::ADS1015Controller::ModeSession::~ModeSession()
{
	release();
}

void v1::ADS1015Controller::ModeSession::release()
{
	if( m_pController != nullptr )
	{
		m_pController->releaseMode( m_mode );
		m_pController = nullptr;
	}
}

v1::ADS1015Controller::SingleShotSession::SingleShotSession( ADS1015Controller& controller ) noexcept
	: ModeSession( controller, Mode::SINGLE_SHOT )
{ }

auto v1::ADS1015Controller::SingleShotSession::read( Channel channel ) -> Result< std::int16_t >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->performSingleShot( channel );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Single-shot session is not active" );
}

auto v1::ADS1015Controller::SingleShotSession::setGain( Gain gain ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->applyGainSetting( gain );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Single-shot session is not active" );
}

auto v1::ADS1015Controller::SingleShotSession::setSampleRate( SampleRate rate ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->applySampleRateSetting( rate );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Single-shot session is not active" );
}

v1::ADS1015Controller::ContinuousSession::ContinuousSession( ADS1015Controller& controller, Channel channel ) noexcept
	: ModeSession( controller, Mode::CONTINUOUS )
	, m_channel{ channel }
{ }

auto v1::ADS1015Controller::ContinuousSession::read() -> Result< std::int16_t >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->readContinuousValue();
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Continuous session is not active" );
}

auto v1::ADS1015Controller::ContinuousSession::setGain( Gain gain ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		auto gainResult = ctrl->applyGainSetting( gain );
		if( !gainResult )
		{
			return gainResult;
		}

		return ctrl->configureContinuous( m_channel );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Continuous session is not active" );
}

auto v1::ADS1015Controller::ContinuousSession::setSampleRate( SampleRate rate ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		auto rateResult = ctrl->applySampleRateSetting( rate );
		if( !rateResult )
		{
			return rateResult;
		}

		return ctrl->configureContinuous( m_channel );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Continuous session is not active" );
}

v1::ADS1015Controller::DifferentialSession::DifferentialSession( ADS1015Controller& controller,
																 Channel positive,
																 Channel negative ) noexcept
	: ModeSession( controller, Mode::SINGLE_SHOT )
	, m_positive{ positive }
	, m_negative{ negative }
{ }

auto v1::ADS1015Controller::DifferentialSession::read() -> Result< std::int16_t >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->performDifferentialSingleShot( m_positive, m_negative );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Differential session is not active" );
}

auto v1::ADS1015Controller::DifferentialSession::setChannels( Channel positive, Channel negative ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		auto mux = differentialMux( positive, negative );
		if( !mux )
		{
			return utils::MakeError( mux.error() );
		}

		m_positive = positive;
		m_negative = negative;
		return utils::MakeSuccess();
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Differential session is not active" );
}

auto v1::ADS1015Controller::DifferentialSession::setGain( Gain gain ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->applyGainSetting( gain );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Differential session is not active" );
}

auto v1::ADS1015Controller::DifferentialSession::setSampleRate( SampleRate rate ) -> Result< void >
{
	if( auto* ctrl = controller(); ctrl != nullptr )
	{
		return ctrl->applySampleRateSetting( rate );
	}

	return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION, "Differential session is not active" );
}

auto v1::ADS1015Controller::startSingleShot() -> Result< SingleShotSession >
{
	if( auto acquired = acquireMode( Mode::SINGLE_SHOT ); !acquired )
	{
		return utils::MakeError( acquired.error() );
	}

	return utils::MakeSuccess( SingleShotSession{ *this } );
}

auto v1::ADS1015Controller::startContinuous( Channel channel ) -> Result< ContinuousSession >
{
	if( auto acquired = acquireMode( Mode::CONTINUOUS ); !acquired )
	{
		return utils::MakeError( acquired.error() );
	}

	if( auto configured = configureContinuous( channel ); !configured )
	{
		releaseMode( Mode::CONTINUOUS );
		return utils::MakeError( configured.error() );
	}

	return utils::MakeSuccess( ContinuousSession{ *this, channel } );
}

auto v1::ADS1015Controller::startDifferential( Channel positive, Channel negative ) -> Result< DifferentialSession >
{
	if( auto acquired = acquireMode( Mode::SINGLE_SHOT ); !acquired )
	{
		return utils::MakeError( acquired.error() );
	}

	if( auto mux = differentialMux( positive, negative ); !mux )
	{
		releaseMode( Mode::SINGLE_SHOT );
		return utils::MakeError( mux.error() );
	}

	return utils::MakeSuccess( DifferentialSession{ *this, positive, negative } );
}

auto v1::ADS1015Controller::performSingleShot( Channel channel ) -> Result< std::int16_t >
{
	return sampleSingleShot( singleEndedMux( channel ) );
}

auto v1::ADS1015Controller::performDifferentialSingleShot( Channel positive, Channel negative )
	-> Result< std::int16_t >
{
	auto mux = differentialMux( positive, negative );
	if( !mux )
	{
		return utils::MakeError( mux.error() );
	}

	return sampleSingleShot( *mux );
}

auto v1::ADS1015Controller::configureContinuous( Channel channel ) -> Result< void >
{
	return readConfig()
		.transform( [ channel ]( std::uint16_t config ) {
			config &= ~( kMuxMask | kModeMask | kCompQueMask );
			config |= singleEndedMux( channel );
			config |= static_cast< std::uint16_t >( Mode::CONTINUOUS );
			config |= kCompQueDisable;
			return config;
		} )
		.and_then( [ this ]( std::uint16_t config ) { return writeConfig( config ); } );
}

auto v1::ADS1015Controller::readContinuousValue() -> Result< std::int16_t >
{
	if( auto wait = waitForConversionComplete(); !wait )
	{
		return utils::MakeError( wait.error() );
	}

	return readConversionValue();
}

auto v1::ADS1015Controller::sampleSingleShot( std::uint16_t muxBits ) -> Result< std::int16_t >
{
	auto configResult = readConfig();
	if( !configResult )
	{
		return utils::MakeError( configResult.error() );
	}

	auto config = *configResult;
	config &= ~( kMuxMask | kModeMask | kCompQueMask );
	config |= muxBits;
	config |= static_cast< std::uint16_t >( Mode::SINGLE_SHOT );
	config |= kCompQueDisable;
	config |= kOsSingle;

	if( auto writeResult = writeConfig( config ); !writeResult )
	{
		return utils::MakeError( writeResult.error() );
	}

	if( auto wait = waitForConversionComplete(); !wait )
	{
		return utils::MakeError( wait.error() );
	}

	return readConversionValue();
}

auto v1::ADS1015Controller::waitForConversionComplete() -> Result< void >
{
	for( int attempt = 0; attempt < kMaxConversionPolls; ++attempt )
	{
		auto configResult = readConfig();
		if( !configResult )
		{
			return utils::MakeError( configResult.error() );
		}

		if( ( *configResult & kOsSingle ) != 0 )
		{
			return utils::MakeSuccess();
		}

		sleep( kPollInterval );
	}

	return utils::MakeError( utils::ErrorCode::TIMEOUT, "ADS1015 conversion timed out" );
}

auto v1::ADS1015Controller::readConversionValue() -> Result< std::int16_t >
{
	std::array< std::uint8_t, 2 > data{};
	if( read( kPointerConversion, data.data(), data.size() ) != 2 )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const std::int16_t raw = static_cast< std::int16_t >( ( data[ 0 ] << 8 ) | data[ 1 ] );
	return raw >> 4;
}

auto v1::ADS1015Controller::differentialMux( Channel positive, Channel negative ) -> Result< std::uint16_t >
{
	using enum Channel;

	if( positive == CH0 && negative == CH1 )
	{
		return utils::MakeSuccess( static_cast< std::uint16_t >( 0x0000 ) );
	}
	if( positive == CH0 && negative == CH3 )
	{
		return utils::MakeSuccess( static_cast< std::uint16_t >( 0x1000 ) );
	}
	if( positive == CH1 && negative == CH3 )
	{
		return utils::MakeSuccess( static_cast< std::uint16_t >( 0x2000 ) );
	}
	if( positive == CH2 && negative == CH3 )
	{
		return utils::MakeSuccess( static_cast< std::uint16_t >( 0x3000 ) );
	}

	return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, "Unsupported differential channel pair" );
}

auto v1::ADS1015Controller::singleEndedMux( Channel channel ) -> std::uint16_t
{
	const auto offset = static_cast< std::uint16_t >( channel ) << 12U;
	return static_cast< std::uint16_t >( 0x4000U + offset );
}

auto v1::ADS1015Controller::acquireMode( Mode mode ) -> Result< void >
{
	if( m_activeMode.has_value() )
	{
		return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION,
								 "Another ADS1015 session is currently active" );
	}

	m_activeMode = mode;
	return utils::MakeSuccess();
}

void v1::ADS1015Controller::releaseMode( Mode mode ) noexcept
{
	if( m_activeMode.has_value() && m_activeMode == mode )
	{
		m_activeMode.reset();
	}
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
