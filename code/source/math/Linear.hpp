
#ifndef I2C_MATH_LINEAR_HPP__
#define I2C_MATH_LINEAR_HPP__

#include "LinearBase.hpp"

namespace PBL::Math
{

template < typename T >
class Vector2 : public VectorBase< T, 2u >
{
	using Parent = VectorBase< T, 2u >;

public:
	using Parent::Parent;

	constexpr T x() const noexcept { return this->m_data[ 0 ]; }
	constexpr T y() const noexcept { return this->m_data[ 1 ]; }
};

template < typename T >
class Vector3 : public VectorBase< T, 4u > // one extra for padding
{
	using Parent = VectorBase< T, 4u >;

public:
	constexpr Vector3()
		: Parent{}
	{ }

	template < typename... Args >
		requires( sizeof...( Args ) == 3u )
	constexpr Vector3( Args&&... args ) noexcept
		: Parent{ std::forward< Args >( args )..., T{} }
	{ }

	constexpr T x() const noexcept { return this->m_data[ 0 ]; }
	constexpr T y() const noexcept { return this->m_data[ 1 ]; }
	constexpr T z() const noexcept { return this->m_data[ 2 ]; }
};

template < typename T >
class Vector4 : public VectorBase< T, 4u >
{
	using Parent = VectorBase< T, 4u >;

public:
	using Parent::Parent;

	constexpr T x() const noexcept { return this->m_data[ 0 ]; }
	constexpr T y() const noexcept { return this->m_data[ 1 ]; }
	constexpr T z() const noexcept { return this->m_data[ 2 ]; }
	constexpr T w() const noexcept { return this->m_data[ 3 ]; }
};

// Matrixes only operate with Row or Column Vector types.

template < typename T >
class RowVector : public T
{
public:
	using T::T;

	// TODO
};

// TODO: Redefine behaviour for column vector
template < typename T >
class ColumnVector : public T
{
public:
	using T::T;

	// TODO
};

using Vector2f = Vector2< float >;
using Vector2d = Vector2< double >;
using Vector3f = Vector3< float >;
using Vector3d = Vector3< double >;
using Vector4f = Vector4< float >;
using Vector4d = Vector4< double >;

} // namespace PBL::Math
#endif // I2CMATHS_LINEAR_HPP__