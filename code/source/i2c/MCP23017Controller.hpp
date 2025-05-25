#ifndef PBL_I2C_MCP23017_CONTROLLER_HPP__
#define PBL_I2C_MCP23017_CONTROLLER_HPP__

#include "ICBase.hpp"

#include <utils/Utils.hpp>
#include <utils/Counter.hpp>
#include <utils/PinConfig.hpp>
#include <utils/Overloaded.hpp>

// C++
#include <array>
#include <expected>
#include <functional>

namespace pbl::i2c
{

// TODO: Add here one more nested namespace detail::mcp23017::port
namespace detail::mcp23017::port
{

/// MCP23017 chip available pins on port A and B
enum class Pins : std::uint8_t
{
	PIN_1 = 0x01,
	PIN_2 = 0x02,
	PIN_3 = 0x04,
	PIN_4 = 0x08,
	PIN_5 = 0x10,
	PIN_6 = 0x20,
	PIN_7 = 0x40,
	PIN_8 = 0x80
};

enum class PinsIds : std::uint8_t
{
	PIN_1 = 1,
	PIN_2 = 2,
	PIN_3 = 3,
	PIN_4 = 4,
	PIN_5 = 5,
	PIN_6 = 6,
	PIN_7 = 7,
	PIN_8 = 8
};

/// MCP23017 chip pin states (LOW and HIGH)
enum class PinState : std::uint8_t
{
	LOW = 0,
	HIGH = 1
};

/// MCP23017 chip available pinmodes
enum class PinMode : std::uint8_t
{
	OUTPUT = 0,
	INPUT = 1
};

/// MCP23017 chip interrupt control options for INTCON register
enum class InterruptControl : std::uint8_t
{
	PREVIOUS = 0, // Interrupt on change from previous state
	COMPARE = 1 // Interrupt on comparison with DEFVAL
};

namespace calls
{

inline constexpr auto PinModeTo = [] [[nodiscard]] ( auto v ) noexcept -> bool { return v == PinMode::INPUT; };
inline constexpr auto PinModeFrom = [] [[nodiscard]] ( auto b ) noexcept -> PinMode {
	return b ? PinMode::INPUT : PinMode::OUTPUT;
};

inline constexpr auto PinStatesTo = [] [[nodiscard]] ( auto v ) noexcept -> bool { return v == PinState::HIGH; };
inline constexpr auto PinStatesFrom = [] [[nodiscard]] ( auto b ) noexcept -> PinState {
	return b ? PinState::HIGH : PinState::LOW;
};

inline constexpr auto InterruptControlTo = [] [[nodiscard]] ( auto v ) noexcept -> bool {
	return v == InterruptControl::COMPARE;
};
inline constexpr auto InterruptControlFrom = [] [[nodiscard]] ( auto b ) noexcept -> InterruptControl {
	return b ? InterruptControl::COMPARE : InterruptControl::PREVIOUS;
};
}; // namespace calls

/// PinModes represents IODIR register for pin direction configuration.
/// Each bit indicates whether the corresponding pin is set to INPUT (1) or OUTPUT (0).
using PinModes =
	utils::PinConfig< PinMode, PinMode::OUTPUT, decltype( calls::PinModeTo ), decltype( calls::PinModeFrom ) >;

/// PinStates represents the GPIO register, reflecting current pin states.
/// Each bit indicates the actual logic level (HIGH or LOW) of each pin.
using PinStates =
	utils::PinConfig< PinState, PinState::LOW, decltype( calls::PinStatesTo ), decltype( calls::PinStatesFrom ) >;

/// PinInterruptControl represents the INTCON register for interrupt conditions.
/// Each bit indicates whether a pin triggers an interrupt on comparison with DEFVAL (1) or change from previous state (0).
using PinInterruptControl = utils::PinConfig< InterruptControl,
											  InterruptControl::PREVIOUS,
											  decltype( calls::InterruptControlTo ),
											  decltype( calls::InterruptControlFrom ) >;

/// PinDefaultComparison represents DEFVAL register, setting default values for interrupt comparison.
/// Each bit represents the comparison value (0 or 1) for each pin.
using PinDefaultComparison = utils::PinConfig< bool, false >;

/// PinInterruptEnable represents GPINTEN register for enabling/disabling interrupts.
/// Each bit indicates whether an interrupt is enabled (1) or disabled (0) for each pin.
using PinInterruptEnable = utils::PinConfig< bool, false >;

/// PinInterrupts allows configuration of interrupts per pin.
/// This is equivalent to PinInterruptEnable but represents higher-level intent.
using PinInterrupts = utils::PinConfig< bool, false >;

/// PinPullUps represents GPPU register, configuring pull-up resistors.
/// Each bit determines whether the pull-up resistor is enabled (1) or disabled (0) for each pin.
using PinPullUps = utils::PinConfig< bool, false >;

// Read-Only Configurations

/// Captured pin states at interrupt time, reflecting INTCAP register.
/// This is a read-only configuration, storing the state of each pin at the time an interrupt was triggered.
using PinInterruptCapture =
	utils::PinConfig< PinState, PinState::LOW, decltype( calls::PinStatesTo ), decltype( calls::PinStatesFrom ) >;

/// Interrupt flags indicating which pins triggered an interrupt, reflecting INTF register.
/// This is a read-only configuration, showing which pins currently have an interrupt pending.
using PinInterruptFlags = utils::PinConfig< bool, false >;

} // namespace detail::mcp23017::port

/**
 * @class MCP23017Controller
 * @brief Manages interaction with the MCP23017 16-bit I/O expander.
 *
 * The MCP23017Controller serves as an interface for controlling the MCP23017, a 16-bit, general-purpose 
 * parallel I/O port expander for I2C bus communication. This class allows individual management of the 
 * 16 I/O pins on the MCP23017 device, enabling users to configure each as an input or output. 
 *
 * Key features of the MCP23017Controller include:
 *  - Configuring each of the 16 I/Os individually by setting the pin direction (input or output).
 *  - Writing to any combination of I/Os simultaneously, thereby eliminating the need to 
 *    continuously update individual pins.
 *  - Reading the current state of the I/O pins when they are configured as inputs.
 *
 * Additionally, this controller ensures a seamless interaction with the MCP23017 device by abstracting 
 * the I2C communication complexities associated with data writing and retrieval operations.
 *
 * Example usage:
 * @code
 * // TBW ...
 * @endcode
 *
 * @note The initial state of the pins after powering up the MCP23017 device is defined by its hardware 
 * characteristics. Users should ensure they configure the pins to a known state before performing operations.
 * 
 * @note Most likely the default values on power-on:
 * - IODIR registers (input/output direction): All pins are configured as inputs by default.
 * - GPIO registers: Undefined, because inputs and outputs will have no state initially until configured.
 * - GPPU registers (pull-up resistors): By default, the pull-up resistors are disabled for all pins.
 * 
 * @note MCP23017 output pin configuration:
 * 1. If pins are set as output after chip restart (power off - on)
 * the pins will be all on 0x00. To set them all of use i2cset -y 1 0x20 0x14 0xff
 *
 * @todo Finalize the implementation details of the class and perform comprehensive testing to ensure 
 * reliable operation under various scenarios.
 * 
 * @todo For ports either use std::bitset<8> or implement new structs,
 * i.e. struct Port{ Type type;, ... }
 * 
 * @author AK aka MrAviator93
 */
class MCP23017ControllerV2 final : public ICBase, public utils::Counter< MCP23017ControllerV2 >
{

