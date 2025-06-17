#ifndef PBL_I2C_MCP23017_CONTROLLER_IPP__
#define PBL_I2C_MCP23017_CONTROLLER_IPP__

namespace pbl::i2c
{

[[nodiscard]] inline auto MCP23017Controller::Port::pin( Pins pin )
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
#endif // PBL_I2C_MCP23017_CONTROLLER_IPP__