// PBL
#include <utils/Bit.hpp>

// Third Party
#include <gtest/gtest.h>

// C++
#include <cstdint>
#include <type_traits>

namespace pbl::utils
{

TEST( BitTest, BitIsCorrectForUint8 )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto bitResult = ( bit< uint8_t, 1 >() );
	const auto bitResult2 = ( bit< uint8_t, 2 >() );
	const auto bitResult3 = ( bit< uint8_t, 8 >() );

	// Assert
	EXPECT_EQ( bitResult, static_cast< uint8_t >( 0x01 ) ); // 1 << 0
	EXPECT_EQ( bitResult2, static_cast< uint8_t >( 0x02 ) ); // 1 << 1
	EXPECT_EQ( bitResult3, static_cast< uint8_t >( 0x80 ) ); // 1 << 7
}

TEST( BitTest, BitIsCorrectForUint16 )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto bitResult = ( bit< uint16_t, 1 >() );
	const auto bitResult2 = ( bit< uint16_t, 9 >() );
	const auto bitResult3 = ( bit< uint16_t, 16 >() );

	// Assert
	EXPECT_EQ( bitResult, static_cast< uint16_t >( 0x0001 ) ); // 1 << 0
	EXPECT_EQ( bitResult2, static_cast< uint16_t >( 0x0100 ) ); // 1 << 8
	EXPECT_EQ( bitResult3, static_cast< uint16_t >( 0x8000 ) ); // 1 << 15
}

TEST( BitTest, BitIsCorrectForUint32 )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto bitResult = ( bit< uint32_t, 1 >() );
	const auto bitResult2 = ( bit< uint32_t, 17 >() );
	const auto bitResult3 = ( bit< uint32_t, 32 >() );

	// Assert
	EXPECT_EQ( bitResult, 0x00000001U ); // 1 << 0
	EXPECT_EQ( bitResult2, 0x00010000U ); // 1 << 16
	EXPECT_EQ( bitResult3, 0x80000000U ); // 1 << 31
}

TEST( BitTest, BitIsCorrectForUint64 )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto bitResult = ( bit< uint64_t, 1 >() );
	const auto bitResult2 = ( bit< uint64_t, 33 >() );
	const auto bitResult3 = ( bit< uint64_t, 64 >() );

	// Assert
	EXPECT_EQ( bitResult, 0x0000000000000001ULL ); // 1 << 0
	EXPECT_EQ( bitResult2, 0x0000000100000000ULL ); // 1 << 32
	EXPECT_EQ( bitResult3, 0x8000000000000000ULL ); // 1 << 63
}

TEST( BitTest, BitIsCompileTimeEvaluable )
{
	// Arrange
	// No additional setup required.

	// Act
	constexpr auto thirdBit = bit< uint8_t, 3 >();
	constexpr auto highestBit = bit< uint32_t, 32 >();

	// Assert
	static_assert( thirdBit == 0x04, "bit<T, Shift> must be constexpr" ); // 1 << (3-1) = 1 << 2 = 0x04
	static_assert( highestBit == 0x80000000U, "bit<T, Shift> must be constexpr" ); // 1 << 31
}

} // namespace pbl::utils
