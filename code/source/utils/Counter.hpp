#ifndef PBL_UTILS_COUNTER_HPP__
#define PBL_UTILS_COUNTER_HPP__

// C++
#include <atomic>

namespace pbl::utils
{

/**
 * @class Counter
 * @brief Counts how many classes of type T we have.
 * This class must be inherited either as private or protected.
 * Use when it's necessary to keep track of class object count.
 * 
 * @tparam T 
 */
template < typename T >
class Counter
{
public:
	Counter() { ++m_counter; }

	Counter( const Counter& ) { ++m_counter; }

	Counter& operator=( const Counter& c )
	{
		if( this != &c )
		{
			--m_counter;
		}
		return *this;
	}

	Counter( Counter&& ) { ++m_counter; }

	Counter& operator=( Counter&& c )
	{
		if( this != &c )
		{
			--m_counter;
		}
		return *this;
	}

	~Counter() { --m_counter; }

	/// Returns the current count of instances.
	[[nodiscard]] static std::size_t count() { return m_counter; }

private:
	inline static std::atomic_size_t m_counter{}; //!< Internal counter of number of instances of object of type T
};

} // namespace pbl::utils
#endif // PBL_UTILS_COUNTER_HPP__
