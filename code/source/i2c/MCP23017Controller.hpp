#ifndef PBL_I2C_MCP23017_CONTROLLER_HPP__
#define PBL_I2C_MCP23017_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <array>
#include <bitset>
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
 * BusController busController;
 * MCP23017Controller mcp23017(busController); // assume 0x20 is the I2C address of MCP23017
 *
 * // Set individual pins or multiple pins as outputs or inputs
 * mcp23017.setPortConfig(MCP23017Controller::Port::PORT_A, 0xFF); // All pins as inputs on Port A
 * mcp23017.setPortConfig(MCP23017Controller::Port::PORT_B, 0x00); // All pins as outputs on Port B
 *
 * // Turn on and off pins on Port B
 * mcp23017.setOnPortB(MCP23017Controller::Pins::PIN_1);   // Turn on Pin 1 on Port B
 * mcp23017.setOffPortB(MCP23017Controller::Pins::PIN_1);  // Turn off Pin 1 on Port B
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
class MCP23017Controller final : public ICBase, public utils::Counter< MCP23017Controller >
{
public:
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

	/// MCP23017 chip pin states, this needs to be checked! Could be otherway around!
	enum class PinState : std::uint8_t
	{
		OFF = 0,
		ON = 1
	};

	/// MCP23017 chip available pinmodes
	enum class PinMode : std::uint8_t
	{
		OUTPUT = 0,
		INPUT = 1
	};

	/// MCP23017 chip available ports
	enum class Port : std::uint8_t
	{
		PORT_A = 0x14,
		PORT_B = 0x15
	};

	/// MCP23017 chip available pins on port A and B
	enum Pins : std::uint8_t
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

	using enum Port;
	using enum Address;
	using enum PinMode;
	using enum PinState;

	/// Default ctor, all pins are configured as output by default
	explicit MCP23017Controller( BusController& busController, Address address = H20 ) noexcept;

	/// The secondary ctor, which enables the individual port configuration
	MCP23017Controller( BusController& busController,
						std::uint8_t portAConfig,
						std::uint8_t portBConfig,
						Address address = H20 ) noexcept;

	/// TBW
	bool setPortConfig( Port port, std::uint8_t config );

	// Set's pin on on port A
	bool setOnPortA( Pins pin );

	// TBW ...
	template < typename... Args >
	bool setOnPortA( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portAConfiguration & pinValue ) == 0 )
		{
			m_portAPinStates &= ~pinValue;
			return write( 0x14, m_portAPinStates );
		}

		return false;
	}

	/// TBW
	bool setOffPortA( Pins pin );

	// TBW ...
	template < typename... Args >
	bool setOffPortA( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portAConfiguration & pinValue ) == 0 )
		{
			m_portAPinStates |= pinValue;
			return write( 0x14, m_portAPinStates );
		}

		return false;
	}

	// Set's pin on on port B
	bool setOnPortB( Pins pin );

	// TBW ...
	template < typename... Args >
	bool setOnPortB( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portBConfiguration & pinValue ) == 0 )
		{
			m_portBPinStates &= ~pinValue;
			return write( 0x15, m_portBPinStates );
		}

		return false;
	}

	/// TBW
	bool setOffPortB( Pins pin );

	// TBW ...
	template < typename... Args >
	bool setOffPortB( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portBConfiguration & pinValue ) == 0 )
		{
			m_portBPinStates |= pinValue;
			return write( 0x15, m_portBPinStates );
		}

		return false;
	}

private:
	/// Configures A and B ports setting pins as inputs or outputs
	bool configure();

	/// Retrieves current pin states for A and B ports
	bool retrieve();

private:
	std::uint8_t m_portAConfiguration{}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port A
	std::uint8_t m_portBConfiguration{}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port B

	std::uint8_t m_portAPinStates{}; //!< Current pin states for port A
	std::uint8_t m_portBPinStates{}; //!< Current pin states for port B
};

/// Version 2 of the original MCP23017 controller
class MCP23017ControllerV2 final : public ICBase, public utils::Counter< MCP23017ControllerV2 >
{

	struct PortTag
	{ };

public:
	template < typename T >
	using Result = std::expected< T, std::string >;

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

		/// MCP23017 chip pin states, this needs to be checked! Could be otherway around!
		enum class PinState : std::uint8_t
		{
			OFF = 0,
			ON = 1
		};

		/// MCP23017 chip available pinmodes
		enum class PinMode : std::uint8_t
		{
			OUTPUT = 0,
			INPUT = 1
		};

