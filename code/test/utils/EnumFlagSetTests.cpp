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
	// Arrange
	// No additional setup required.

	// Act
	FlagSet flags;
	const auto flagsNoneResult = flags.none();
	const auto flagsAnyResult = flags.any();

	// Assert
	EXPECT_TRUE( flagsNoneResult );
	EXPECT_FALSE( flagsAnyResult );
	EXPECT_EQ( flags.value(), 0 );
}

TEST( EnumFlagSetTest, ConstructFromEnumValue )
{
	// Arrange
	// No additional setup required.

	// Act
	FlagSet flags( TestFlags::FlagA );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_FALSE( flagsTestFlagBResult );
}

TEST( EnumFlagSetTest, ConstructFromIntValue )
{
	// Arrange
	// No additional setup required.

	// Act
	FlagSet flags( static_cast< uint8_t >( TestFlags::FlagA | TestFlags::FlagC ) );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagCResult = flags.test( TestFlags::FlagC );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagCResult );
	EXPECT_FALSE( flagsTestFlagBResult );
}

TEST( EnumFlagSetTest, VariadicConstructorWorks )
{
	// Arrange
	// No additional setup required.

	// Act
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagB );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );
	const auto flagsTestFlagCResult = flags.test( TestFlags::FlagC );

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagBResult );
	EXPECT_FALSE( flagsTestFlagCResult );
}

TEST( EnumFlagSetTest, SetAndClearFlags )
{
	// Arrange
	FlagSet flags;

	// Act
	flags.set( TestFlags::FlagA );
	flags.set( TestFlags::FlagB );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );
	flags.clear( TestFlags::FlagA );
	const auto flagsTestFlagAResult2 = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagBResult2 = flags.test( TestFlags::FlagB );

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagBResult );
	EXPECT_FALSE( flagsTestFlagAResult2 );
	EXPECT_TRUE( flagsTestFlagBResult2 );
}

TEST( EnumFlagSetTest, VariadicSetAndClear )
{
	// Arrange
	FlagSet flags;

	// Act
	flags.set( TestFlags::FlagA, TestFlags::FlagC );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagCResult = flags.test( TestFlags::FlagC );
	flags.clear( TestFlags::FlagA, TestFlags::FlagC );
	const auto flagsNoneResult = flags.none();

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagCResult );
	EXPECT_TRUE( flagsNoneResult );
}

TEST( EnumFlagSetTest, FlipWorksCorrectly )
{
	// Arrange
	FlagSet flags;

	// Act
	flags.set( TestFlags::FlagA );
	flags.flip( TestFlags::FlagA );
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	flags.flip( TestFlags::FlagB );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );

	// Assert
	EXPECT_FALSE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagBResult );
}

TEST( EnumFlagSetTest, CountSetBits )
{
	// Arrange
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagC );

	// Act
	const auto flagsCountResult = flags.count();
	flags.set( TestFlags::FlagB );
	const auto flagsCountResult2 = flags.count();
	flags.reset();
	const auto flagsCountResult3 = flags.count();

	// Assert
	EXPECT_EQ( flagsCountResult, 2 );
	EXPECT_EQ( flagsCountResult2, 3 );
	EXPECT_EQ( flagsCountResult3, 0 );
}

// TODO: Adjust this test case!
TEST( EnumFlagSetTest, AllAnyNoneChecks )
{
	// Arrange
	FlagSet flags{ TestFlags::FlagA, TestFlags::FlagB };

	// Act
	const auto flagsAnyResult = flags.any();
	const auto flagsNoneResult = flags.none();
	const auto flagsFullResult = flags.full();
	flags.reset();
	const auto flagsNoneResult2 = flags.none();
	// Only one bit set
	flags.set( TestFlags::FlagC );
	const auto flagsFullResult2 = flags.full();

	// Assert
	EXPECT_TRUE( flagsAnyResult );
	EXPECT_FALSE( flagsNoneResult );
	EXPECT_FALSE( flagsFullResult );
	EXPECT_TRUE( flagsNoneResult2 );
	EXPECT_FALSE( flagsFullResult2 );
}

