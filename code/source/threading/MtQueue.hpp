#ifndef PBL_THREADING_MT_QUEUE_HPP__
#define PBL_THREADING_MT_QUEUE_HPP__

// C++
#include <deque>
#include <vector>
#include <optional>
#include <type_traits>
#include <shared_mutex>

namespace pbl::threading
{

template < typename T >
class MtQueue
{
public:
	MtQueue() = default;

	explicit MtQueue( std::size_t n )
	{
		std::lock_guard _{ m_mutex };
		m_queue.resize( n );
	}

	MtQueue( std::initializer_list< T > initList )
	{
		std::lock_guard _{ m_mutex };
		auto first = std::make_move_iterator( initList.begin() );
		auto last = std::make_move_iterator( initList.end() );
		m_queue.insert( m_queue.end(), first, last );
	}

	MtQueue( const MtQueue& q ) noexcept( std::is_nothrow_copy_constructible_v< T > )
	{
		std::shared_lock _{ q.m_mutex };
		m_queue = q.m_queue;
	}

	MtQueue( MtQueue&& q ) noexcept( std::is_nothrow_move_constructible_v< T > )
	{
		std::lock_guard _{ q.m_mutex };
		m_queue = std::move( q.m_queue );
	}

	~MtQueue() = default;

	/// Returns true if empty
	[[nodiscard]] bool empty() const noexcept
	{
		std::shared_lock _{ m_mutex };
		return m_queue.empty();
	}

	/// Returns the current size of the queue
	[[nodiscard]] std::size_t size() const noexcept
	{
		std::shared_lock _{ m_mutex };
		return m_queue.size();
	}

	/// Clears the queue
	void clear()
	{
		std::lock_guard _{ m_mutex };
		m_queue.clear();
	}

	/// Pushes an element into the queue by copy
	void push( const T& element )
	{
		std::lock_guard _{ m_mutex };
		m_queue.push_back( element );
	}

	/// Pushes an element into the queue by moving it
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
	void push( C< T, A >& container )
	{
		std::lock_guard _{ m_mutex };
		m_queue.emplace_back( std::move( container ) );
	}

	[[nodiscard]] std::optional< T > get()
	{
		std::lock_guard _{ m_mutex };

		if( m_queue.empty() ) [[unlikely]]
		{
			return std::nullopt;
		}

		T out = std::move( m_queue.front() );
		m_queue.pop_front();

		return out;
	}

	[[nodiscard]] std::vector< T > get( std::size_t n )
	{
		std::lock_guard _{ m_mutex };

		if( n == 0 || m_queue.empty() ) [[unlikely]]
		{
			return {};
		}

		const std::size_t nToCopy = std::min( n, m_queue.size() );

		std::vector< T > out;
		out.reserve( nToCopy );

		// Move elements from m_queue into out using iterators
		auto first = std::make_move_iterator( m_queue.begin() );
		auto last = std::make_move_iterator( m_queue.begin() + nToCopy );
		std::copy( first, last, std::back_inserter( out ) );

		// Erase moved-from elements
		m_queue.erase( m_queue.begin(), m_queue.begin() + nToCopy );

		return out;
	}

private:
	mutable std::shared_mutex m_mutex; //!< Mutex
	std::deque< T > m_queue; //!< Queue attribute which is actually a deque
};

} // namespace pbl::threading
#endif // PBL_THREADING_MT_QUEUE_HPP__
