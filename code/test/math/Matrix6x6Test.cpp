// PBL
#include <math/Matrix6x6.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::math
{

TEST( Matrix6x6Test, ElementAccessReturnsCorrectValues )
{
	Matrix6x6< float > matrix{ 1.0f,  2.0f,	 3.0f,	4.0f,  5.0f,  6.0f,	 7.0f,	8.0f,  9.0f,  10.0f, 11.0f, 12.0f,
							   13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f,
							   25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f };

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 1.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 4 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( matrix.at( 5 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( matrix.at( 6 ).value(), 7.0f );
	EXPECT_FLOAT_EQ( matrix.at( 7 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( matrix.at( 8 ).value(), 9.0f );
	EXPECT_FLOAT_EQ( matrix.at( 9 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( matrix.at( 10 ).value(), 11.0f );
	EXPECT_FLOAT_EQ( matrix.at( 11 ).value(), 12.0f );
	EXPECT_FLOAT_EQ( matrix.at( 12 ).value(), 13.0f );
	EXPECT_FLOAT_EQ( matrix.at( 13 ).value(), 14.0f );
	EXPECT_FLOAT_EQ( matrix.at( 14 ).value(), 15.0f );
	EXPECT_FLOAT_EQ( matrix.at( 15 ).value(), 16.0f );
	EXPECT_FLOAT_EQ( matrix.at( 16 ).value(), 17.0f );
	EXPECT_FLOAT_EQ( matrix.at( 17 ).value(), 18.0f );
	EXPECT_FLOAT_EQ( matrix.at( 18 ).value(), 19.0f );
	EXPECT_FLOAT_EQ( matrix.at( 19 ).value(), 20.0f );
	EXPECT_FLOAT_EQ( matrix.at( 20 ).value(), 21.0f );
	EXPECT_FLOAT_EQ( matrix.at( 21 ).value(), 22.0f );
	EXPECT_FLOAT_EQ( matrix.at( 22 ).value(), 23.0f );
	EXPECT_FLOAT_EQ( matrix.at( 23 ).value(), 24.0f );
	EXPECT_FLOAT_EQ( matrix.at( 24 ).value(), 25.0f );
	EXPECT_FLOAT_EQ( matrix.at( 25 ).value(), 26.0f );
	EXPECT_FLOAT_EQ( matrix.at( 26 ).value(), 27.0f );
	EXPECT_FLOAT_EQ( matrix.at( 27 ).value(), 28.0f );
	EXPECT_FLOAT_EQ( matrix.at( 28 ).value(), 29.0f );
	EXPECT_FLOAT_EQ( matrix.at( 29 ).value(), 30.0f );
	EXPECT_FLOAT_EQ( matrix.at( 30 ).value(), 31.0f );
	EXPECT_FLOAT_EQ( matrix.at( 31 ).value(), 32.0f );
	EXPECT_FLOAT_EQ( matrix.at( 32 ).value(), 33.0f );
	EXPECT_FLOAT_EQ( matrix.at( 33 ).value(), 34.0f );
	EXPECT_FLOAT_EQ( matrix.at( 34 ).value(), 35.0f );
	EXPECT_FLOAT_EQ( matrix.at( 35 ).value(), 36.0f );
}

} // namespace pbl::math
