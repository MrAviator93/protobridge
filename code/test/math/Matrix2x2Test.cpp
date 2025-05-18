// PBL
#include <math/Matrix2x2.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::math
{
TEST( Matrix2x2Test, ElementAccessReturnsCorrectValues )
{
	Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 1.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 4.0f );
}

TEST( Matrix2x2Test, MatrixAdditionReturnsCorrectResult )
{
	Matrix2x2< float > matrix1{ 1.0f, 2.0f, 3.0f, 4.0f };
	Matrix2x2< float > matrix2{ 5.0f, 6.0f, 7.0f, 8.0f };

	const auto result = matrix1 + matrix2;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 8.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 12.0f );
}

TEST( Matrix2x2Test, MatrixSubtractionReturnsCorrectResult )
{
	Matrix2x2< float > matrix1{ 5.0f, 6.0f, 7.0f, 8.0f };
	Matrix2x2< float > matrix2{ 1.0f, 2.0f, 3.0f, 4.0f };

	const auto result = matrix1 - matrix2;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 4.0f );
}

TEST( Matrix2x2Test, MatrixMultiplicationReturnsCorrectResult )
{
	Matrix2x2< float > matrix1{ 1.0f, 2.0f, 3.0f, 4.0f };
	Matrix2x2< float > matrix2{ 2.0f, 0.0f, 1.0f, 2.0f };

	const auto result = matrix1 * matrix2;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 10.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 8.0f );
}

TEST( Matrix2x2Test, ScalarAdditionReturnsCorrectMatrix )
{
	Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };
	float scalar = 2.0f;

	const auto result = matrix + scalar;

	EXPECT_FLOAT_EQ( result.at( 0 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( result.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( result.at( 2 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( result.at( 3 ).value(), 6.0f );
}

TEST( Matrix2x2Test, ScalarAdditionInPlaceModifiesMatrix )
{
	Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };
	float scalar = 2.0f;

	matrix += scalar;

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 3.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 5.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 6.0f );
}

TEST( Matrix2x2Test, ScalarMultiplicationModifiesMatrixCorrectly )
{
	Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };

	matrix = matrix * 2.0f;

	EXPECT_FLOAT_EQ( matrix.at( 0 ).value(), 2.0f );
	EXPECT_FLOAT_EQ( matrix.at( 1 ).value(), 4.0f );
	EXPECT_FLOAT_EQ( matrix.at( 2 ).value(), 6.0f );
	EXPECT_FLOAT_EQ( matrix.at( 3 ).value(), 8.0f );
}

} // namespace pbl::math