		template < typename T, T Default, typename AssignmentOp = decltype( []( const T& ) { return false; } ) >
		struct PinConfig
		{
			[[nodiscard]] constexpr operator std::bitset< 8 >() const noexcept
			{
				std::bitset< 8 > bitset;
				bitset[ 0 ] = AssignmentOp{}( pin1 );
				bitset[ 1 ] = AssignmentOp{}( pin2 );
				bitset[ 2 ] = AssignmentOp{}( pin3 );
				bitset[ 3 ] = AssignmentOp{}( pin4 );
				bitset[ 4 ] = AssignmentOp{}( pin5 );
				bitset[ 5 ] = AssignmentOp{}( pin6 );
				bitset[ 6 ] = AssignmentOp{}( pin7 );
				bitset[ 7 ] = AssignmentOp{}( pin8 );
				return bitset;
			}

			T pin1{ Default };
			T pin2{ Default };
			T pin3{ Default };
			T pin4{ Default };
			T pin5{ Default };
			T pin6{ Default };
			T pin7{ Default };
			T pin8{ Default };
		};

		/// TBW
		using PinModeAssignOp = decltype( []( auto v ) { return v == PinMode::INPUT; } );
		using PinModes = PinConfig< PinMode, PinMode::OUTPUT, PinModeAssignOp >;

		/// TBW
		using PinStatesAssignOp = decltype( []( auto v ) { return v == PinState::ON; } );
		using PinStates = PinConfig< PinState, PinState::OFF, PinStatesAssignOp >;

		/// Allows to configure individual pins with enabled or disabled interrupt
		using PinInterrupts = PinConfig< bool, false, decltype( []( auto v ) { return v; } ) >;

		/// Allows to configure individual pins with enabled or disabled pull-up resistor
		using PinPullUps = PinConfig< bool, false, decltype( []( auto v ) { return v; } ) >;

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
				return std::unexpected( "Not implemented ..." );
			}

			[[nodiscard]] Result< PinState > pinState()
			{
				// TODO: return m_pinStateCb(m_pin);
				return std::unexpected( "Not implemented ..." );
			}

			Result< void > setPinState( const PinState state )
			{
				// TODO: return m_setPinState(m_pin, state);
				return std::unexpected( "Not implemented ..." );
			}

			Result< void > switchPinState()
			{
				// TODO: return m_switchPinState(m_pin, state);
				return std::unexpected( "Not implemented ..." );
			}

			// Enable or disable the pull-up resistor for this pin
			Result< void > setPullUpResistor( bool enable )
			{
				// TODO return m_setPullUpCb( m_pin, enable );
				return std::unexpected( "Not implemented ..." );
			}

			// Enable or disable interrupts for this pin
			Result< void >
			enableInterrupt( bool enable, bool compareWithDefault = false, PinState defaultValue = PinState::OFF )
			{
				// TODO: return m_enableInterruptCb( m_pin, enable, compareWithDefault, defaultValue );
				return std::unexpected( "Not implemented ..." );
			}

			// Result<void> setInterruptTrigger(Pins pin, bool onChange, PinState triggerState = PinState::OFF);

			// Result<void> clearInterruptFlag();

			// [[nodiscard]] bool isInterruptTriggered() const;

		private:
			Port& m_port;
			Pins m_pin;
		};

		/// Returns a specific pin
		[[nodiscard]] Pin pin( Pins pin );

		/// Returns all pin modes
		[[nodiscard]] Result< PinModes > pinModes();

		/// Returns all pin states
		// [[nodiscard]] PinStates pinStates();

	private:
		MCP23017ControllerV2& m_controller;
		Address m_address;
		std::bitset< 8 > m_iodir; // IODIR register for input/output direction
		std::bitset< 8 > m_gpio; // GPIO register for pin states
		std::bitset< 8 > m_gppu; // GPPU register for pull-up configuration
		std::bitset< 8 > m_intcon; // INTCON register for interrupt configuration
		std::bitset< 8 > m_defval; // DEFVAL register for default comparison value for interrupts
		std::bitset< 8 > m_gpinten; // GPINTEN register for enabling/disabling interrupts
		std::bitset< 8 > m_intcap; // INTCAP register for capturing the state at interrupt time
		std::bitset< 8 > m_intf; // INTF register for interrupt flags (which pin triggered)
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
	Port m_portA;
	Port m_portB;
};

} // namespace pbl::i2c
#endif // PBL_I2C_MCP23017_CONTROLLER_HPP__
