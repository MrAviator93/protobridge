#ifndef PBL_THREADING_MT_QUEUE_IPP__
#define PBL_THREADING_MT_QUEUE_IPP__

namespace pbl::threading
{

template < typename T >
template < template < typename, typename > class C, class A >
void MtQueue< T >::push( C< T, A >& container )
{
	std::lock_guard _{ m_mutex };
	m_queue.emplace_back( std::move( container ) );
}

template < typename T >
[[nodiscard]] std::optional< T > MtQueue< T >::get()
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

template < typename T >
[[nodiscard]] std::vector< T > MtQueue< T >::get( std::size_t n )
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

} // namespace pbl::threading
#endif // PBL_THREADING_MT_QUEUE_IPP__