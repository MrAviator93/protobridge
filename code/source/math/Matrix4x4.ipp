#ifndef PBL_MATH_MATRIX_4X4_IPP__
#define PBL_MATH_MATRIX_4X4_IPP__

namespace pbl::math
{

template < typename T >
constexpr Matrix4x4< T > Matrix4x4< T >::operator+( const Matrix4x4< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix4x4{ self.m_data[ 0 ] + other.m_data[ 0 ],
					  self.m_data[ 1 ] + other.m_data[ 1 ],
					  self.m_data[ 2 ] + other.m_data[ 2 ],
					  self.m_data[ 3 ] + other.m_data[ 3 ],
					  self.m_data[ 4 ] + other.m_data[ 4 ],
					  self.m_data[ 5 ] + other.m_data[ 5 ],
					  self.m_data[ 6 ] + other.m_data[ 6 ],
					  self.m_data[ 7 ] + other.m_data[ 7 ],
					  self.m_data[ 8 ] + other.m_data[ 8 ],
					  self.m_data[ 9 ] + other.m_data[ 9 ],
					  self.m_data[ 10 ] + other.m_data[ 10 ],
					  self.m_data[ 11 ] + other.m_data[ 11 ],
					  self.m_data[ 12 ] + other.m_data[ 12 ],
					  self.m_data[ 13 ] + other.m_data[ 13 ],
					  self.m_data[ 14 ] + other.m_data[ 14 ],
					  self.m_data[ 15 ] + other.m_data[ 15 ] };
}

template < typename T >
constexpr Matrix4x4< T >& Matrix4x4< T >::operator+=( const Matrix4x4< T >& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += other.m_data[ 0 ];
	self.m_data[ 1 ] += other.m_data[ 1 ];
	self.m_data[ 2 ] += other.m_data[ 2 ];
	self.m_data[ 3 ] += other.m_data[ 3 ];
	self.m_data[ 4 ] += other.m_data[ 4 ];
	self.m_data[ 5 ] += other.m_data[ 5 ];
	self.m_data[ 6 ] += other.m_data[ 6 ];
	self.m_data[ 7 ] += other.m_data[ 7 ];
	self.m_data[ 8 ] += other.m_data[ 8 ];
	self.m_data[ 9 ] += other.m_data[ 9 ];
	self.m_data[ 10 ] += other.m_data[ 10 ];
	self.m_data[ 11 ] += other.m_data[ 11 ];
	self.m_data[ 12 ] += other.m_data[ 12 ];
	self.m_data[ 13 ] += other.m_data[ 13 ];
	self.m_data[ 14 ] += other.m_data[ 14 ];
	self.m_data[ 15 ] += other.m_data[ 15 ];
	return self;
}

template < typename T >
constexpr Matrix4x4< T > Matrix4x4< T >::operator+( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix4x4{ self.m_data[ 0 ] + scalar,
					  self.m_data[ 1 ] + scalar,
					  self.m_data[ 2 ] + scalar,
					  self.m_data[ 3 ] + scalar,
					  self.m_data[ 4 ] + scalar,
					  self.m_data[ 5 ] + scalar,
					  self.m_data[ 6 ] + scalar,
					  self.m_data[ 7 ] + scalar,
					  self.m_data[ 8 ] + scalar,
					  self.m_data[ 9 ] + scalar,
					  self.m_data[ 10 ] + scalar,
					  self.m_data[ 11 ] + scalar,
					  self.m_data[ 12 ] + scalar,
					  self.m_data[ 13 ] + scalar,
					  self.m_data[ 14 ] + scalar,
					  self.m_data[ 15 ] + scalar };
}

template < typename T >
constexpr Matrix4x4< T >& Matrix4x4< T >::operator+=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += scalar;
	self.m_data[ 1 ] += scalar;
	self.m_data[ 2 ] += scalar;
	self.m_data[ 3 ] += scalar;
	self.m_data[ 4 ] += scalar;
	self.m_data[ 5 ] += scalar;
	self.m_data[ 6 ] += scalar;
	self.m_data[ 7 ] += scalar;
	self.m_data[ 8 ] += scalar;
	self.m_data[ 9 ] += scalar;
	self.m_data[ 10 ] += scalar;
	self.m_data[ 11 ] += scalar;
	self.m_data[ 12 ] += scalar;
	self.m_data[ 13 ] += scalar;
	self.m_data[ 14 ] += scalar;
	self.m_data[ 15 ] += scalar;
	return self;
}

