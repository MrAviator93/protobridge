#ifndef PBL_UTILS_RESULT_HPP__
#define PBL_UTILS_RESULT_HPP__

#include "ErrorCode.hpp"

// C++
#include <expected>

namespace pbl::utils
{

template < typename T >
using Result = std::expected< T, Error >;

} // namespace pbl::utils
#endif // PBL_UTILS_RESULT_HPP__