	struct PortTag
	{ };

public:
	template < typename T >
	using Result = utils::Result< T >;

	class Port
	{
		struct PinTag
		{ };

		class ModeTag
		{ };

	public:
		using Pins = detail::mcp23017::port::Pins;
		using PinMode = detail::mcp23017::port::PinMode;
		using PinState = detail::mcp23017::port::PinState;

		using PinModes = detail::mcp23017::port::PinModes;
		using PinStates = detail::mcp23017::port::PinStates;
		using PinInterruptControl = detail::mcp23017::port::PinInterruptControl;
		using PinDefaultComparison = detail::mcp23017::port::PinDefaultComparison;
		using PinInterruptEnable = detail::mcp23017::port::PinInterruptEnable;
		using PinInterrupts = detail::mcp23017::port::PinInterrupts;
		using PinPullUps = detail::mcp23017::port::PinPullUps;
		using PinInterruptCapture = detail::mcp23017::port::PinInterruptCapture;
		using PinInterruptFlags = detail::mcp23017::port::PinInterruptFlags;

		/// MCP23017 chip port addresses
		enum class Address : std::uint8_t
		{
			PORT_A = 0x14,
			PORT_B = 0x15
		};

		Port( MCP23017ControllerV2& controller, Address address, PortTag )
			: m_controller{ controller }
			, m_address{ address }
		{ }

