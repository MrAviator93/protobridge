// PBL
#include <utils/EnumFlagSet.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

namespace
{

enum class TestFlags : uint8_t
{
	None = 0,
	FlagA = 1 << 0,
	FlagB = 1 << 1,
	FlagC = 1 << 2,
	FlagD = 1 << 3,
	All = FlagA | FlagB | FlagC | FlagD
};

using FlagSet = pbl::utils::EnumFlagSet< uint8_t, TestFlags >;

} // namespace

TEST( EnumFlagSetTest, DefaultConstructorHasNoFlags )
{
	FlagSet flags;
	EXPECT_TRUE( flags.none() );
	EXPECT_FALSE( flags.any() );
	EXPECT_EQ( flags.value(), 0 );
}

TEST( EnumFlagSetTest, ConstructFromEnumValue )
{
	FlagSet flags( TestFlags::FlagA );
	EXPECT_TRUE( flags.test( TestFlags::FlagA ) );
	EXPECT_FALSE( flags.test( TestFlags::FlagB ) );
}

TEST( EnumFlagSetTest, ConstructFromIntValue )
{
	FlagSet flags( static_cast< uint8_t >( TestFlags::FlagA | TestFlags::FlagC ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagA ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagC ) );
	EXPECT_FALSE( flags.test( TestFlags::FlagB ) );
}

TEST( EnumFlagSetTest, VariadicConstructorWorks )
{
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagB );
	EXPECT_TRUE( flags.test( TestFlags::FlagA ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagB ) );
	EXPECT_FALSE( flags.test( TestFlags::FlagC ) );
}

TEST( EnumFlagSetTest, SetAndClearFlags )
{
	FlagSet flags;
	flags.set( TestFlags::FlagA );
	flags.set( TestFlags::FlagB );
	EXPECT_TRUE( flags.test( TestFlags::FlagA ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagB ) );

	flags.clear( TestFlags::FlagA );
	EXPECT_FALSE( flags.test( TestFlags::FlagA ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagB ) );
}

TEST( EnumFlagSetTest, VariadicSetAndClear )
{
	FlagSet flags;
	flags.set( TestFlags::FlagA, TestFlags::FlagC );
	EXPECT_TRUE( flags.test( TestFlags::FlagA ) );
	EXPECT_TRUE( flags.test( TestFlags::FlagC ) );

	flags.clear( TestFlags::FlagA, TestFlags::FlagC );
	EXPECT_TRUE( flags.none() );
}

TEST( EnumFlagSetTest, FlipWorksCorrectly )
{
	FlagSet flags;
	flags.set( TestFlags::FlagA );
	flags.flip( TestFlags::FlagA );
	EXPECT_FALSE( flags.test( TestFlags::FlagA ) );

	flags.flip( TestFlags::FlagB );
	EXPECT_TRUE( flags.test( TestFlags::FlagB ) );
}

TEST( EnumFlagSetTest, CountSetBits )
{
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagC );
	EXPECT_EQ( flags.count(), 2 );

	flags.set( TestFlags::FlagB );
	EXPECT_EQ( flags.count(), 3 );

	flags.reset();
	EXPECT_EQ( flags.count(), 0 );
}

// TODO: Adjust this test case!
// TEST( EnumFlagSetTest, AllAnyNoneChecks )
// {
// 	FlagSet flags( TestFlags::FlagA, TestFlags::FlagB );
// 	EXPECT_TRUE( flags.any() );
// 	EXPECT_FALSE( flags.none() );
// 	EXPECT_FALSE( flags.all() );

// 	flags.reset();
// 	EXPECT_TRUE( flags.none() );

// 	// Only one bit set
// 	flags.set( TestFlags::FlagC );
// 	EXPECT_TRUE( flags.all() ); // only one bit = true, all() logic holds
// }

TEST( EnumFlagSetTest, ComparisonOperator )
{
	FlagSet f1( TestFlags::FlagA, TestFlags::FlagB );
	FlagSet f2( TestFlags::FlagA, TestFlags::FlagB );
	FlagSet f3( TestFlags::FlagA );

	EXPECT_TRUE( f1 == f2 );
	EXPECT_FALSE( f1 == f3 );
	EXPECT_TRUE( f3 < f1 );
}

} // namespace pbl::utils
