#ifndef PBL_UTILS_RANDOM_GENERATOR_HPP__
#define PBL_UTILS_RANDOM_GENERATOR_HPP__

// C++
#include <array>
#include <chrono>
#include <random>

#include <algorithm>

namespace pbl::utils
{

namespace detail
{

template < typename T >
struct DistributionSelector;

// Specialization for integers
template < typename T >
	requires std::is_integral_v< T >
struct DistributionSelector< T >
{
	using Type = std::uniform_int_distribution< T >;
};

// Specialization for floating-point numbers
template < typename T >
	requires std::is_floating_point_v< T >
struct DistributionSelector< T >
{
	using Type = std::uniform_real_distribution< T >;
};

template < typename T >
using DistributionSelectorT = DistributionSelector< T >::Type;

} // namespace detail

/**
 * @class RandomGenerator
 * @brief A versatile random number generator that works with various data types.
 * 
 * This class template is used for generating random numbers within a specified
 * range. It utilizes the Mersenne Twister algorithm for random number generation.
 * The class is flexible and works with different numeric types.
 *
 * @tparam T The type of data for which the random numbers are generated.
 * @tparam void This is a default non-type parameter used for template specialization.
 */
template < typename T, typename = void >
class RandomGenerator final
{
public:
	/**
     * @brief Constructs a random number generator with a range.
     * 
     * The constructor initializes the random number generator and sets the range of
	* generated numbers. It uses a high-quality seed generated from a random device.
     *
     * @param a The lower bound of the random number range.
     * @param b The upper bound of the random number range.
     * 
     * 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may
	*          lead to undefined behavior or runtime errors.
     */
	RandomGenerator( const T& a, const T& b )
		: m_a{ a }
		, m_b{ b }
	{
		std::random_device rd{};
		std::array< int, std::mt19937::state_size > seedData{};
		std::generate( std::begin( seedData ), std::end( seedData ), std::ref( rd ) );
		std::seed_seq seq( std::begin( seedData ), std::end( seedData ) );
		std::mt19937 generator{ seq };
		std::swap( m_generator, generator );
	}

	/**
     * @brief Constructs a random time duration generator with a specific seed and range.
     * 
     * This constructor allows for seeding the random number generator for reproducibility.
	* It is useful in scenarios where the sequence of random time durations needs to be
	* repeatable across different program runs.
     *
     * @param seed The seed value for the random number generator.
     * @param a The lower bound of the random time duration range.
     * @param b The upper bound of the random time duration range.
     * 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may lead to
     *          undefined behavior or runtime errors.
     */
	RandomGenerator( std::mt19937::result_type seed, const T& a, const T& b )
		: m_a{ a }
		, m_b{ b }
	{
		std::random_device rd{};
		std::array< int, std::mt19937::state_size > seedData{};
		std::generate( std::begin( seedData ), std::end( seedData ), std::ref( rd ) );
		std::seed_seq seq( std::begin( seedData ), std::end( seedData ) );
		std::mt19937 generator{ seq };
		std::swap( m_generator, generator );
		m_generator.seed( seed );
	}

	/// Generates a random number within the pre-set range.
	[[nodiscard]] T operator()() { return operator()( m_a, m_b ); }

	/**
     * @brief Generates a random number within a specified range.
     * 
     * This function call operator allows specifying a new range for each call, overriding
	* the range set during construction.
     *
     * @param a The lower bound of the random number range.
     * @param b The upper bound of the random number range.
     * @return T A random number of type T.
     * 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may lead to 
     *          undefined behavior or runtime errors.
     */
	[[nodiscard]] T operator()( const T& a, const T& b )
	{
		detail::DistributionSelectorT< T > distribution{ a, b };
		return distribution( m_generator );
	}

private:
	const T m_a;
	const T m_b;
	std::mt19937 m_generator;
};

/**
 * @class RandomGenerator
 * @brief Specialization of RandomGenerator for generating random durations.
 * 
 * This class template specialization is used for generating random time durations. It is
 * particularly useful for scenarios where random time intervals are required, and it works
 * with various chrono duration types.
 *
 * @tparam Rep A numeric type representing the number of ticks in the duration.
 * @tparam Period A std::ratio representing the tick period (i.e., the number of seconds per tick).
 */
template < typename Rep, typename Period >
class RandomGenerator< Rep, std::chrono::duration< Rep, Period > > final
{
public:
	using TimeType = std::chrono::duration< Rep, Period >;

	/**
     * @brief Constructs a random time duration generator with a range.
     * 
     * The constructor initializes the random number generator and sets the range of generated time durations.
     *
     * @param a The lower bound of the random time duration range.
     * @param b The upper bound of the random time duration range.
	* 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may lead to 
     *          undefined behavior or runtime errors.
     */
	RandomGenerator( const Rep& a, const Rep& b )
		: m_randomGenerator{ a, b }
	{ }

	/**
     * @brief Constructs a random time duration generator with a specific seed and range.
     * 
     * This constructor allows for seeding the random number generator for reproducibility.
	* It is useful in scenarios where the sequence of random time durations needs to be 
	* repeatable across different program runs.
     *
     * @param seed The seed value for the random number generator.
     * @param a The lower bound of the random time duration range.
     * @param b The upper bound of the random time duration range.
	* 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may lead to 
     *          undefined behavior or runtime errors.
     */
	RandomGenerator( std::mt19937::result_type seed, const Rep& a, const Rep& b )
		: m_randomGenerator{ seed, a, b }
	{ }

	/// Generates a random time duration within the pre-set range.
	[[nodiscard]] TimeType operator()() { return TimeType{ m_randomGenerator() }; }

	/**
     * @brief Generates a random time duration within the pre-set range.
     * 
     * This function call operator allows specifying a new range for each call, overriding
	* the range set during construction.
     *
     * @param a The lower bound of the random duration range.
     * @param b The upper bound of the random duration range.
     * @return T A random duration of type T.
     * 
     * @warning It's crucial that 'a' is less than 'b'. Failing to ensure this may lead to 
     *          undefined behavior or runtime errors.
     */
	[[nodiscard]] TimeType operator()( const Rep& a, const Rep& b ) { return TimeType{ m_randomGenerator( a, b ) }; }

private:
	RandomGenerator< Rep > m_randomGenerator;
};

} // namespace pbl::utils
#endif // PBL_UTILS_RANDOM_GENERATOR_HPP__
