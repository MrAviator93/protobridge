// PBL
#include <utils/CompactBitset.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( CompactBitsetTest, AllBitsInitiallyUnset )
{
	CompactBitset< 2 > bitset;
	EXPECT_FALSE( bitset.test( 0 ) );
	EXPECT_FALSE( bitset.test( 1 ) );
}

TEST( CompactBitsetTest, SetBitsReflectNewState )
{
	CompactBitset< 2 > bitset;
	bitset.set( 0u );
	bitset.set( 1u );
	EXPECT_TRUE( bitset.test( 0u ) );
	EXPECT_TRUE( bitset.test( 1u ) );
}

TEST( CompactBitsetTest, ResetAllBitsUnsetsThem )
{
	CompactBitset< 2 > bitset;
	bitset.set( 0 );
	bitset.set( 1 );
	bitset.reset();
	EXPECT_FALSE( bitset.test( 0 ) );
	EXPECT_FALSE( bitset.test( 1 ) );
}

TEST( CompactBitsetTest, FlipChangesBitState )
{
	CompactBitset< 2 > bitset;
	bitset.flip( 0 );
	EXPECT_TRUE( bitset.test( 0 ) );
	EXPECT_FALSE( bitset.test( 1 ) );
}

TEST( CompactBitsetTest, ResetSingleBit )
{
	CompactBitset< 2 > bitset;
	bitset.set( 0 );
	bitset.set( 1 );
	bitset.reset( 1 );
	EXPECT_TRUE( bitset.test( 0 ) );
	EXPECT_FALSE( bitset.test( 1 ) );
}

TEST( CompactBitsetTest, CompareTwoBitsetsInitiallyEqual )
{
	CompactBitset< 2 > bitset;
	CompactBitset< 2 > otherBitset;
	EXPECT_TRUE( bitset == otherBitset );
}

TEST( CompactBitsetTest, CompareTwoBitsetsAfterMutation )
{
	CompactBitset< 2 > bitset;
	CompactBitset< 2 > otherBitset;
	bitset.set( 0 );
	EXPECT_FALSE( bitset == otherBitset );
}

} // namespace pbl::utils
