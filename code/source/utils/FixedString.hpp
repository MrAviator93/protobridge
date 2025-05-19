#ifndef PBL_UTILS_FIXED_STRING_HPP__
#define PBL_UTILS_FIXED_STRING_HPP__

// C++
#include <array>
#include <string>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <system_error>

namespace pbl::utils
{

/**
 * @brief A compile-time, fixed-size string implementation.
 * 
 * `FixedString` is a utility that facilitates the use of strings within
 * template parameters It stores a string in a way that its value can be
 * retrieved at compile-time, making it suitable for scenarios where constant
 * expressions are required.
 * 
 * Example usage:
 * @code
 * template <FixedString Name>
 * class SomeTemplatedClass {
 *     // Access the static string as a compile-time constant
 *     constexpr auto name() const { return Name; }
 * };
 * 
 * // Instantiate with a fixed string
 * SomeTemplatedClass<"example_fixed_string"> example;
 * auto staticName = example.name();  // staticName will have "example_static_string"
 * @endcode
 * 
 * @tparam N The size of the string, not including the null terminator.
 */
template < std::size_t N >
struct FixedString final
{
	/**
     * @brief Constructs a `FixedString` from a traditional C-string literal.
	 * 
	 * @note Not marked as explicit to allow implicit conversions from string
	 * literals to const char[N].
     * 
     * @param str The string literal to initialize the `FixedString`. Must be null-terminated.
     */
	constexpr FixedString( const char ( &str )[ N + 1 ] )
		: chars{}
	{
		std::copy_n( str, N + 1, chars );
	}

	/// Retrieves the size of the stored string (excluding the null terminator).
	[[nodiscard]] static constexpr auto size() noexcept { return N; }

	/// Allows implicit conversion of the instance to a const char* pointing to the stored string.
	[[nodiscard]] constexpr operator const char*() const noexcept { return chars; }

	/// Defaulted three-way comparison operator (spaceship operator).
	[[nodiscard]] constexpr auto operator<=>( const FixedString& ) const = default;

	/// Compares the equality of two `FixedString` instances, checking if they contain the same characters.
	[[nodiscard]] constexpr bool operator==( const FixedString& str ) const
	{
		return std::equal( chars, chars + N + 1, str.chars, str.chars + N + 1 );
	}

	/// Compares the in-equality of two `FixedString` instances, checking if they contain the same characters.
	[[nodiscard]] constexpr bool operator!=( const FixedString& str ) const { return !operator==( str ); }

	/// Compares the stored string with a C-string for equality.
	[[nodiscard]] constexpr bool operator==( const char* pStr ) const
	{
		return std::equal( chars, chars + N + 1, pStr, pStr + std::strlen( pStr ) + 1 );
	}

	/// Compares the stored string with a C-string for in-equality.
	[[nodiscard]] constexpr bool operator!=( const char* pStr ) const { return !operator==( pStr ); }

	/// Compares the stored string with a std::string instance for equality.
	[[nodiscard]] constexpr bool operator==( const std::string& str ) const
	{
		return std::equal( chars, chars + N + 1, str.c_str(), str.c_str() + str.length() + 1 );
	}

	/// Compares the stored string with a std::string instance for in-equality.
	[[nodiscard]] constexpr bool operator!=( const std::string& str ) const { return !operator==( str ); }

	/// Compares the equality of two `FixedString` instances of potentially different sizes.
	template < std::size_t M >
	[[nodiscard]] constexpr bool operator==( const FixedString< M >& str ) const
	{
		// If the sizes of the two strings are different, they cannot be equal
		if( N != M )
		{
			return false;
		}

		return std::equal( chars, chars + M + 1, str.chars, str.chars + M + 1 );
	}

	/// Compares the inequality of two `FixedString` instances of potentially different sizes.
	template < std::size_t M >
	[[nodiscard]] constexpr bool operator!=( const FixedString< M >& str ) const
	{
		return !operator==( str );
	}

	/// Allows implicit conversion of the instance to a std::string_view.
	[[nodiscard]] constexpr operator std::string_view() const noexcept { return std::string_view{ chars, N }; }

	/// Returns an instance of std::string_view containing chars.
	[[nodiscard]] constexpr std::string_view view() const noexcept { return std::string_view{ chars, N }; }

	/// Returns an instance of std::string containing chars (makes a full std::string copy).
	[[nodiscard]] constexpr std::string_view string() const noexcept { return std::string( &chars, N ); }

	/// Returns whether the FixedString is empty.
	[[nodiscard]] constexpr bool empty() const noexcept { return N == 0; }

	/// Returns an iterator pointing to the beginning of the stored string.
	[[nodiscard]] constexpr const char* begin() const noexcept { return chars; }

	/// Returns an iterator pointing to the end of the stored string.
	[[nodiscard]] constexpr const char* end() const noexcept { return chars + N; }

	/// Returns the first character of the stored string.
	[[nodiscard]] constexpr char front() const noexcept { return chars[ 0 ]; }

	/// Returns the last character of the stored string.
	[[nodiscard]] constexpr char back() const noexcept { return chars[ N - 1 ]; }

	template < std::size_t Index >
		requires( Index < N )
	[[nodiscard]] constexpr char at() const noexcept
	{
		return chars[ Index ];
	}

	/// Provides bounds-checked access to the specified character by index.
	[[nodiscard]] constexpr char at( std::size_t index ) const
	{
		if( index >= N )
		{
			throw std::out_of_range( "FixedString index out of range" );
		}

		return chars[ index ];
	}

	/// Provides bounds-checked access to the specified character by index, using an error_code for error reporting.
	[[nodiscard]] constexpr char at( std::size_t index, std::error_code& ec ) const noexcept
	{
		if( index >= N )
		{
			ec = std::make_error_code( std::errc::result_out_of_range );

			// Return a default value to indicate error
			return {};
		}

		ec.clear();
		return chars[ index ];
	}

	/// Accesses the specified character by index (performs bounds check).
	[[nodiscard]] constexpr char operator[]( const std::size_t index ) const { return at( index ); }

	/// Accesses the specified character by index, returns default on error via std::error_code.
	[[nodiscard]] constexpr char operator[]( std::size_t index, std::error_code& ec ) const noexcept
	{
		return at( index, ec );
	}

	char chars[ N + 1 ];
}; // namespace pbl::utils

/// Deduction guide to allow the creation of a FixedString without specifying the size explicitly.
template < std::size_t N >
FixedString( const char ( &str )[ N ] ) -> FixedString< N - 1 >;

} // namespace pbl::utils
#endif // PBL_UTILS_FIXED_STRING_HPP__
