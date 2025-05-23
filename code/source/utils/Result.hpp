#ifndef PBL_UTILS_RESULT_HPP__
#define PBL_UTILS_RESULT_HPP__

#include "Error.hpp"

// C++
#include <expected>

namespace pbl::utils
{

template < typename T >
using Result = std::expected< T, Error >;

namespace detail
{

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

struct MakeErrorFromInstanceHelper
{
	template < typename T >
	operator Result< T >() const
	{
		return std::unexpected( error );
	}

	Error error;
};

struct MakeSuccessHelper
{
	template < typename T = void >
	operator Result< T >() const
	{
		static_assert( std::is_void_v< T >, "Use MakeSuccess(value) for non-void types." );
		return {};
	}
};

template < typename T >
struct MakeSuccessFromValueHelper
{
	T value;

	operator Result< T >() && { return Result< T >{ std::move( value ) }; }
};

} // namespace detail

template < typename T >
[[nodiscard]] inline Result< T > MakeError( ErrorCode code, std::optional< std::string > msg = std::nullopt )
{
	return std::unexpected( Error( code, std::move( msg ) ) );
}

/// Creates a Result<T> from an error code + optional message
[[nodiscard]] inline auto MakeError( ErrorCode code, std::optional< std::string > message = std::nullopt )
{
	return detail::MakeErrorHelper{ code, std::move( message ) };
}

/// Creates a Result<T> from a pre-existing Error object
[[nodiscard]] inline auto MakeError( const Error& error )
{
	return detail::MakeErrorFromInstanceHelper{ error };
}

/// For Result<void>
[[nodiscard]] inline auto MakeSuccess()
{
	return detail::MakeSuccessHelper{};
}

/// For Result<T> (non-void)
template < typename T >
[[nodiscard]] inline auto MakeSuccess( T&& value )
{
	return detail::MakeSuccessFromValueHelper< std::decay_t< T > >{ std::forward< T >( value ) };
}

} // namespace pbl::utils
#endif // PBL_UTILS_RESULT_HPP__
