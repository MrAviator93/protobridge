#ifndef PBL_EXAMPLES_GENERAL_THERMOSTAT_CHECK_ACTIVE_HPP__
#define PBL_EXAMPLES_GENERAL_THERMOSTAT_CHECK_ACTIVE_HPP__

// C++
#include <functional>

namespace PBL::Examples
{

template < typename... Ts >
class CheckActive
{
	template < typename... Args >
	CheckActive( Args... args )
		: m_args{ std::ref( args )... }
	{ }

	[[nodiscard]] bool isActive() const
	{
		return std::apply(
			[ & ]( const auto&... args ) {
				const auto isActive = [] [[nodiscard]]< typename T >( const T& arg ) -> bool {
					if constexpr( requires { arg.isActive(); } )
					{
						return arg.isActive();
					}

					return true;
				};

				return ( ... && isActive( args.get() ) );
			},
			m_args );
	}

private:
	std::tuple< std::reference_wrapper< Ts >... > m_args;
};

} // namespace PBL::Examples
#endif // PBL_EXAMPLES_GENERAL_THERMOSTAT_CHECK_ACTIVE_HPP__
