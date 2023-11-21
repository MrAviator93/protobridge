
#include "Timer.hpp"

namespace PBL::Utils
{

Timer::Timer()
	: m_startTP( m_clock.now() )
{ }

void Timer::start()
{
	m_startTP = m_clock.now();
}

void Timer::reset()
{
	start();
}

double Timer::elapsedTimeS() const
{
	return static_cast< double >( elapsedTimeUs() ) / 1000000.0;
}

std::int64_t Timer::elapsedTimeMs() const
{
	return std::chrono::duration_cast< Milliseconds >( m_clock.now() - m_startTP ).count();
}

std::int64_t Timer::elapsedTimeUs() const
{
	return std::chrono::duration_cast< Microseconds >( m_clock.now() - m_startTP ).count();
	// return std::chrono::time_point_cast< std::chrono::microseconds >( m_endTP ).time_since_epoch().count;
}

std::int64_t Timer::elapsedTimeNs() const
{
	return std::chrono::duration_cast< Nanoseconds >( m_clock.now() - m_startTP ).count();
}

} // namespace PBL::Utils
