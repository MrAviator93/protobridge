#ifndef PBL_I2C_ADS1015_CONTROLLER_HPP__
#define PBL_I2C_ADS1015_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <expected>

namespace pbl::i2c
{

/**
 * @brief Controller for the ADS1015 12-bit ADC with I2C interface.
 * 
 * The ADS1015 is a 12-bit ADC with an I2C interface, providing four single-
 * ended or two differential inputs. It supports different gain settings,
 * allowing flexibility in input voltage ranges, and can operate in single-shot
 * or continuous mode.
 */
class ADS1015Controller final : public ICBase, public utils::Counter< ADS1015Controller >
{
	struct ReaderPrivateTag
	{ };

public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

	using Success = Result< void >;

	enum class Address : std::uint8_t
	{
		H48 = 0x48, // ADDR connected to GND
		H49 = 0x49, // ADDR connected to VDD
		H4A = 0x4A, // ADDR connected to SDA
		H4B = 0x4B // ADDR connected to SCL
	};

	/// Input Channels for single-ended reading
	enum class Channel : std::uint8_t
	{
		CH0 = 0,
		CH1 = 1,
		CH2 = 2,
		CH3 = 3
	};

	/// Programmable Gain Amplifier (PGA) settings
	enum class Gain : std::uint16_t
	{
		FS_6_144V = 0x0000, // +/-6.144V range
		FS_4_096V = 0x0200, // +/-4.096V range
		FS_2_048V = 0x0400, // +/-2.048V range (default)
		FS_1_024V = 0x0600, // +/-1.024V range
		FS_0_512V = 0x0800, // +/-0.512V range
		FS_0_256V = 0x0A00 // +/-0.256V range
	};

	/// Data Rate (Sample Rate) Options
	enum class SampleRate : std::uint16_t
	{
		SPS_128 = 0x0000, // 128 samples per second
		SPS_250 = 0x0020, // 250 samples per second
		SPS_490 = 0x0040, // 490 samples per second
		SPS_920 = 0x0060, // 920 samples per second
		SPS_1600 = 0x0080, // 1600 samples per second (default)
		SPS_2400 = 0x00A0, // 2400 samples per second
		SPS_3300 = 0x00C0 // 3300 samples per second
	};

	/// Operating Mode (Single-shot or Continuous)
	enum class Mode : std::uint16_t
	{
		CONTINUOUS = 0x0000,
		SINGLE_SHOT = 0x0100
	};

	using enum Address;

	class Reader
	{
	public:
		enum class Mode
		{
			SINGLE_ENDED,
			DIFFERENTIAL,
			CONTINUOUS
		};

		Reader( ADS1015Controller& ads, ReaderPrivateTag )
			: m_ads{ ads }
		{
			// ads.lock();
		}

		~Reader()
		{
			// ads.unlock();
		}

		std::expected< std::int16_t, std::string > read()
		{
			// return ads.read();
			return 0;
		}

	private:
		ADS1015Controller& m_ads;
	};

	explicit ADS1015Controller( class BusController& busController, Address address = H48 );

	/// TBW
	Result< void > setGain( Gain gain );

	/// TBW
	Result< void > setSampleRate( SampleRate rate );

	// We need to think how are we going to pass here a channel, and second channel for differential read ...
	// [[nodiscard]] Reader makeReader(Reader::Mode mode) { return Reader{ *this, ReaderPrivateTag{} }; }

	// Read a single-ended input
	// std::int16_t readSingleEnded( Channel channel )
	// {
	// 	// Configure only if the channel has changed
	// 	if( channel != currentChannel_ )
	// 	{
	// 		configureChannel( channel );
	// 		currentChannel_ = channel; // Update the cached current channel
	// 	}
	// 	initiateConversion();
	// 	return readConversionResult();
	// }

	// // Read differential between channels
	// std::int16_t readDifferential( Channel positiveChannel, Channel negativeChannel )
	// {
	// 	configureDifferential( positiveChannel, negativeChannel );
	// 	initiateConversion();
	// 	return readConversionResult();
	// }

	// // Start continuous read on a specific channel
	// void startContinuousRead( Channel channel )
	// {
	// 	configureChannel( channel, Mode::CONTINUOUS ); // Configure for continuous mode
	// 	continuousMode_ = true; // Flag to indicate continuous mode is active
	// }

	// // Retrieve the last conversion result in continuous mode
	// std::int16_t readContinuous()
	// {
	// 	if( !continuousMode_ )
	// 	{
	// 		throw std::runtime_error( "Continuous read mode not initialized. Call startContinuousRead() first." );
	// 	}
	// 	return readConversionResult();
	// }

	// // Stop continuous read and reset to single-shot mode
	// void stopContinuousRead()
	// {
	// 	continuousMode_ = false;
	// 	configureChannel( currentChannel_, Mode::SINGLE_SHOT ); // Reset to single-shot mode
	// }

private:
	Result< std::uint16_t > readConfig();

	Result< void > writeConfig( std::uint16_t config );

private:
	// TODO
};

// // Create and configure ADS1015
// ADS1015Controller adc(busController, ADS1015Controller::Address::H48);
// adc.setGain(ADS1015Controller::Gain::FS_2_048V)
//    .setSampleRate(ADS1015Controller::SampleRate::SPS_920)
//    .setMode(ADS1015Controller::Mode::SINGLE_SHOT);

// // Perform a single-ended read on channel 0
// std::int16_t singleResult = adc.readSingleEnded(ADS1015Controller::Channel::CH0);
// std::cout << "Single-ended reading from CH0: " << singleResult << std::endl;

// // Perform a differential read between channel 0 and channel 1
// std::int16_t differentialResult = adc.readDifferential(ADS1015Controller::Channel::CH0, ADS1015Controller::Channel::CH1);
// std::cout << "Differential reading between CH0 and CH1: " << differentialResult << std::endl;

// // Start and read from continuous mode
// adc.beginContinuousRead(ADS1015Controller::Channel::CH2);
// for (int i = 0; i < 10; ++i) {
//     std::int16_t continuousResult = adc.readContinuous();
//     std::cout << "Continuous reading from CH2: " << continuousResult << std::endl;
// }
// adc.endContinuousRead();

} // namespace pbl::i2c
#endif // PBL_I2C_ADS1015_CONTROLLER_HPP__
