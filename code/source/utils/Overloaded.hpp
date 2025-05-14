#ifndef PBL_UTILS_OVERLOADED_HPP__
#define PBL_UTILS_OVERLOADED_HPP__

namespace pbl::utils
{

template < typename... Ts >
struct Overloaded : Ts...
{
	using Ts::operator()...;
};

template < typename... Ts >
Overloaded( Ts... ) -> Overloaded< Ts... >;

} // namespace pbl::utils
#endif // PBL_UTILS_OVERLOADED_HPP__
