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
	EXPECT_EQ( ( bit< uint8_t, 1 >() ), static_cast< uint8_t >( 0x01 ) ); // 1 << 0
	EXPECT_EQ( ( bit< uint8_t, 2 >() ), static_cast< uint8_t >( 0x02 ) ); // 1 << 1
	EXPECT_EQ( ( bit< uint8_t, 8 >() ), static_cast< uint8_t >( 0x80 ) ); // 1 << 7
}

TEST( BitTest, BitIsCorrectForUint16 )
{
	EXPECT_EQ( ( bit< uint16_t, 1 >() ), static_cast< uint16_t >( 0x0001 ) ); // 1 << 0
	EXPECT_EQ( ( bit< uint16_t, 9 >() ), static_cast< uint16_t >( 0x0100 ) ); // 1 << 8
	EXPECT_EQ( ( bit< uint16_t, 16 >() ), static_cast< uint16_t >( 0x8000 ) ); // 1 << 15
}

TEST( BitTest, BitIsCorrectForUint32 )
{
	EXPECT_EQ( ( bit< uint32_t, 1 >() ), 0x00000001U ); // 1 << 0
	EXPECT_EQ( ( bit< uint32_t, 17 >() ), 0x00010000U ); // 1 << 16
	EXPECT_EQ( ( bit< uint32_t, 32 >() ), 0x80000000U ); // 1 << 31
}

TEST( BitTest, BitIsCorrectForUint64 )
{
	EXPECT_EQ( ( bit< uint64_t, 1 >() ), 0x0000000000000001ULL ); // 1 << 0
	EXPECT_EQ( ( bit< uint64_t, 33 >() ), 0x0000000100000000ULL ); // 1 << 32
	EXPECT_EQ( ( bit< uint64_t, 64 >() ), 0x8000000000000000ULL ); // 1 << 63
}

TEST( BitTest, BitIsCompileTimeEvaluable )
{
	static_assert( bit< uint8_t, 3 >() == 0x04, "bit<T, Shift> must be constexpr" ); // 1 << (3-1) = 1 << 2 = 0x04
	static_assert( bit< uint32_t, 32 >() == 0x80000000U, "bit<T, Shift> must be constexpr" ); // 1 << 31
}

} // namespace pbl::utils
