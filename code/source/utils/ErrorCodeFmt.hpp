#ifndef PBL_UTILS_ERROR_CODE_FMT_HPP__
#define PBL_UTILS_ERROR_CODE_FMT_HPP__

#include "ErrorCode.hpp"

// C++
#include <format>

namespace std
{
template <>
struct formatter< pbl::utils::ErrorCode > : formatter< std::string_view >
{
	auto format( pbl::utils::ErrorCode code, format_context& ctx ) const
	{
		return formatter< std::string_view >::format( pbl::utils::toStringView( code ), ctx );
	}
};
} // namespace std

#endif // PBL_UTILS_ERROR_CODE_FMT_HPP__
