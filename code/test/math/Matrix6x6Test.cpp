#include <math/Matrix6x6.hpp>

// Third Party
#include <doctest/doctest.h>

SCENARIO( "Testing element access" )
{
	GIVEN( "A 6x6 matrix" )
	{
		pbl::math::Matrix6x6< float > matrix{ 1.0f,	 2.0f,	3.0f,  4.0f,  5.0f,	 6.0f,	7.0f,  8.0f,  9.0f,
											  10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f,
											  19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f,
											  28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f };

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
			CHECK_EQ( matrix.at( 9 ), 10.0f );
			CHECK_EQ( matrix.at( 10 ), 11.0f );
			CHECK_EQ( matrix.at( 11 ), 12.0f );
			CHECK_EQ( matrix.at( 12 ), 13.0f );
			CHECK_EQ( matrix.at( 13 ), 14.0f );
			CHECK_EQ( matrix.at( 14 ), 15.0f );
			CHECK_EQ( matrix.at( 15 ), 16.0f );
			CHECK_EQ( matrix.at( 16 ), 17.0f );
			CHECK_EQ( matrix.at( 17 ), 18.0f );
			CHECK_EQ( matrix.at( 18 ), 19.0f );
			CHECK_EQ( matrix.at( 19 ), 20.0f );
			CHECK_EQ( matrix.at( 20 ), 21.0f );
			CHECK_EQ( matrix.at( 21 ), 22.0f );
			CHECK_EQ( matrix.at( 22 ), 23.0f );
			CHECK_EQ( matrix.at( 23 ), 24.0f );
			CHECK_EQ( matrix.at( 24 ), 25.0f );
			CHECK_EQ( matrix.at( 25 ), 26.0f );
			CHECK_EQ( matrix.at( 26 ), 27.0f );
			CHECK_EQ( matrix.at( 27 ), 28.0f );
			CHECK_EQ( matrix.at( 28 ), 29.0f );
			CHECK_EQ( matrix.at( 29 ), 30.0f );
			CHECK_EQ( matrix.at( 30 ), 31.0f );
			CHECK_EQ( matrix.at( 31 ), 32.0f );
			CHECK_EQ( matrix.at( 32 ), 33.0f );
			CHECK_EQ( matrix.at( 33 ), 34.0f );
			CHECK_EQ( matrix.at( 34 ), 35.0f );
			CHECK_EQ( matrix.at( 35 ), 36.0f );
		}
	}
}
