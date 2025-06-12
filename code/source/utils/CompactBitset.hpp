#ifndef PBL_COMPACT_BITSET_HPP__
#define PBL_COMPACT_BITSET_HPP__

// C++
#include <array>
#include <numeric>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>

namespace pbl::utils
{

/**
 * @brief A space-efficient representation of a fixed-size set of bits.
 * 
 * The `CompactBitset` class template provides functionality similar to 
 * the standard `std::bitset`. However, it optimizes storage space for 
 * scenarios where the bit count is low, particularly important in 
 * memory-constrained environments like those found in microcontrollers 
 * and embedded systems. Unlike `std::bitset`, which typically has a 
 * minimum size of 8 bytes, `CompactBitset` adjusts its storage size 
 * based on the template parameter `N`, ensuring minimal overhead for 
 * smaller bit sets.
 *
 * @note The first bit starts at position 0, i.e. bit 0.
 * 
 * @tparam N The number of bits that the `CompactBitset` is to hold. 
 *           This is a compile-time constant.
 */
template < std::size_t N >
class CompactBitset final
{
	static_assert( N > 0, "The bitset size must be greater than zero." );

	// Determine whether we need an array or a single byte
	static constexpr bool kIsArray = N > 8;

public:
	/// Default constructor
	constexpr CompactBitset() noexcept = default;

	/// Returns the number of bits the bitset holds
	[[nodiscard]] static constexpr auto size() noexcept { return N; }

	/// Returns the number of bytes that are used for storing the bitset
	[[nodiscard]] static constexpr std::size_t storageSize() noexcept;

	/// Sets the value of the bit at the specified position
	constexpr void set( std::size_t bit, bool value = true ) noexcept;

	/// Checks if the bit at the specified position is set (i.e., whether it is true)
	[[nodiscard]] constexpr bool test( std::size_t bit ) noexcept;

	/// Resets all bits to false
	constexpr void reset() noexcept;

	/// Resets the bit at the specified position to false
	constexpr void reset( std::size_t bit ) noexcept;

	/// Flips the value of the bit at the specified position
	constexpr void flip( std::size_t bit ) noexcept;

	/// Checks if all bits are set to true
	[[nodiscard]] constexpr bool all() const noexcept;

	/// Checks if any bit is set to true
	[[nodiscard]] constexpr bool any() const noexcept;

	/// Checks if none of the bits are set to true
	[[nodiscard]] constexpr bool none() const noexcept { return !any(); }

	/// Returns the number of bits set to true
	[[nodiscard]] constexpr std::size_t count() const noexcept;

	/// Compares the equality of two bitsets
	[[nodiscard]] bool operator==( const CompactBitset& other ) const noexcept;

private:
	// Helper type to define the array type only if needed
	template < typename T, std::size_t Size, bool UseArray >
	struct StorageTypeHelper;

	// Specialization for single byte
	template < typename T, std::size_t Size >
	struct StorageTypeHelper< T, Size, false >
	{
		using Type = T;
	};

	// Specialization for array
	template < typename T, std::size_t Size >
	struct StorageTypeHelper< T, Size, true >
	{
		// +7 to round up to the correct number of bytes
		using Type = std::array< T, ( Size + 7 ) / 8 >;
	};

	using StorageType = typename StorageTypeHelper< std::uint8_t, N, kIsArray >::Type;

private:
	/// Counts the number of set bits (1s) in a single byte
	static constexpr std::size_t countBitsInByte( std::uint8_t byte ) noexcept;

	/// Checks if the given bit index is within the valid range of the bitset
	static constexpr bool bitWithinBounds( std::size_t bit ) noexcept { return bit < N; }

private:
	StorageType m_storage{};
};

} // namespace pbl::utils

#include "CompactBitset.ipp"

#endif // PBL_COMPACT_BITSET_HPP__
