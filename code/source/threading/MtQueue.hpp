#ifndef PBL_THREADING_MT_QUEUE_HPP__
#define PBL_THREADING_MT_QUEUE_HPP__

// C++
#include <deque>
#include <vector>
#include <optional>
#include <algorithm>
#include <type_traits>
#include <shared_mutex>

namespace pbl::threading
{

/// A thread-safe queue class that uses std::shared_mutex for synchronization.
template < typename T >
class MtQueue
{
public:
	/// Default constructor.
	MtQueue() = default;

	/// Constructs the queue and reserves a specific size.
	explicit MtQueue( std::size_t size ) { m_queue.resize( size ); }

	/// Constructs the queue using an initializer list.
	MtQueue( std::initializer_list< T > initList )
	{
		auto first = std::make_move_iterator( initList.begin() );
		auto last = std::make_move_iterator( initList.end() );
		m_queue.insert( m_queue.end(), first, last );
	}

	/// Copy constructor.
	MtQueue( const MtQueue& q ) noexcept( std::is_nothrow_copy_constructible_v< T > )
	{
		std::shared_lock _{ q.m_mutex };
		m_queue = q.m_queue;
	}

	/// Move constructor.
	MtQueue( MtQueue&& q ) noexcept( std::is_nothrow_move_constructible_v< T > )
	{
		std::lock_guard _{ q.m_mutex };
		m_queue = std::move( q.m_queue );
	}

	/// Destructor, clears the queue.
	~MtQueue() { clear(); }

	/// Returns true if empty.
	[[nodiscard]] bool empty() const noexcept
	{
		std::shared_lock _{ m_mutex };
		return m_queue.empty();
	}

	/// Returns the current size of the queue.
	[[nodiscard]] std::size_t size() const noexcept
	{
		std::shared_lock _{ m_mutex };
		return m_queue.size();
	}

	/// Clears the queue.
	void clear()
	{
		std::lock_guard _{ m_mutex };
		m_queue.clear();
	}

	/// Resizes the internal queue.
	void resize( std::size_t n )
	{
		std::lock_guard _{ m_mutex };
		m_queue.resize( n );
	}

	/// Pushes an element into the queue by copy.
	void push( const T& element )
	{
		std::lock_guard _{ m_mutex };
		m_queue.push_back( element );
	}

	/// Pushes an element into the queue by moving it.
	void push( T&& element )
	{
		std::lock_guard _{ m_mutex };
		m_queue.emplace_back( std::move( element ) );
	}

	/**
	 * @brief Takes an input a container with an elements, which are pushed
	 * into the queue.
	 * 
	 * @tparam C container of elements which are to be pushed - moved into the queue 
	 * @tparam A 
	 * @param container 
	 */
	template < template < typename, typename > class C, class A >
	void push( C< T, A >& container );

	/// Retrieves and removes the front element from the queue.
	[[nodiscard]] std::optional< T > get();

	/// Retrieves and removes up to n front elements from the queue.
	[[nodiscard]] std::vector< T > get( std::size_t n );

private:
	mutable std::shared_mutex m_mutex; //!< Mutex
	std::deque< T > m_queue; //!< Queue attribute which is actually a deque
};

} // namespace pbl::threading

#include "MtQueue.ipp"

#endif // PBL_THREADING_MT_QUEUE_HPP__
