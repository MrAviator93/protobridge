// PBL
#include <math/Matrix3x3.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::math
{

TEST( Matrix3x3Test, ElementAccessReturnsCorrectValues )
{
	Matrix3x3< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 1.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 4 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( matrix.at( 5 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( matrix.at( 6 ).value(), 7.0f );
	EXPECT_FLOAT_EQ( matrix.at( 7 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( matrix.at( 8 ).value(), 9.0f );
}

TEST( Matrix3x3Test, MatrixAdditionReturnsCorrectResult )
{
	Matrix3x3< float > a{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	Matrix3x3< float > b{ 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	auto result = a + b;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 4 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 5 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 6 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 7 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 8 ).value(), 10.0f );
}

TEST( Matrix3x3Test, MatrixSubtractionReturnsCorrectResult )
{
	Matrix3x3< float > a{ 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	Matrix3x3< float > b{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	auto result = a - b;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 7.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( result.at( 4 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 5 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( result.at( 6 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( result.at( 7 ).value(), 1.0f );
	EXPECT_FLOAT_EQ( result.at( 8 ).value(), 0.0f );
}

TEST( Matrix3x3Test, MatrixMultiplicationReturnsCorrectResult )
{
	Matrix3x3< float > a{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	Matrix3x3< float > b{ 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	auto result = a * b;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 30.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 24.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 18.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 84.0f );
	EXPECT_FLOAT_EQ( result.at( 4 ).value(), 69.0f );
	EXPECT_FLOAT_EQ( result.at( 5 ).value(), 54.0f );
	EXPECT_FLOAT_EQ( result.at( 6 ).value(), 138.0f );
	EXPECT_FLOAT_EQ( result.at( 7 ).value(), 114.0f );
	EXPECT_FLOAT_EQ( result.at( 8 ).value(), 90.0f );
}

TEST( Matrix3x3Test, ScalarAdditionReturnsCorrectMatrix )
{
	Matrix3x3< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	auto result = matrix + 1.0f;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( result.at( 4 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( result.at( 5 ).value(), 7.0f );
	EXPECT_FLOAT_EQ( result.at( 6 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( result.at( 7 ).value(), 9.0f );
	EXPECT_FLOAT_EQ( result.at( 8 ).value(), 10.0f );
}

TEST( Matrix3x3Test, ScalarAdditionInPlaceModifiesMatrix )
{
	Matrix3x3< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	matrix += 1.0f;

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( matrix.at( 4 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( matrix.at( 5 ).value(), 7.0f );
	EXPECT_FLOAT_EQ( matrix.at( 6 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( matrix.at( 7 ).value(), 9.0f );
	EXPECT_FLOAT_EQ( matrix.at( 8 ).value(), 10.0f );
}

TEST( Matrix3x3Test, ScalarMultiplicationReturnsCorrectMatrix )
{
	Matrix3x3< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	matrix = matrix * 2.0f;

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( matrix.at( 4 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( matrix.at( 5 ).value(), 12.0f );
	EXPECT_FLOAT_EQ( matrix.at( 6 ).value(), 14.0f );
	EXPECT_FLOAT_EQ( matrix.at( 7 ).value(), 16.0f );
	EXPECT_FLOAT_EQ( matrix.at( 8 ).value(), 18.0f );
}

} // namespace pbl::math