		template < typename Dispatcher >
		class Pin;

		/// Returns a specific pin
		[[nodiscard]] auto pin( Pins pin );

		/// Reads the direction (input/output) for all pins on the port (IODIR register).
		[[nodiscard]] Result< detail::mcp23017::port::PinModes > pinModes();

		/// Sets the direction (input/output) for all pins on the port (IODIR register).
		[[nodiscard]] Result< void > setPinModes( const detail::mcp23017::port::PinModes& modes );

		/// Reads the current logic state (HIGH/LOW) of all pins on the port (GPIO register).
		[[nodiscard]] Result< detail::mcp23017::port::PinStates > pinStates();

		/// Sets the logic state (HIGH/LOW) of all output pins on the port (GPIO register).
		[[nodiscard]] Result< void > setPinStates( const detail::mcp23017::port::PinStates& states );

		/// Reads the pull-up configuration (GPPU register).
		[[nodiscard]] Result< detail::mcp23017::port::PinPullUps > pullUps();

		/// Sets the pull-up configuration (GPPU register).
		[[nodiscard]] Result< void > setPullUps( const detail::mcp23017::port::PinPullUps& pullUps );

		/// Reads the captured pin states at the time of the last interrupt (INTCAP register).
		[[nodiscard]] Result< detail::mcp23017::port::PinInterruptCapture > pinInterruptCapture();

		/// Reads which pins triggered an interrupt (INTF register).
		[[nodiscard]] Result< detail::mcp23017::port::PinInterruptFlags > pinInterruptFlags();

		/// Clears all interrupt flags for the port (INTF register).
		/// NOTE: MCP23017 does not support per-pin clearing of interrupt flags!
		[[nodiscard]] Result< void > clearInterruptFlags();

		/// Reads which pins have interrupts enabled (GPINTEN register).
		[[nodiscard]] Result< detail::mcp23017::port::PinInterruptEnable > interruptEnable();

		/// Sets which pins have interrupts enabled (GPINTEN register).
		[[nodiscard]] Result< void > setInterruptEnable( const detail::mcp23017::port::PinInterruptEnable& mask );

		/// Reads the interrupt control mode for each pin (INTCON register).
		[[nodiscard]] Result< detail::mcp23017::port::PinInterruptControl > interruptControl();

		/// Sets the interrupt control mode for each pin (INTCON register).
		[[nodiscard]] Result< void > setInterruptControl( const detail::mcp23017::port::PinInterruptControl& control );

		/// Reads the default comparison values for interrupts (DEFVAL register).
		[[nodiscard]] Result< detail::mcp23017::port::PinDefaultComparison > interruptDefaults();

		/// Sets the default comparison values for interrupts (DEFVAL register).
		[[nodiscard]] Result< void >
		setInterruptDefaults( const detail::mcp23017::port::PinDefaultComparison& defaults );

	private:
		MCP23017ControllerV2& m_controller;
		Address m_address;

		// These are configurations saved in the registers!
		// detail::mcp23017::port::PinModes m_iodir; // IODIR register for input/output direction
		// detail::mcp23017::port::PinStates m_gpio; // GPIO register for pin states
		// detail::mcp23017::port::PinPullUps m_gppu; // GPPU register for pull-up configuration
		// detail::mcp23017::port::PinInterruptControl m_intcon; // INTCON register for interrupt configuration
		// detail::mcp23017::port::PinDefaultComparison m_defval; // DEFVAL register for default comparison value for interrupts
		// detail::mcp23017::port::PinInterruptEnable m_gpinten; // GPINTEN register for enabling/disabling interrupts
		// detail::mcp23017::port::PinInterruptCapture m_intcap; // INTCAP register for capturing the state at interrupt time
		// detail::mcp23017::port::PinInterruptFlags m_intf; // INTF register for interrupt flags (which pin triggered)
	};