TEST( EnumFlagSetTest, ComparisonOperator )
{
	// Arrange
	FlagSet f1( TestFlags::FlagA, TestFlags::FlagB );
	FlagSet f2( TestFlags::FlagA, TestFlags::FlagB );
	FlagSet f3( TestFlags::FlagA );

	// Act
	const auto f1F2Result = f1 == f2;
	const auto f1F3Result = f1 == f3;
	const auto f3F1Result = f3 < f1;

	// Assert
	EXPECT_TRUE( f1F2Result );
	EXPECT_FALSE( f1F3Result );
	EXPECT_TRUE( f3F1Result );
}

TEST( EnumFlagSetTest, AllVariadicCheck )
{
	// Arrange
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );

	// Act
	const auto flagsAllFlagAFlagBResult = flags.all( TestFlags::FlagA, TestFlags::FlagB );
	const auto flagsAllFlagCFlagDResult = flags.all( TestFlags::FlagC, TestFlags::FlagD );
	const auto flagsAllFlagAFlagBFlagCFlagDResult =
		flags.all( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );
	flags.clear( TestFlags::FlagD );
	const auto flagsAllFlagAFlagBFlagCFlagDResult2 =
		flags.all( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );

	// Assert
	EXPECT_TRUE( flagsAllFlagAFlagBResult );
	EXPECT_TRUE( flagsAllFlagCFlagDResult );
	EXPECT_TRUE( flagsAllFlagAFlagBFlagCFlagDResult );
	EXPECT_FALSE( flagsAllFlagAFlagBFlagCFlagDResult2 );
}

TEST( EnumFlagSetTest, AnyVariadicCheck )
{
	// Arrange
	FlagSet flags( TestFlags::FlagA );

	// Act
	const auto flagsAnyFlagBFlagAResult = flags.any( TestFlags::FlagB, TestFlags::FlagA );
	const auto flagsAnyFlagCFlagDResult = flags.any( TestFlags::FlagC, TestFlags::FlagD );

	// Assert
	EXPECT_TRUE( flagsAnyFlagBFlagAResult ); // A is set
	EXPECT_FALSE( flagsAnyFlagCFlagDResult );
}

TEST( EnumFlagSetTest, CombinedEnumAndIntConstructor )
{
	// Arrange
	uint8_t combined = static_cast< uint8_t >( TestFlags::FlagA ) | static_cast< uint8_t >( TestFlags::FlagC );
	FlagSet flags( combined );

	// Act
	const auto flagsTestFlagAResult = flags.test( TestFlags::FlagA );
	const auto flagsTestFlagCResult = flags.test( TestFlags::FlagC );
	const auto flagsTestFlagBResult = flags.test( TestFlags::FlagB );

	// Assert
	EXPECT_TRUE( flagsTestFlagAResult );
	EXPECT_TRUE( flagsTestFlagCResult );
	EXPECT_FALSE( flagsTestFlagBResult );
}

TEST( EnumFlagSetTest, SetAndClearAllFlags )
{
	// Arrange
	FlagSet flags;

	// Act
	flags.set( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );
	const auto flagsAllFlagAFlagBFlagCFlagDResult =
		flags.all( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );
	flags.clear( TestFlags::FlagA, TestFlags::FlagB, TestFlags::FlagC, TestFlags::FlagD );
	const auto flagsNoneResult = flags.none();

	// Assert
	EXPECT_TRUE( flagsAllFlagAFlagBFlagCFlagDResult );
	EXPECT_TRUE( flagsNoneResult );
}

TEST( EnumFlagSetTest, ValueGetter )
{
	// Arrange
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagC );

	// Act
	auto val = flags.value();

	// Assert
	EXPECT_EQ( val, static_cast< uint8_t >( TestFlags::FlagA ) | static_cast< uint8_t >( TestFlags::FlagC ) );
}

TEST( EnumFlagSetTest, ResetOnNonEmpty )
{
	// Arrange
	FlagSet flags( TestFlags::FlagA, TestFlags::FlagD );

	// Act
	const auto flagsAnyResult = flags.any();
	flags.reset();
	const auto flagsNoneResult = flags.none();

	// Assert
	EXPECT_TRUE( flagsAnyResult );
	EXPECT_TRUE( flagsNoneResult );
}

} // namespace pbl::utils
