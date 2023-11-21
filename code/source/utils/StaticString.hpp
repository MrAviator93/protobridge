
#ifndef I2C_UTILS_STATIC_STRING_HPP__
#define I2C_UTILS_STATIC_STRING_HPP__

#include <array>
#include <string>
#include <cstring>
#include <algorithm>

namespace I2C
{

/**
 * @brief A compile-time, fixed-size string implementation.
 * 
 * `StaticString` is a utility that facilitates the use of strings within
 * template parameters It stores a string in a way that its value can be
 * retrieved at compile-time, making it suitable for scenarios where constant
 * expressions are required.
 * 
 * Example usage:
 * @code
 * template <StaticString Name>
 * class SomeTemplatedClass {
 *     // Access the static string as a compile-time constant
 *     constexpr auto name() const { return Name; }
 * };
 * 
 * // Instantiate with a static string
 * SomeTemplatedClass<"example_static_string"> example;
 * auto staticName = example.name();  // staticName will have "example_static_string"
 * @endcode
 * 
 * @todo Consider renaming to FixedString for clarity, indicating immutability and fixed size.
 * @todo Evaluate the possibility of using std::array<char, N + 1> for storage to potentially
 *       simplify certain operations.
 * 
 * @tparam N The size of the string, not including the null terminator.
 */
template < std::size_t N >
struct StaticString final
{
	/**
     * @brief Constructs a `StaticString` from a traditional C-string literal.
     * 
     * @param str The string literal to initialize the `StaticString`. Must be null-terminated.
     */
	constexpr StaticString( const char ( &str )[ N + 1 ] ) { std::copy_n( str, N + 1, chars ); }

	/// Retrieves the size of the stored string (excluding the null terminator).
	[[nodiscard]] constexpr auto size() const noexcept { return N; }

	/// Allows implicit conversion of the instance to a const char* pointing to the stored string.
	[[nodiscard]] constexpr operator const char*() const noexcept { return chars; }

	/// Defaulted three-way comparison operator (spaceship operator).
	[[nodiscard]] constexpr auto operator<=>( const StaticString& ) const = default;

	/// Compares the equality of two `StaticString` instances, checking if they contain the same characters.
	[[nodiscard]] constexpr bool operator==( const StaticString& str ) const
	{
		return std::equal( chars, chars + N + 1, str.chars, str.chars + N + 1 );
	}

	/// Compares the stored string with a C-string for equality.
	[[nodiscard]] constexpr bool operator==( const char* pStr ) const
	{
		return std::equal( chars, chars + N + 1, pStr, pStr + std::strlen( pStr ) + 1 );
	}

	/// Compares the stored string with a std::string instance for equality.
	[[nodiscard]] constexpr bool operator==( const std::string& str ) const
	{
		return std::equal( chars, chars + N + 1, str.c_str(), str.c_str() + str.length() + 1 );
	}

	char chars[ N + 1 ];
};

// Deduction guide to allow the creation of a StaticString without specifying the size explicitly.
template < std::size_t N >
StaticString( const char ( &str )[ N ] ) -> StaticString< N - 1 >;

} // namespace I2C
#endif // I2C_UTILS_STATIC_STRING_HPP__