	enum class Address : std::uint8_t
	{
		H20 = 0x20, // A2=0, A1=0, A0=0
		H21 = 0x21, // A2=0, A1=0, A0=1
		H22 = 0x22, // A2=0, A1=1, A0=0
		H23 = 0x23, // A2=0, A1=1, A0=1
		H24 = 0x24, // A2=1, A1=0, A0=0
		H25 = 0x25, // A2=1, A1=0, A0=1
		H26 = 0x26, // A2=1, A1=1, A0=0
		H27 = 0x27 // A2=1, A1=1, A0=1
	};

	using enum Address;

	/// Default ctor, all pins are configured as output by default
	explicit MCP23017ControllerV2( BusController& busController, Address address = H20 ) noexcept;

	[[nodiscard]] auto& portA( this auto& self ) noexcept { return self.m_portA; }
	[[nodiscard]] auto& portB( this auto& self ) noexcept { return self.m_portB; }

private:
	MCP23017ControllerV2( const MCP23017ControllerV2& ) = delete;
	MCP23017ControllerV2& operator=( const MCP23017ControllerV2& ) = delete;

private:
	Port m_portA;
	Port m_portB;
};

template < typename Dispatcher >
class MCP23017ControllerV2::Port::Pin
{
public:
	Pin( Port& port, Pins pin, Dispatcher dispatcher, PinTag )
		: m_port{ port }
		, m_pin{ pin }
		, m_dispatcher{ std::move( dispatcher ) }
	{ }

	/// TBW
	[[nodiscard]] auto pin() const { return m_pin; }

	/// Retrieves pin mode from MCP23017 iodir register
	[[nodiscard]] Result< PinMode > mode() const
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinModes{}, m_pin );
	}

	[[nodiscard]] Result< bool > isInput() const
	{
		return mode().transform( []( PinMode mode ) { return mode == PinMode::INPUT; } );
	}

	[[nodiscard]] Result< bool > isOutput() const
	{
		return mode().transform( []( PinMode mode ) { return mode == PinMode::OUTPUT; } );
	}

	[[nodiscard]] Result< void > setMode( const PinMode mode )
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinModes{}, m_pin, mode );
	}

	[[nodiscard]] Result< PinState > pinState()
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinState{}, m_pin );
	}

	[[nodiscard]] Result< void > setPinState( const PinState state )
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinState{}, m_pin, state );
	}

	[[nodiscard]] Result< void > switchPinState()
	{
		return pinState()
			.transform( utils::Toggle< PinState >{ PinState::HIGH, PinState::LOW } )
			.and_then( [ this ]( PinState newState ) -> Result< void > { return setPinState( newState ); } );
	}

	// Enable or disable the pull-up resistor for this pin
	[[nodiscard]] Result< void > setPullUpResistor( bool enable )
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinPullUps{}, m_pin, enable );
	}

	// Enable or disable interrupts for this pin
	[[nodiscard]] Result< void >
	enableInterrupt( bool enable, bool compareWithDefault = false, PinState defaultValue = PinState::LOW )
	{
		return std::invoke(
			m_dispatcher, detail::mcp23017::port::PinInterruptControl{}, enable, compareWithDefault, defaultValue );
	}

	[[nodiscard]] Result< void > setInterruptTrigger( detail::mcp23017::port::InterruptControl controlMode,
													  PinState defaultValue = PinState::LOW )
	{
		return std::invoke( m_dispatcher,
							detail::mcp23017::port::PinInterruptControl{},
							m_pin,
							true, // enable
							controlMode == detail::mcp23017::port::InterruptControl::COMPARE,
							defaultValue );
	}

	[[nodiscard]] Result< bool > isInterruptTriggered()
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinInterruptFlags{}, m_pin );
	}

	[[nodiscard]] Result< PinState > interruptCaptureState() const
	{
		return std::invoke( m_dispatcher, detail::mcp23017::port::PinInterruptCapture{}, m_pin );
	}

private:
	Port& m_port;
	Pins m_pin;
	Dispatcher m_dispatcher;
};

