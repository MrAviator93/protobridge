// PBL
#include <utils/CompactBitset.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( CompactBitsetTest, AllBitsInitiallyUnset )
{
	// Arrange
	// No additional setup required.

	// Act
	CompactBitset< 2 > bitset;
	const auto bitsetTestResult = bitset.test( 0 );
	const auto bitsetTestResult2 = bitset.test( 1 );

	// Assert
	EXPECT_FALSE( bitsetTestResult );
	EXPECT_FALSE( bitsetTestResult2 );
}

TEST( CompactBitsetTest, SetBitsReflectNewState )
{
	// Arrange
	CompactBitset< 2 > bitset;

	// Act
	bitset.set( 0u );
	bitset.set( 1u );
	const auto bitsetTestUResult = bitset.test( 0u );
	const auto bitsetTestUResult2 = bitset.test( 1u );

	// Assert
	EXPECT_TRUE( bitsetTestUResult );
	EXPECT_TRUE( bitsetTestUResult2 );
}

TEST( CompactBitsetTest, ResetAllBitsUnsetsThem )
{
	// Arrange
	CompactBitset< 2 > bitset;

	bitset.set( 0 );
	bitset.set( 1 );

	// Act
	bitset.reset();
	const auto bitsetTestResult = bitset.test( 0 );
	const auto bitsetTestResult2 = bitset.test( 1 );

	// Assert
	EXPECT_FALSE( bitsetTestResult );
	EXPECT_FALSE( bitsetTestResult2 );
}

TEST( CompactBitsetTest, FlipChangesBitState )
{
	// Arrange
	CompactBitset< 2 > bitset;

	// Act
	bitset.flip( 0 );
	const auto bitsetTestResult = bitset.test( 0 );
	const auto bitsetTestResult2 = bitset.test( 1 );

	// Assert
	EXPECT_TRUE( bitsetTestResult );
	EXPECT_FALSE( bitsetTestResult2 );
}

TEST( CompactBitsetTest, ResetSingleBit )
{
	// Arrange
	CompactBitset< 2 > bitset;

	bitset.set( 0 );
	bitset.set( 1 );

	// Act
	bitset.reset( 1 );
	const auto bitsetTestResult = bitset.test( 0 );
	const auto bitsetTestResult2 = bitset.test( 1 );

	// Assert
	EXPECT_TRUE( bitsetTestResult );
	EXPECT_FALSE( bitsetTestResult2 );
}

TEST( CompactBitsetTest, CompareTwoBitsetsInitiallyEqual )
{
	// Arrange
	CompactBitset< 2 > bitset;
	CompactBitset< 2 > otherBitset;

	// Act
	const auto bitsetOtherBitsetResult = bitset == otherBitset;

	// Assert
	EXPECT_TRUE( bitsetOtherBitsetResult );
}

TEST( CompactBitsetTest, CompareTwoBitsetsAfterMutation )
{
	// Arrange
	CompactBitset< 2 > bitset;
	CompactBitset< 2 > otherBitset;

	// Act
	bitset.set( 0 );
	const auto bitsetOtherBitsetResult = bitset == otherBitset;

	// Assert
	EXPECT_FALSE( bitsetOtherBitsetResult );
}

} // namespace pbl::utils
