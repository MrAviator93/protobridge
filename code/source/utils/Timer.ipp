#ifndef PBL_UTILS_TIMER_IPP__
#define PBL_UTILS_TIMER_IPP__

namespace pbl::utils
{

template < typename Callback >
	requires std::invocable< Callback, Timer::Dt >
auto Timer::onTick( Callback&& callback ) -> std::invoke_result_t< Callback, Dt >
{
	if( hasElapsed() )
	{
		const auto dt = elapsedSinceSetInSeconds();

		if constexpr( std::is_void_v< std::invoke_result_t< Callback, Dt > > )
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
	else // The timer is not yet elapsed
	{
		if constexpr( !std::is_void_v< std::invoke_result_t< Callback, Dt > > )
		{
			return {}; // Only return if the return type is not void
		}
		else
		{
			return;
		}
	}
}

} // namespace pbl::utils
#endif // PBL_UTILS_TIMER_IPP__