[[nodiscard]] inline auto MCP23017ControllerV2::Port::pin( Pins pin )
{
	namespace dmp = detail::mcp23017::port;

	auto dispatcher = utils::Overloaded{
		//  IODIR (PinModes)
		[ this ]( dmp::PinModes, Pins inPin ) -> Result< PinMode > {
			return pinModes().and_then( [ inPin ]( const auto& modes ) -> Result< PinMode > {
				if( auto mode = modes[ static_cast< std::size_t >( inPin ) ] )
				{
					return *mode;
				}
				return utils::MakeError< PinMode >( utils::ErrorCode::INVALID_ARGUMENT );
			} );
		},
		[ this ]( dmp::PinModes, Pins inPin, PinMode mode ) -> Result< void > {
			return pinModes().and_then( [ this, inPin, mode ]( auto modes ) -> Result< void > {
				if( !modes.setPin( static_cast< std::size_t >( inPin ), mode ) )
				{
					return utils::MakeError< void >( utils::ErrorCode::UNEXPECTED_ERROR );
				}
				return setPinModes( modes );
			} );
		},

		//  GPIO (PinState)
		[ this ]( dmp::PinState, Pins inPin ) -> Result< PinState > {
			return pinStates().and_then( [ inPin ]( const auto& states ) -> Result< PinState > {
				if( auto state = states[ static_cast< std::size_t >( inPin ) ] )
				{
					return *state;
				}
				return utils::MakeError< PinState >( utils::ErrorCode::INVALID_ARGUMENT );
			} );
		},
		[ this ]( dmp::PinState, Pins inPin, PinState state ) -> Result< void > {
			return pinStates().and_then( [ this, inPin, state ]( auto states ) -> Result< void > {
				if( !states.setPin( static_cast< std::size_t >( inPin ), state ) )
				{
					return utils::MakeError< void >( utils::ErrorCode::UNEXPECTED_ERROR );
				}
				return setPinStates( states );
			} );
		},

		//  GPPU (Pull-Up Resistor)
		[ this ]( dmp::PinPullUps, Pins inPin, bool enable ) -> Result< void > {
			return pullUps().and_then( [ this, inPin, enable ]( auto pulls ) -> Result< void > {
				if( !pulls.setPin( static_cast< std::size_t >( inPin ), enable ) )
				{
					return utils::MakeError< void >( utils::ErrorCode::UNEXPECTED_ERROR );
				}
				return setPullUps( pulls );
			} );
		},

		// INTCON, DEFVAL, GPINTEN (Interrupt Control)
		[ this ]( dmp::PinInterruptControl, Pins inPin, bool enable, bool compareWithDefault, PinState defaultValue )
			-> Result< void > {
			return interruptControl()
				.and_then( [ this, inPin, compareWithDefault ]( auto intcon ) -> Result< void > {
					if( !intcon.setPin( static_cast< std::size_t >( inPin ),
										utils::select( compareWithDefault,
													   dmp::InterruptControl::COMPARE,
													   dmp::InterruptControl::PREVIOUS ) ) )
					{
						return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
					}

					return setInterruptControl( intcon );
				} )
				.and_then( [ this, inPin, defaultValue ] {
					return interruptDefaults().and_then( [ this, inPin, defaultValue ](
															 auto defval ) -> Result< void > {
						if( !defval.setPin( static_cast< std::size_t >( inPin ), defaultValue == dmp::PinState::HIGH ) )
						{
							return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
						}

						return setInterruptDefaults( defval );
					} );
				} )
				.and_then( [ this, inPin, enable, defaultValue ] {
					return interruptEnable().and_then(
						[ this, inPin, enable, defaultValue ]( auto enables ) -> Result< void > {
							if( !enables.setPin( static_cast< std::size_t >( inPin ), enable ) )
							{
								return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
							}

							return setInterruptEnable( enables );
						} );
				} );
		},

		// INTF (Interrupt Flag): check flag
		[ this ]( dmp::PinInterruptFlags, Pins inPin ) -> Result< bool > {
			return pinInterruptFlags().transform( [ inPin ]( const auto& flags ) {
				return flags[ static_cast< std::size_t >( inPin ) ].value_or( false );
			} );
		},

		//  INTCAP (Interrupt Capture)
		[ this ]( dmp::PinInterruptCapture, Pins inPin ) -> Result< PinState > {
			return pinInterruptCapture().and_then( [ inPin ]( const auto& cap ) -> Result< PinState > {
				if( auto val = cap[ static_cast< std::size_t >( inPin ) ] )
				{
					return *val;
				}

				return utils::MakeError< PinState >( utils::ErrorCode::INVALID_ARGUMENT );
			} );
		}

	};

	// TODO: This will get more complicated
	return Pin{ *this, pin, dispatcher, PinTag{} };
}

} // namespace pbl::i2c
#endif // PBL_I2C_MCP23017_CONTROLLER_HPP__
