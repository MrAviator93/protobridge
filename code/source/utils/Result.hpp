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

} // namespace detail

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

} // namespace pbl::utils
#endif // PBL_UTILS_RESULT_HPP__
