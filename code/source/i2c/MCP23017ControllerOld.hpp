#ifndef PBL_I2C_MCP23017_CONTROLLER_OLD_HPP__
#define PBL_I2C_MCP23017_CONTROLLER_OLD_HPP__

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
	[[deprecated( "Use the new version of MCP23017Controller" )]] explicit MCP23017Controller(
		BusController& busController, Address address = H20 ) noexcept;

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

} // namespace pbl::i2c
#endif // PBL_I2C_MCP23017_CONTROLLER_OLD_HPP__
