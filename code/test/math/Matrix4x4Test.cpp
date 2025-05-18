// PBL
#include <math/Matrix4x4.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::math
{

TEST( Matrix4x4Test, ElementAccessReturnsCorrectValues )
{
	Matrix4x4< float > matrix{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

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
}

TEST( Matrix4x4Test, MatrixAdditionReturnsCorrectResult )
{
	Matrix4x4< float > a{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

	Matrix4x4< float > b{
		16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	auto result = a + b;

	for( int i = 0; i < 16; ++i )
	{
		EXPECT_FLOAT_EQ( result.at( i ).value(), 17.0f );
	}
}

TEST( Matrix4x4Test, MatrixSubtractionReturnsCorrectResult )
{
	Matrix4x4< float > a{
		16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	Matrix4x4< float > b{
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	auto result = a - b;

	for( int i = 0; i < 16; ++i )
	{
		EXPECT_FLOAT_EQ( result.at( i ).value(), static_cast< float >( a.at( i ).value() - 1.0f ) );
	}
}

TEST( Matrix4x4Test, MatrixMultiplicationReturnsCorrectResult )
{
	Matrix4x4< float > a{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

	Matrix4x4< float > b{
		16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	Matrix4x4< float > result = a * b;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 80.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 70.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 60.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 50.0f );
	EXPECT_FLOAT_EQ( result.at( 4 ).value(), 240.0f );
	EXPECT_FLOAT_EQ( result.at( 5 ).value(), 214.0f );
	EXPECT_FLOAT_EQ( result.at( 6 ).value(), 188.0f );
	EXPECT_FLOAT_EQ( result.at( 7 ).value(), 162.0f );
	EXPECT_FLOAT_EQ( result.at( 8 ).value(), 400.0f );
	EXPECT_FLOAT_EQ( result.at( 9 ).value(), 358.0f );
	EXPECT_FLOAT_EQ( result.at( 10 ).value(), 316.0f );
	EXPECT_FLOAT_EQ( result.at( 11 ).value(), 274.0f );
	EXPECT_FLOAT_EQ( result.at( 12 ).value(), 560.0f );
	EXPECT_FLOAT_EQ( result.at( 13 ).value(), 502.0f );
	EXPECT_FLOAT_EQ( result.at( 14 ).value(), 444.0f );
	EXPECT_FLOAT_EQ( result.at( 15 ).value(), 386.0f );
}

TEST( Matrix4x4Test, ScalarAdditionReturnsCorrectMatrix )
{
	Matrix4x4< float > matrix{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

	auto result = matrix + 1.0f;

	for( int i = 0; i < 16; ++i )
	{
		EXPECT_FLOAT_EQ( result.at( i ).value(), static_cast< float >( i + 2 ) );
	}
}

TEST( Matrix4x4Test, ScalarAdditionInPlaceModifiesMatrix )
{
	Matrix4x4< float > matrix{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

	matrix += 1.0f;

	for( int i = 0; i < 16; ++i )
	{
		EXPECT_FLOAT_EQ( matrix.at( i ).value(), static_cast< float >( i + 2 ) );
	}
}

TEST( Matrix4x4Test, ScalarMultiplicationReturnsCorrectMatrix )
{
	Matrix4x4< float > matrix{
		1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };

	matrix = matrix * 2.0f;

	for( int i = 0; i < 16; ++i )
	{
		EXPECT_FLOAT_EQ( matrix.at( i ).value(), static_cast< float >( ( i + 1 ) * 2 ) );
	}
}

} // namespace pbl::math
