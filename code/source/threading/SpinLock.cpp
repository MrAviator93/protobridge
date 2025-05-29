#include "SpinLock.hpp"

// C++
#include <thread>

namespace pbl::threading
{

namespace
{

/// Exponential backoff strategy
class ExpBackOffStrategy
{
public:
	using Type = std::uint32_t;

	/**
	 * @brief Construct a new Exp Back Off Strategy object
	 * 
	 * For Spinlock choosing values are very dependent on the task.
	 * If you have 
	 * 
	 * @param initial 
	 */
	ExpBackOffStrategy( Type initial = 1'000, Type step = 2, Type threshold = 10'000 )
		: m_initial{ initial }
		, m_step{ step }
		, m_threshold{ threshold }
		, m_current{ initial }
	{ }

	void operator()()
	{
		for( Type i{ 0u }; i < m_current; ++i )
		{
			nop();
		}

		m_current *= m_step;
		if( m_current > m_threshold )
		{
			m_current = m_threshold;
			std::this_thread::yield();
		}
	}

	void reset() { m_current = m_initial; }

private:
	inline void nop()
	{
		// There are different ways to go about this
		asm volatile( "nop;" );
	}

private:
	Type m_initial{};
	Type m_step{};
	Type m_threshold{};
	Type m_current{};
};

} // namespace

void SpinLock::deepLock() noexcept
{
	ExpBackOffStrategy strategy;
	while( m_locked.test_and_set( std::memory_order::acquire ) )
	{
		// // TODO: Use reinterpret_cast<> here or static_cast<>
		// if( *(volatile std::uint32_t*)&m_locked == 0 )
		// {
		// 	if( !m_locked.test_and_set( std::memory_order::acquire ) )
		// 	{
		// 		return;
		// 	}
		// }

		strategy();
	}
}

} // namespace pbl::threading
