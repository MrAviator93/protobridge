
// I2C
#include <utils/CompactBitset.hpp>

// 3rd Party
#include <doctest/doctest.h>

SCENARIO( "Testing the functionalities of CompactBitset" )
{
	GIVEN( "A CompactBitset of 2 bits" )
	{
		pbl::utils::CompactBitset< 2 > bitset;

		THEN( "All bits are initially unset" )
		{
			CHECK_FALSE( bitset.test( 0 ) );
			CHECK_FALSE( bitset.test( 1 ) );
		}

		WHEN( "Bits are set" )
		{
			bitset.set( 0 );
			bitset.set( 1 );

			THEN( "They should reflect the new state" )
			{
				CHECK( bitset.test( 0 ) );
				CHECK( bitset.test( 1 ) );
			}
		}

		WHEN( "Bits are reset" )
		{
			bitset.set( 0 );
			bitset.set( 1 );
			bitset.reset();

			THEN( "All bits should be unset" )
			{
				CHECK_FALSE( bitset.test( 0 ) );
				CHECK_FALSE( bitset.test( 1 ) );
			}
		}

		WHEN( "Bits are flipped" )
		{
			// Assuming 0 was initially false.
			bitset.flip( 0 );

			THEN( "They should reflect the flipped state" )
			{
				CHECK( bitset.test( 0 ) );

				// Assuming 1 remains initially false.
				CHECK_FALSE( bitset.test( 1 ) );
			}
		}

		WHEN( "A single bit is reset" )
		{
			bitset.set( 0 );
			bitset.set( 1 );
			bitset.reset( 1 );

			THEN( "Only the specified bit should be unset" )
			{
				CHECK( bitset.test( 0 ) );
				CHECK_FALSE( bitset.test( 1 ) );
			}
		}

		WHEN( "Comparing two bitsets" )
		{
			pbl::utils::CompactBitset< 2 > otherBitset;

			// Initially, both should be equal because both are unset
			CHECK( bitset == otherBitset );

			// Change the state of 'bitset'
			bitset.set( 0 );

			THEN( "They should not be equal if their bits differ" )
			{
				CHECK_FALSE( bitset == otherBitset );
			}
		}
	}
}
