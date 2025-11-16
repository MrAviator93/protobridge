#ifndef PBL_I2C_ADS1015_CONTROLLER_HPP__
#define PBL_I2C_ADS1015_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <expected>
#include <optional>
#include <utility>

namespace pbl::i2c
{

inline namespace v1
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
public:
	class ModeSession;
	class SingleShotSession;
	class ContinuousSession;
	class DifferentialSession;

	template < typename T >
	using Result = utils::Result< T >;

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

	/// Initializes the controller for the specified ADS1015 address.
	explicit ADS1015Controller( class BusController& busController, Address address = H48 );

	/// Default move constructor.
	ADS1015Controller( ADS1015Controller&& ) = default;

	/// Default move assignment.
	ADS1015Controller& operator=( ADS1015Controller&& ) = default;

	/// Configures the programmable gain amplifier.
	[[nodiscard]] Result< void > setGain( Gain gain );

	/// Configures the conversion sample rate.
	[[nodiscard]] Result< void > setSampleRate( SampleRate rate );

	/// Starts a single-shot session, ensuring exclusive access to the controller.
	[[nodiscard]] Result< SingleShotSession > startSingleShot();

	/// Starts a continuous session on the provided channel.
	[[nodiscard]] Result< ContinuousSession > startContinuous( Channel channel );

	/// Starts a differential single-shot session for the provided channel pair.
	[[nodiscard]] Result< DifferentialSession > startDifferential( Channel positive, Channel negative );

private:
	ADS1015Controller( const ADS1015Controller& ) = delete;
	ADS1015Controller& operator=( const ADS1015Controller& ) = delete;

private:
	friend class ModeSession;
	friend class SingleShotSession;
	friend class ContinuousSession;
	friend class DifferentialSession;

	/// Reads the device configuration register.
	[[nodiscard]] Result< std::uint16_t > readConfig();

	/// Writes the device configuration register.
	[[nodiscard]] Result< void > writeConfig( std::uint16_t config );

	/// Applies a gain update without changing session ownership.
	[[nodiscard]] Result< void > applyGainSetting( Gain gain );

	/// Applies a sample-rate update without changing session ownership.
	[[nodiscard]] Result< void > applySampleRateSetting( SampleRate rate );

	/// Performs a single-ended conversion.
	[[nodiscard]] Result< std::int16_t > performSingleShot( Channel channel );

	/// Performs a differential conversion.
	[[nodiscard]] Result< std::int16_t > performDifferentialSingleShot( Channel positive, Channel negative );

	/// Configures the hardware for continuous sampling.
	[[nodiscard]] Result< void > configureContinuous( Channel channel );

	/// Retrieves the most recent continuous conversion.
	[[nodiscard]] Result< std::int16_t > readContinuousValue();

	/// Executes a synchronous single-shot conversion using the provided mux bits.
	[[nodiscard]] Result< std::int16_t > sampleSingleShot( std::uint16_t muxBits );

	/// Waits for the ADS1015 conversion-ready flag.
	[[nodiscard]] Result< void > waitForConversionComplete();

	/// Reads the conversion register and normalizes the 12-bit value.
	[[nodiscard]] Result< std::int16_t > readConversionValue();

	/// Computes mux bits for a differential pair.
	[[nodiscard]] static Result< std::uint16_t > differentialMux( Channel positive, Channel negative );

	/// Computes mux bits for a single-ended channel.
	[[nodiscard]] static std::uint16_t singleEndedMux( Channel channel );

	/// Guards access to mutually exclusive operating modes.
	[[nodiscard]] Result< void > acquireMode( Mode mode );

	/// Releases the current active mode if it matches.
	void releaseMode( Mode mode ) noexcept;

private:
	std::optional< Mode > m_activeMode;
};

/// Base RAII guard for ADS1015 mode-exclusive sessions.
class ADS1015Controller::ModeSession
{
public:
	/// Move-only guard that ensures exclusive ADS1015 access for a mode.
	ModeSession( const ModeSession& ) = delete;
	ModeSession& operator=( const ModeSession& ) = delete;

private:
	friend class ADS1015Controller;

	explicit ModeSession( ADS1015Controller& controller, Mode mode ) noexcept;
	ModeSession( ModeSession&& other ) noexcept;
	ModeSession& operator=( ModeSession&& other ) noexcept;
	~ModeSession();

protected:
	[[nodiscard]] ADS1015Controller* controller() noexcept { return m_pController; }
	[[nodiscard]] const ADS1015Controller* controller() const noexcept { return m_pController; }
	[[nodiscard]] bool isActive() const noexcept { return m_pController != nullptr; }

private:
	void release();

private:
	ADS1015Controller* m_pController;
	Mode m_mode;
};

/// RAII session for single-shot conversions.
class ADS1015Controller::SingleShotSession final : public ModeSession
{
public:
	SingleShotSession( SingleShotSession&& ) noexcept = default;
	SingleShotSession& operator=( SingleShotSession&& ) noexcept = default;

	/// Performs a single-ended conversion on the provided channel.
	[[nodiscard]] Result< std::int16_t > read( Channel channel );

	/// Updates the gain while keeping the session ownership.
	[[nodiscard]] Result< void > setGain( Gain gain );

	/// Updates the sample rate while keeping the session ownership.
	[[nodiscard]] Result< void > setSampleRate( SampleRate rate );

private:
	friend class ADS1015Controller;

	explicit SingleShotSession( ADS1015Controller& controller ) noexcept;
};

/// RAII session for continuous streaming conversions.
class ADS1015Controller::ContinuousSession final : public ModeSession
{
public:
	ContinuousSession( ContinuousSession&& ) noexcept = default;
	ContinuousSession& operator=( ContinuousSession&& ) noexcept = default;

	/// Reads the latest sample produced in continuous mode.
	[[nodiscard]] Result< std::int16_t > read();

	/// Updates the gain and reapplies the continuous configuration.
	[[nodiscard]] Result< void > setGain( Gain gain );

	/// Updates the sample rate and reapplies the continuous configuration.
	[[nodiscard]] Result< void > setSampleRate( SampleRate rate );

private:
	friend class ADS1015Controller;

	ContinuousSession( ADS1015Controller& controller, Channel channel ) noexcept;

private:
	[[maybe_unused]] Channel m_channel;
};

/// RAII session for differential single-shot conversions.
class ADS1015Controller::DifferentialSession final : public ModeSession
{
public:
	DifferentialSession( DifferentialSession&& ) noexcept = default;
	DifferentialSession& operator=( DifferentialSession&& ) noexcept = default;

	/// Performs a differential conversion with the currently selected pair.
	[[nodiscard]] Result< std::int16_t > read();

	/// Selects the differential channel pair for future reads.
	[[nodiscard]] Result< void > setChannels( Channel positive, Channel negative );

	/// Updates the gain while keeping the session ownership.
	[[nodiscard]] Result< void > setGain( Gain gain );

	/// Updates the sample rate while keeping the session ownership.
	[[nodiscard]] Result< void > setSampleRate( SampleRate rate );

private:
	friend class ADS1015Controller;

	DifferentialSession( ADS1015Controller& controller, Channel positive, Channel negative ) noexcept;

private:
	Channel m_positive;
	Channel m_negative;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_ADS1015_CONTROLLER_HPP__
