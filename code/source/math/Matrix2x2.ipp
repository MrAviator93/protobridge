#ifndef PBL_MATH_MATRIX_2X2_IPP__
#define PBL_MATH_MATRIX_2X2_IPP__

namespace pbl::math
{

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+( const Matrix2x2< T >& other ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] + other.m_data[ 0 ],
					  this->m_data[ 1 ] + other.m_data[ 1 ],
					  this->m_data[ 2 ] + other.m_data[ 2 ],
					  this->m_data[ 3 ] + other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator+=( const Matrix2x2& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] + other.m_data[ 0 ];
	self.m_data[ 1 ] + other.m_data[ 1 ];
	self.m_data[ 2 ] + other.m_data[ 2 ];
	self.m_data[ 3 ] + other.m_data[ 3 ];
	return *this;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+( const T scalar ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] + scalar,
					  this->m_data[ 1 ] + scalar,
					  this->m_data[ 2 ] + scalar,
					  this->m_data[ 3 ] + scalar );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator+=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += scalar;
	self.m_data[ 1 ] += scalar;
	self.m_data[ 2 ] += scalar;
	self.m_data[ 3 ] += scalar;
	return self;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator-( const Matrix2x2< T >& other ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] - other.m_data[ 0 ],
					  this->m_data[ 1 ] - other.m_data[ 1 ],
					  this->m_data[ 2 ] - other.m_data[ 2 ],
					  this->m_data[ 3 ] - other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator-( const T scalar ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] - scalar,
					  this->m_data[ 1 ] - scalar,
					  this->m_data[ 2 ] - scalar,
					  this->m_data[ 3 ] - scalar );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator-=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] -= scalar;
	self.m_data[ 1 ] -= scalar;
	self.m_data[ 2 ] -= scalar;
	self.m_data[ 3 ] -= scalar;
	return self;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( const Matrix2x2< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix2x2( self.m_data[ 0 ] * other.m_data[ 0 ] + self.m_data[ 1 ] * other.m_data[ 2 ],
					  self.m_data[ 0 ] * other.m_data[ 1 ] + self.m_data[ 1 ] * other.m_data[ 3 ],
					  self.m_data[ 2 ] * other.m_data[ 0 ] + self.m_data[ 3 ] * other.m_data[ 2 ],
					  self.m_data[ 2 ] * other.m_data[ 1 ] + self.m_data[ 3 ] * other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator*=( const Matrix2x2< T >& other ) noexcept
{
	auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& mdata = other.m_data;

	// Temporary variables to hold the results
	const T m0 = sdata[ 0 ] * mdata[ 0 ] + sdata[ 1 ] * mdata[ 2 ];
	const T m1 = sdata[ 0 ] * mdata[ 1 ] + sdata[ 1 ] * mdata[ 3 ];
	const T m2 = sdata[ 2 ] * mdata[ 0 ] + sdata[ 3 ] * mdata[ 2 ];
	const T m3 = sdata[ 2 ] * mdata[ 1 ] + sdata[ 3 ] * mdata[ 3 ];

	// Assign the results back to self
	self.m_data[ 0 ] = m0;
	self.m_data[ 1 ] = m1;
	self.m_data[ 2 ] = m2;
	self.m_data[ 3 ] = m3;

	return self;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix2x2(
		self.m_data[ 0 ] * scalar, self.m_data[ 1 ] * scalar, self.m_data[ 2 ] * scalar, self.m_data[ 3 ] * scalar );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator*=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] *= scalar;
	self.m_data[ 1 ] *= scalar;
	self.m_data[ 2 ] *= scalar;
	self.m_data[ 3 ] *= scalar;
	return self;
}

} // namespace pbl::math
#endif // PBL_MATH_MATRIX_2X2_IPP__
