
#include "Timer.hpp"

namespace pbl::utils
{

Timer::Timer( const std::chrono::microseconds duration )
	: m_startTime{ Clock::now() }
	, m_elapsedDuration{ duration }
{ }

bool Timer::hasElapsed() const
{
	return ( Clock::now() - m_startTime ) >= m_elapsedDuration;
}

auto Timer::remainingTime() const -> Duration
{
	const auto elapsedTime = Clock::now() - m_startTime;
	if( elapsedTime >= m_elapsedDuration )
	{
		return Duration::zero();
	}
	
	return std::chrono::duration_cast< Duration >( m_elapsedDuration - elapsedTime );
}

} // namespace pbl::utils
