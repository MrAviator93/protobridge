#ifndef PBL_I2C_MCP23017_CONTROLLER_HPP__
#define PBL_I2C_MCP23017_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>
#include <utils/PinConfig.hpp>

// C++
#include <array>
#include <expected>
#include <functional>

namespace pbl::i2c
{

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

	public:
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

		using PinModeTo = decltype( []( auto v ) noexcept -> bool { return v == PinMode::INPUT; } );
		using PinModeFrom =
			decltype( []( auto b ) noexcept -> PinMode { return b == true ? PinMode::INPUT : PinMode::OUTPUT; } );

		/// PinModes represents IODIR register for pin direction configuration.
		/// Each bit indicates whether the corresponding pin is set to INPUT (1) or OUTPUT (0).
		using PinModes = utils::PinConfig< PinMode, PinMode::OUTPUT, PinModeTo, PinModeFrom >;

		using PinStatesTo = decltype( []( auto v ) noexcept -> bool { return v == PinState::HIGH; } );
		using PinStatesFrom =
			decltype( []( auto b ) noexcept -> PinState { return b == true ? PinState::HIGH : PinState::LOW; } );

		/// PinStates represents the GPIO register, reflecting current pin states.
		/// Each bit indicates the actual logic level (HIGH or LOW) of each pin.
		using PinStates = utils::PinConfig< PinState, PinState::LOW, PinStatesTo, PinStatesFrom >;

		using InterruptControlTo = decltype( []( auto v ) noexcept -> bool { return v == InterruptControl::COMPARE; } );
		using InterruptControlFrom = decltype( []( auto b ) noexcept -> InterruptControl {
			return b == true ? InterruptControl::COMPARE : InterruptControl::PREVIOUS;
		} );

		/// PinInterruptControl represents the INTCON register for interrupt conditions.
		/// Each bit indicates whether a pin triggers an interrupt on comparison with DEFVAL (1) or change from previous state (0).
		using PinInterruptControl =
			utils::PinConfig< InterruptControl, InterruptControl::PREVIOUS, InterruptControlTo, InterruptControlFrom >;

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
		using PinInterruptCapture = utils::PinConfig< PinState, PinState::LOW, PinStatesTo, PinStatesFrom >;

		/// Interrupt flags indicating which pins triggered an interrupt, reflecting INTF register.
		/// This is a read-only configuration, showing which pins currently have an interrupt pending.
		using PinInterruptFlags = utils::PinConfig< bool, false >;

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

		class Pin
		{
		public:
			using GetPinModeCb = std::function< PinMode( Pins ) >;
			using SetPinModeCb = std::function< Result< void >( Pins, PinMode ) >;
			using GetPinStateCb = std::function< Result< PinState >( Pins ) >;
			using SetPinStateCb = std::function< Result< void >( Pins, PinState ) >;
			using SetPullUpCb = std::function< Result< void >( Pins, bool ) >;
			using EnableInterruptCb = std::function< Result< void >( Pins, bool, bool, PinState ) >;

			Pin( Port& port, Pins pin, PinTag )
				: m_port{ port }
				, m_pin{ pin }
			{ }

			/// TBW
			[[nodiscard]] auto pin() const { return m_pin; }

			[[nodiscard]] PinMode mode() const
			{
				// TODO: return m_getPinModeCb(m_pin);
				return PinMode::INPUT;
			}

			[[nodiscard]] bool isInput() const { return mode() == PinMode::INPUT; }
			[[nodiscard]] bool isOutput() const { return mode() == PinMode::OUTPUT; }

			Result< void > setMode( const PinMode mode )
			{
				// TODO: return m_setPinModeCb(m_pin, mode);
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			[[nodiscard]] Result< PinState > pinState()
			{
				// TODO: return m_pinStateCb(m_pin);
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			Result< void > setPinState( const PinState state )
			{
				// TODO: return m_setPinState(m_pin, state);
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			Result< void > switchPinState()
			{
				// TODO: return m_switchPinState(m_pin, state);
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			// Enable or disable the pull-up resistor for this pin
			Result< void > setPullUpResistor( bool enable )
			{
				// TODO return m_setPullUpCb( m_pin, enable );
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			// Enable or disable interrupts for this pin
			Result< void >
			enableInterrupt( bool enable, bool compareWithDefault = false, PinState defaultValue = PinState::LOW )
			{
				// TODO: return m_enableInterruptCb( m_pin, enable, compareWithDefault, defaultValue );
				return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
			}

			// Result<void> setInterruptTrigger(Pins pin, bool onChange, PinState triggerState = PinState::LOW);

			// Result<void> clearInterruptFlag();

			// [[nodiscard]] bool isInterruptTriggered() const;

		private:
			Port& m_port;
			Pins m_pin;
		};

		/// Returns a specific pin
		[[nodiscard]] Pin pin( Pins pin );

		/// Returns all pin modes
		// [[nodiscard]] Result< PinModes > pinModes();

		/// Returns all pin states
		// [[nodiscard]] Result< PinStates > pinStates();

		/// TBW, Read Only (Should be easy to implement, just read)
		// [[nodiscard]] Result<PinInterruptCapture> pinInterruptCapture();

		/// TBW, Read Only (Should be easy to implement, just read)
		// [[nodiscard]] Result<PinInterruptFlags> pinInterruptFlags();

	private:
		MCP23017ControllerV2& m_controller;
		Address m_address;

		// There are configurations saved in the registers!
		// std::bitset< 8 > m_iodir; // IODIR register for input/output direction
		// std::bitset< 8 > m_gpio; // GPIO register for pin states
		// std::bitset< 8 > m_gppu; // GPPU register for pull-up configuration
		// std::bitset< 8 > m_intcon; // INTCON register for interrupt configuration
		// std::bitset< 8 > m_defval; // DEFVAL register for default comparison value for interrupts
		// std::bitset< 8 > m_gpinten; // GPINTEN register for enabling/disabling interrupts
		// std::bitset< 8 > m_intcap; // INTCAP register for capturing the state at interrupt time
		// std::bitset< 8 > m_intf; // INTF register for interrupt flags (which pin triggered)
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

} // namespace pbl::i2c
#endif // PBL_I2C_MCP23017_CONTROLLER_HPP__
