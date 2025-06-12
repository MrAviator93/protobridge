#ifndef PBL_UTILS_OVERLOADED_HPP__
#define PBL_UTILS_OVERLOADED_HPP__

namespace pbl::utils
{

/**
 * @brief Combines multiple callable types into one overload set.
 *
 * This struct enables clean multi-signature handling, often used in conjunction with
 * `std::visit`, command dispatching, or generic lambda-based overloading.
 *
 * ### Primary Use Cases:
 * - Visiting a `std::variant` using multiple lambdas.
 * - Creating lightweight dispatchers for custom protocols or operations.
 *
 * @tparam Ts Variadic list of callable types (e.g., lambdas or structs with `operator()`).
 *
 * ### Example 1: Variant Visiting (std::visit)
 * @code
 * std::variant<int, std::string> var = "hello";
 * std::visit(pbl::utils::Overloaded{
 *     [](int i) { std::cout << "int: " << i << '\n'; },
 *     [](const std::string& s) { std::cout << "string: " << s << '\n'; }
 * }, var);
 * @endcode
 *
 * ### Example 2: Dispatcher Invocation (advanced)
 * @code
 * auto dispatcher = pbl::utils::Overloaded{
 *     [](auto) { return "unsupported"; },
 *     [](PinRead, int pin) { return readPin(pin); },
 *     [](PinWrite, int pin, bool state) { return writePin(pin, state); }
 * };
 *
 * // Somewhere in code
 * std::invoke(dispatcher, PinRead{}, 3);  // dispatches to matching lambda
 * @endcode
 */
template < typename... Ts >
struct Overloaded : Ts...
{
	using Ts::operator()...;
};

/// Deduction guide for Overloaded to allow class template argument deduction (CTAD).
template < typename... Ts >
Overloaded( Ts... ) -> Overloaded< Ts... >;

} // namespace pbl::utils
#endif // PBL_UTILS_OVERLOADED_HPP__
