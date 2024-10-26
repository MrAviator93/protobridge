#include <math/Matrix2x2.hpp>

// 3rd Party
#include <doctest/doctest.h>

SCENARIO( "Testing element access" )
{
	GIVEN( "A 2x2 matrix" )
	{
		pbl::math::Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };

		THEN( "Accessing elements should return the correct values" )
		{
			CHECK_EQ( matrix.at( 0 ), 1.0f );
			CHECK_EQ( matrix.at( 1 ), 2.0f );
			CHECK_EQ( matrix.at( 2 ), 3.0f );
			CHECK_EQ( matrix.at( 3 ), 4.0f );
		}
	}
}

SCENARIO( "Testing matrix addition" )
{
	GIVEN( "Two 2x2 matrices" )
	{
		pbl::math::Matrix2x2< float > matrix1{ 1.0f, 2.0f, 3.0f, 4.0f };
		pbl::math::Matrix2x2< float > matrix2{ 5.0f, 6.0f, 7.0f, 8.0f };

		WHEN( "We add the two matrices" )
		{
			auto result = matrix1 + matrix2;

			THEN( "The result should be the element-wise sum of the two matrices" )
			{
				CHECK_EQ( result.at( 0 ), 6.0f );
				CHECK_EQ( result.at( 1 ), 8.0f );
				CHECK_EQ( result.at( 2 ), 10.0f );
				CHECK_EQ( result.at( 3 ), 12.0f );
			}
		}
	}
}

SCENARIO( "Testing matrix subtraction" )
{
	GIVEN( "Two 2x2 matrices" )
	{
		pbl::math::Matrix2x2< float > matrix1{ 5.0f, 6.0f, 7.0f, 8.0f };
		pbl::math::Matrix2x2< float > matrix2{ 1.0f, 2.0f, 3.0f, 4.0f };

		WHEN( "We subtract the second matrix from the first" )
		{
			auto result = matrix1 - matrix2;

			THEN( "The result should be the element-wise difference of the two matrices" )
			{
				CHECK_EQ( result.at( 0 ), 4.0f );
				CHECK_EQ( result.at( 1 ), 4.0f );
				CHECK_EQ( result.at( 2 ), 4.0f );
				CHECK_EQ( result.at( 3 ), 4.0f );
			}
		}
	}
}

SCENARIO( "Testing matrix multiplication" )
{
	GIVEN( "Two 2x2 matrices" )
	{
		pbl::math::Matrix2x2< float > matrix1{ 1.0f, 2.0f, 3.0f, 4.0f };
		pbl::math::Matrix2x2< float > matrix2{ 2.0f, 0.0f, 1.0f, 2.0f };

		WHEN( "We multiply the two matrices" )
		{
			auto result = matrix1 * matrix2;

			THEN( "The result should be the matrix product of the two matrices" )
			{
				CHECK_EQ( result.at( 0 ), 4.0f );
				CHECK_EQ( result.at( 1 ), 4.0f );
				CHECK_EQ( result.at( 2 ), 10.0f );
				CHECK_EQ( result.at( 3 ), 8.0f );
			}
		}
	}
}

SCENARIO( "Adding a scalar to each element of a 2x2 matrix" )
{
	GIVEN( "A 2x2 matrix initialized with specific values and a scalar to add" )
	{
		pbl::math::Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };
		float scalar = 2.0f;

		WHEN( "The scalar is added to each element in a new matrix" )
		{
			matrix = matrix + scalar;

			THEN( "Each element in the new matrix should be increased by the scalar value" )
			{
				CHECK_EQ( matrix.at( 0 ), 3.0f );
				CHECK_EQ( matrix.at( 1 ), 4.0f );
				CHECK_EQ( matrix.at( 2 ), 5.0f );
				CHECK_EQ( matrix.at( 3 ), 6.0f );
			}
		}
	}

	GIVEN( "A 2x2 matrix initialized with specific values and a scalar to add (in-place)" )
	{
		pbl::math::Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };
		float scalar = 2.0f;

		WHEN( "The scalar is added in-place to each element of the matrix" )
		{
			matrix += scalar;

			THEN( "Each element of the matrix should be increased by the scalar value in place" )
			{
				CHECK_EQ( matrix.at( 0 ), 3.0f );
				CHECK_EQ( matrix.at( 1 ), 4.0f );
				CHECK_EQ( matrix.at( 2 ), 5.0f );
				CHECK_EQ( matrix.at( 3 ), 6.0f );
			}
		}
	}
}

SCENARIO( "Testing scalar multiplication" )
{
	GIVEN( "A 2x2 matrix and a scalar value" )
	{
		pbl::math::Matrix2x2< float > matrix{ 1.0f, 2.0f, 3.0f, 4.0f };

		WHEN( "We multiply the scalar to the matrix" )
		{
			matrix = matrix * 2.0f;

			THEN( "Each element of the matrix should be increased by the scalar value" )
			{
				CHECK_EQ( matrix.at( 0 ), 2.0f );
				CHECK_EQ( matrix.at( 1 ), 4.0f );
				CHECK_EQ( matrix.at( 2 ), 6.0f );
				CHECK_EQ( matrix.at( 3 ), 8.0f );
			}
		}
	}
}
