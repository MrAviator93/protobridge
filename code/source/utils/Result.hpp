#ifndef PBL_UTILS_RESULT_HPP__
#define PBL_UTILS_RESULT_HPP__

#include "Error.hpp"

// C++
#include <tuple>
#include <expected>

namespace pbl::utils
{

/**
 * @brief A result type that represents either a success (`T`) or an error (`Error`).
 *
 * This is a convenient alias for `std::expected<T, Error>` which provides
 * expressive error handling. It avoids exceptions and makes success/error
 * cases explicit in the type system.
 *
 * Example:
 * @code
 * Result<int> result = parse(std::string_view input);
 *
 * if (result) {
 *     int number = *result;
 * } else {
 *     std::cerr << "Error: " << result.error().description();
 * }
 * @endcode
 */
template < typename T >
using Result = std::expected< T, Error >;

namespace detail
{

/// Internal helper to construct a Result<T> from an ErrorCode + optional message.
struct MakeErrorHelper
{
	template < typename T >
	operator Result< T >() const
	{
		return std::unexpected( Error( code, std::move( message ) ) );
	}

	ErrorCode code;
	std::optional< std::string > message;
};

/// Internal helper to construct a Result<T> from an existing Error instance.
struct MakeErrorFromInstanceHelper
{
	template < typename T >
	operator Result< T >() const
	{
		return std::unexpected( error );
	}

	Error error;
};

/// Internal helper to create a `Result<void>` success instance.
struct MakeSuccessHelper
{
	template < typename T = void >
	operator Result< T >() const
	{
		static_assert( std::is_void_v< T >, "Use MakeSuccess(value) for non-void types." );
		return {};
	}
};

/// Internal helper to wrap a value in a `Result<T>` success.
template < typename T >
struct MakeSuccessFromValueHelper
{
	T value;

	operator Result< T >() && { return Result< T >{ std::move( value ) }; }
};

} // namespace detail

/// Creates a `Result<T>` representing an error with code and optional message.
template < typename T >
[[nodiscard]] inline Result< T > MakeError( ErrorCode code, std::optional< std::string > msg = std::nullopt )
{
	return std::unexpected( Error( code, std::move( msg ) ) );
}

/// Creates a `Result<T>` from an error code + optional message
[[nodiscard]] inline auto MakeError( ErrorCode code, std::optional< std::string > message = std::nullopt )
{
	return detail::MakeErrorHelper{ code, std::move( message ) };
}

/// Creates a `Result<T>` from a pre-existing Error object
[[nodiscard]] inline auto MakeError( const Error& error )
{
	return detail::MakeErrorFromInstanceHelper{ error };
}

/// Creates a successful `Result<void>` (used for operations with no return value).
[[nodiscard]] inline auto MakeSuccess()
{
	return detail::MakeSuccessHelper{};
}

/// Creates a successful `Result<T>` containing a given value (non-void).
template < typename T >
[[nodiscard]] inline auto MakeSuccess( T&& value )
{
	return detail::MakeSuccessFromValueHelper< std::decay_t< T > >{ std::forward< T >( value ) };
}

/// Constructs a successful `Result<T>` using in-place construction with arguments.
template < typename T, typename... Args >
[[nodiscard]] inline Result< T > MakeSuccess( std::in_place_t, Args&&... args )
{
	return Result< T >( std::in_place, std::forward< Args >( args )... );
}

} // namespace pbl::utils
#endif // PBL_UTILS_RESULT_HPP__
