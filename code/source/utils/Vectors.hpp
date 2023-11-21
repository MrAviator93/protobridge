
#ifndef I2C_UTILS_VECTORS_HPP__
#define I2C_UTILS_VECTORS_HPP__

#include "Math.hpp"

namespace I2C
{

template < typename T >
struct Vector2
{
	T x{};
	T y{};
};

template < typename T >
struct Vector3
{
	T x{};
	T y{};
	T z{};
};

template < typename T >
struct Vector4
{
	T x{};
	T y{};
	T z{};
	T w{};
};

using Vector2f = Vector2< float >;
using Vector2d = Vector2< double >;

using Vector3f = Vector3< float >;
using Vector3d = Vector3< double >;

using Vector4f = Vector4< float >;
using Vector4d = Vector4< double >;

} // namespace I2C
#endif // I2C_UTILS_VECTORS_HPP__