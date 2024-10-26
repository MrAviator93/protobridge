#include <math/Matrix3x3.hpp>

// Third Party
#include <doctest/doctest.h>

SCENARIO( "Testing element access" )
{
	GIVEN( "A 2x2 matrix" )
	{
		pbl::math::Matrix3x3< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

		THEN( "Accessing elements should return the correct values" )
		{
			CHECK_EQ( matrix.at( 0 ), 1.0f );
			CHECK_EQ( matrix.at( 1 ), 2.0f );
			CHECK_EQ( matrix.at( 2 ), 3.0f );
			CHECK_EQ( matrix.at( 3 ), 4.0f );
            CHECK_EQ( matrix.at( 4 ), 5.0f );
            CHECK_EQ( matrix.at( 5 ), 6.0f );
            CHECK_EQ( matrix.at( 6 ), 7.0f );
            CHECK_EQ( matrix.at( 7 ), 8.0f );
            CHECK_EQ( matrix.at( 8 ), 9.0f );
		}
	}
}