template < typename T >
constexpr Matrix4x4< T > Matrix4x4< T >::operator-( const Matrix4x4< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix4x4{ self.m_data[ 0 ] - other.m_data[ 0 ],
					  self.m_data[ 1 ] - other.m_data[ 1 ],
					  self.m_data[ 2 ] - other.m_data[ 2 ],
					  self.m_data[ 3 ] - other.m_data[ 3 ],
					  self.m_data[ 4 ] - other.m_data[ 4 ],
					  self.m_data[ 5 ] - other.m_data[ 5 ],
					  self.m_data[ 6 ] - other.m_data[ 6 ],
					  self.m_data[ 7 ] - other.m_data[ 7 ],
					  self.m_data[ 8 ] - other.m_data[ 8 ],
					  self.m_data[ 9 ] - other.m_data[ 9 ],
					  self.m_data[ 10 ] - other.m_data[ 10 ],
					  self.m_data[ 11 ] - other.m_data[ 11 ],
					  self.m_data[ 12 ] - other.m_data[ 12 ],
					  self.m_data[ 13 ] - other.m_data[ 13 ],
					  self.m_data[ 14 ] - other.m_data[ 14 ],
					  self.m_data[ 15 ] - other.m_data[ 15 ] };
}

template < typename T >
constexpr Matrix4x4< T > Matrix4x4< T >::operator*( const Matrix4x4< T >& other ) const noexcept
{
	const auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& mdata = other.m_data;
	return Matrix4x4{
		// First row
		sdata[ 0 ] * mdata[ 0 ] + sdata[ 1 ] * mdata[ 4 ] + sdata[ 2 ] * mdata[ 8 ] + sdata[ 3 ] * mdata[ 12 ],
		sdata[ 0 ] * mdata[ 1 ] + sdata[ 1 ] * mdata[ 5 ] + sdata[ 2 ] * mdata[ 9 ] + sdata[ 3 ] * mdata[ 13 ],
		sdata[ 0 ] * mdata[ 2 ] + sdata[ 1 ] * mdata[ 6 ] + sdata[ 2 ] * mdata[ 10 ] + sdata[ 3 ] * mdata[ 14 ],
		sdata[ 0 ] * mdata[ 3 ] + sdata[ 1 ] * mdata[ 7 ] + sdata[ 2 ] * mdata[ 11 ] + sdata[ 3 ] * mdata[ 15 ],

		// Second row
		sdata[ 4 ] * mdata[ 0 ] + sdata[ 5 ] * mdata[ 4 ] + sdata[ 6 ] * mdata[ 8 ] + sdata[ 7 ] * mdata[ 12 ],
		sdata[ 4 ] * mdata[ 1 ] + sdata[ 5 ] * mdata[ 5 ] + sdata[ 6 ] * mdata[ 9 ] + sdata[ 7 ] * mdata[ 13 ],
		sdata[ 4 ] * mdata[ 2 ] + sdata[ 5 ] * mdata[ 6 ] + sdata[ 6 ] * mdata[ 10 ] + sdata[ 7 ] * mdata[ 14 ],
		sdata[ 4 ] * mdata[ 3 ] + sdata[ 5 ] * mdata[ 7 ] + sdata[ 6 ] * mdata[ 11 ] + sdata[ 7 ] * mdata[ 15 ],

		// Third row
		sdata[ 8 ] * mdata[ 0 ] + sdata[ 9 ] * mdata[ 4 ] + sdata[ 10 ] * mdata[ 8 ] + sdata[ 11 ] * mdata[ 12 ],
		sdata[ 8 ] * mdata[ 1 ] + sdata[ 9 ] * mdata[ 5 ] + sdata[ 10 ] * mdata[ 9 ] + sdata[ 11 ] * mdata[ 13 ],
		sdata[ 8 ] * mdata[ 2 ] + sdata[ 9 ] * mdata[ 6 ] + sdata[ 10 ] * mdata[ 10 ] + sdata[ 11 ] * mdata[ 14 ],
		sdata[ 8 ] * mdata[ 3 ] + sdata[ 9 ] * mdata[ 7 ] + sdata[ 10 ] * mdata[ 11 ] + sdata[ 11 ] * mdata[ 15 ],

		// Fourth row
		sdata[ 12 ] * mdata[ 0 ] + sdata[ 13 ] * mdata[ 4 ] + sdata[ 14 ] * mdata[ 8 ] + sdata[ 15 ] * mdata[ 12 ],
		sdata[ 12 ] * mdata[ 1 ] + sdata[ 13 ] * mdata[ 5 ] + sdata[ 14 ] * mdata[ 9 ] + sdata[ 15 ] * mdata[ 13 ],
		sdata[ 12 ] * mdata[ 2 ] + sdata[ 13 ] * mdata[ 6 ] + sdata[ 14 ] * mdata[ 10 ] + sdata[ 15 ] * mdata[ 14 ],
		sdata[ 12 ] * mdata[ 3 ] + sdata[ 13 ] * mdata[ 7 ] + sdata[ 14 ] * mdata[ 11 ] + sdata[ 15 ] * mdata[ 15 ] };
}

