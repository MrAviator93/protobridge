#ifndef PBL_DEVICES_RELAY_HPP__
#define PBL_DEVICES_RELAY_HPP__

#include <utils/Result.hpp>

// C++
#include <bitset>
#include <cstdint>

namespace pbl::devices
{

/// Relay control abstraction.
template < typename Backend, std::size_t MaxRelays = 32 >
class Relay
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	explicit Relay( Backend& backend )
		: m_backend{ backend }
	{ }

	/// Turn relay ON
	Result< void > on( std::size_t pinIdx )
	{
		if( !validIndex( pinIdx ) )
		{
			return utils::MakeError( "Invalid pin index" );
		}

		auto result = m_backend.set( pinIdx, true );
		if( result )
		{
			m_state.set( pinIdx );
		}

		return result;
	}

	/// Turn relay OFF
	Result< void > off( std::size_t pinIdx )
	{
		if( !validIndex( pinIdx ) )
		{
			return utils::MakeError( "Invalid pin index" );
		}

		auto result = m_backend.set( pinIdx, false );
		if( result )
		{
			m_state.reset( pinIdx );
		}

		return result;
	}

	/// Toggle relay state
	Result< void > toggle( std::size_t pinIdx )
	{
		if( !validIndex( pinIdx ) )
		{
			return utils::MakeError( "Invalid pin index" );
		}

		const bool newState = !m_state.test( pinIdx );
		auto result = m_backend.set( pinIdx, newState );
		if( result )
		{
			m_state.set( pinIdx, newState );
		}

		return result;
	}

	/// Returns the cached logical state of the relay (may not reflect hardware if external changes occur)
	bool isOn( std::size_t pinIdx ) const { return validIndex( pinIdx ) && m_state.test( pinIdx ); }

private:
	[[nodiscard]]
	constexpr bool validIndex( std::size_t idx ) const noexcept
	{
		return idx < MaxRelays;
	}

	Backend& m_backend;
	std::bitset< MaxRelays > m_state{};
};

} // namespace pbl::devices
#endif // PBL_DEVICES_RELAY_HPP__
