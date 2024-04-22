#ifndef PBL_MATH_PREDICTION_MODELS_HPP__
#define PBL_MATH_PREDICTION_MODELS_HPP__

#include <utils/RandomGenerator.hpp>

// C++
#include <cmath>
#include <tuple>
#include <utility>
#include <concepts>
#include <type_traits>

namespace pbl::math
{

template < typename T >
struct PredictionModelBase
{
	using ValueType = T;

	[[nodiscard]] constexpr T operator()() const noexcept { return 0; }

protected:
	PredictionModelBase() = default;
};

/// Model that always returns zero
template < typename T >
struct EmptyModel final : PredictionModelBase< T >
{
	EmptyModel() = default;
};

// TODO: Add to all models 1D suffix, as those are all 1D models!

/// Predicts the next state based on a constant rate of motion
template < typename T >
struct LinearMotionModel final : PredictionModelBase< T >
{
	LinearMotionModel() = default;

	[[nodiscard]] constexpr T operator()( T dt, T rate ) const noexcept { return dt * rate; }
};

/// Model for predicting position in a circular motion
template < typename T >
struct CircularMotionModel final : PredictionModelBase< T >
{
	CircularMotionModel() = default;

	[[nodiscard]] constexpr T operator()( T dt, T speed, T radius ) const noexcept
	{
		return radius * std::sin( speed * dt / radius ); // Circular motion equation
	}
};

/// Randomly walks around the current state
template < typename T >
struct RandomWalkModel final : PredictionModelBase< T >
{
	template < typename... Args >
	RandomWalkModel( Args&&... args )
		: m_generator{ std::forward< Args >( args )... }
	{ }

	[[nodiscard]] T operator()( T currentState, T processNoise )
	{
		return currentState + m_generator( 0, processNoise );
	}

private:
	utils::RandomGenerator< T > m_generator;
};

/// Model for systems affected by an external force
template < typename T >
struct ExternalForceModel final : PredictionModelBase< T >
{
	ExternalForceModel() = default;

	[[nodiscard]] constexpr T operator()( T dt, T rate, T force, T mass ) const noexcept
	{
		// Newton's second law
		T acceleration = force / mass;

		// Equation of motion with external force
		return dt * rate + 0.5 * acceleration * dt * dt;
	}
};

/// Forward declaration of the CheckInvocableRecursive structure
template < typename T, typename Callable, std::size_t N = 0 >
struct CheckInvocableRecursive;

/**
 * @brief A helper structure used to test if a callable type is invocable with 'N' arguments.
 * 
 * It creates a sequence of 'N' arguments of type 'T' and tests whether they can be used
 * to invoke the callable. It leverages std::is_invocable to perform this check.
 * 
 * @tparam T The argument type passed to the callable 
 * @tparam Callable The callable type. 
 * @tparam N The number of arguments to test invocability with.
 */
template < typename T, typename Callable, std::size_t N >
struct CheckInvocableWithNArgs
{
	// Generates a sequence based on 'N' and checks if 'Callable' can be invoked with it.
	template < std::size_t... Is >
	static constexpr bool test( std::index_sequence< Is... > )
	{
		return std::is_invocable_v< Callable, decltype( ( void( Is ), std::declval< T >() ) )... >;
	}

	static constexpr bool value = test( std::make_index_sequence< N >{} );
};

/**
 * @brief The main structure that performs a recursive check to test if 'Callable' is invocable
 * with increasing numbers of 'T' type arguments, up to a maximum of 100.
 * 
 * @tparam T The argument type passed to the callable.
 * @tparam Callable The callable type.
 * @tparam N The current number of arguments being checked.
 */
template < typename T, typename Callable, std::size_t N >
struct CheckInvocableRecursive
{
	static constexpr bool value = CheckInvocableWithNArgs< T, Callable, N >::value ||
								  ( N < 100 && CheckInvocableRecursive< T, Callable, N + 1 >::value );
};

/// Specialization to stop recursion when the maximum limit is reached
template < typename T, typename Callable >
struct CheckInvocableRecursive< T, Callable, 100 >
{
	// Maximum depth reached, stop recursion
	static constexpr bool value = false;
};

/// A convenience variable template for easier usage of the CheckInvocableRecursive structure.
template < typename ValueType, typename ModelType >
constexpr bool CheckInvocableRecursiveV = CheckInvocableRecursive< ValueType, ModelType >::value;

/**
 * @brief A concept that checks whether a given model type satisfies certain criteria: it must
 * have a nested 'ValueType' and be invocable with varying numbers of 'ValueType' arguments.
 * 
 * @tparam ModelType The model type being checked against the concept
 */
template < typename ModelType >
concept PredictionModelType =
	requires { typename ModelType::ValueType; } && std::is_floating_point_v< typename ModelType::ValueType > &&
	CheckInvocableRecursiveV< typename ModelType::ValueType, ModelType >;

} // namespace pbl::math
#endif // PBL_MATH_PREDICTION_MODELS_HPP__