template < typename T >
constexpr Matrix4x4< T >& Matrix4x4< T >::operator*=( const Matrix4x4< T >& other ) noexcept
{
	auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& mdata = other.m_data;

	// Temporary variables to hold the results
	const T m0 = sdata[ 0 ] * mdata[ 0 ] + sdata[ 1 ] * mdata[ 4 ] + sdata[ 2 ] * mdata[ 8 ] + sdata[ 3 ] * mdata[ 12 ];
	const T m1 = sdata[ 0 ] * mdata[ 1 ] + sdata[ 1 ] * mdata[ 5 ] + sdata[ 2 ] * mdata[ 9 ] + sdata[ 3 ] * mdata[ 13 ];
	const T m2 =
		sdata[ 0 ] * mdata[ 2 ] + sdata[ 1 ] * mdata[ 6 ] + sdata[ 2 ] * mdata[ 10 ] + sdata[ 3 ] * mdata[ 14 ];
	const T m3 =
		sdata[ 0 ] * mdata[ 3 ] + sdata[ 1 ] * mdata[ 7 ] + sdata[ 2 ] * mdata[ 11 ] + sdata[ 3 ] * mdata[ 15 ];

	const T m4 = sdata[ 4 ] * mdata[ 0 ] + sdata[ 5 ] * mdata[ 4 ] + sdata[ 6 ] * mdata[ 8 ] + sdata[ 7 ] * mdata[ 12 ];
	const T m5 = sdata[ 4 ] * mdata[ 1 ] + sdata[ 5 ] * mdata[ 5 ] + sdata[ 6 ] * mdata[ 9 ] + sdata[ 7 ] * mdata[ 13 ];
	const T m6 =
		sdata[ 4 ] * mdata[ 2 ] + sdata[ 5 ] * mdata[ 6 ] + sdata[ 6 ] * mdata[ 10 ] + sdata[ 7 ] * mdata[ 14 ];
	const T m7 =
		sdata[ 4 ] * mdata[ 3 ] + sdata[ 5 ] * mdata[ 7 ] + sdata[ 6 ] * mdata[ 11 ] + sdata[ 7 ] * mdata[ 15 ];

	const T m8 =
		sdata[ 8 ] * mdata[ 0 ] + sdata[ 9 ] * mdata[ 4 ] + sdata[ 10 ] * mdata[ 8 ] + sdata[ 11 ] * mdata[ 12 ];
	const T m9 =
		sdata[ 8 ] * mdata[ 1 ] + sdata[ 9 ] * mdata[ 5 ] + sdata[ 10 ] * mdata[ 9 ] + sdata[ 11 ] * mdata[ 13 ];
	const T m10 =
		sdata[ 8 ] * mdata[ 2 ] + sdata[ 9 ] * mdata[ 6 ] + sdata[ 10 ] * mdata[ 10 ] + sdata[ 11 ] * mdata[ 14 ];
	const T m11 =
		sdata[ 8 ] * mdata[ 3 ] + sdata[ 9 ] * mdata[ 7 ] + sdata[ 10 ] * mdata[ 11 ] + sdata[ 11 ] * mdata[ 15 ];

	const T m12 =
		sdata[ 12 ] * mdata[ 0 ] + sdata[ 13 ] * mdata[ 4 ] + sdata[ 14 ] * mdata[ 8 ] + sdata[ 15 ] * mdata[ 12 ];
	const T m13 =
		sdata[ 12 ] * mdata[ 1 ] + sdata[ 13 ] * mdata[ 5 ] + sdata[ 14 ] * mdata[ 9 ] + sdata[ 15 ] * mdata[ 13 ];
	const T m14 =
		sdata[ 12 ] * mdata[ 2 ] + sdata[ 13 ] * mdata[ 6 ] + sdata[ 14 ] * mdata[ 10 ] + sdata[ 15 ] * mdata[ 14 ];
	const T m15 =
		sdata[ 12 ] * mdata[ 3 ] + sdata[ 13 ] * mdata[ 7 ] + sdata[ 14 ] * mdata[ 11 ] + sdata[ 15 ] * mdata[ 15 ];

	// Assign the results back to self
	self.m_data[ 0 ] = m0;
	self.m_data[ 1 ] = m1;
	self.m_data[ 2 ] = m2;
	self.m_data[ 3 ] = m3;
	self.m_data[ 4 ] = m4;
	self.m_data[ 5 ] = m5;
	self.m_data[ 6 ] = m6;
	self.m_data[ 7 ] = m7;
	self.m_data[ 8 ] = m8;
	self.m_data[ 9 ] = m9;
	self.m_data[ 10 ] = m10;
	self.m_data[ 11 ] = m11;
	self.m_data[ 12 ] = m12;
	self.m_data[ 13 ] = m13;
	self.m_data[ 14 ] = m14;
	self.m_data[ 15 ] = m15;

	return self;
}

