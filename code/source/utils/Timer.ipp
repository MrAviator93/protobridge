#ifndef PBL_UTILS_TIMER_IPP__
#define PBL_UTILS_TIMER_IPP__

namespace pbl::utils
{

template < typename Callback >
	requires std::invocable< Callback, Timer::Dt >
auto Timer::onTick( Callback&& callback ) -> std::invoke_result_t< Callback, Dt >
{
	using Result = std::invoke_result_t< Callback, Dt >;

	if( !hasElapsed() )
	{
		if constexpr( std::is_void_v< Result > )
		{
			return;
		}
		else
		{
			return {};
		}
	}

	const auto dt = elapsedSinceSetInSeconds();

	if constexpr( std::is_void_v< Result > )
	{
		std::forward< Callback >( callback )( dt );
		set();
	}
	else
	{
		auto result = std::forward< Callback >( callback )( dt );
		set();
		return result;
	}
}

template < typename Callback >
	requires std::invocable< Callback >
auto Timer::onTick( Callback&& callback ) -> std::invoke_result_t< Callback >
{
	using Result = std::invoke_result_t< Callback >;

	if( !hasElapsed() )
	{
		if constexpr( std::is_void_v< Result > )
		{
			return;
		}
		else
		{
			return {};
		}
	}

	if constexpr( std::is_void_v< Result > )
	{
		std::forward< Callback >( callback )();
		set();
	}
	else
	{
		auto result = std::forward< Callback >( callback )();
		set();
		return result;
	}
}

} // namespace pbl::utils
#endif // PBL_UTILS_TIMER_IPP__