template < typename T >
constexpr Matrix4x4< T > Matrix4x4< T >::operator*( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix4x4{ self.m_data[ 0 ] * scalar,
					  self.m_data[ 1 ] * scalar,
					  self.m_data[ 2 ] * scalar,
					  self.m_data[ 3 ] * scalar,
					  self.m_data[ 4 ] * scalar,
					  self.m_data[ 5 ] * scalar,
					  self.m_data[ 6 ] * scalar,
					  self.m_data[ 7 ] * scalar,
					  self.m_data[ 8 ] * scalar,
					  self.m_data[ 9 ] * scalar,
					  self.m_data[ 10 ] * scalar,
					  self.m_data[ 11 ] * scalar,
					  self.m_data[ 12 ] * scalar,
					  self.m_data[ 13 ] * scalar,
					  self.m_data[ 14 ] * scalar,
					  self.m_data[ 15 ] * scalar };
}

template < typename T >
constexpr Matrix4x4< T >& Matrix4x4< T >::operator*=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] *= scalar;
	self.m_data[ 1 ] *= scalar;
	self.m_data[ 2 ] *= scalar;
	self.m_data[ 3 ] *= scalar;
	self.m_data[ 4 ] *= scalar;
	self.m_data[ 5 ] *= scalar;
	self.m_data[ 6 ] *= scalar;
	self.m_data[ 7 ] *= scalar;
	self.m_data[ 8 ] *= scalar;
	self.m_data[ 9 ] *= scalar;
	self.m_data[ 10 ] *= scalar;
	self.m_data[ 11 ] *= scalar;
	self.m_data[ 12 ] *= scalar;
	self.m_data[ 13 ] *= scalar;
	self.m_data[ 14 ] *= scalar;
	self.m_data[ 15 ] *= scalar;
	return self;
}

} // namespace pbl::math
#endif // PBL_MATH_MATRIX_4X4_IPP__
