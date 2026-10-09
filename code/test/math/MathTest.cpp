// PBL
#include <math/Math.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::math
{

constexpr double kEpsilon = 1e-9;

// Degrees to Radians Tests
TEST( AngleConversionTest, DegreesToRadians_Zero )
{
	// Arrange
	double degrees = 0.0;

	// Act
	double radians = degreesToRadians( degrees );

	// Assert
	EXPECT_NEAR( radians, 0.0, kEpsilon );
}

TEST( AngleConversionTest, DegreesToRadians_Ninety )
{
	// Arrange
	double degrees = 90.0;

	// Act
	double radians = degreesToRadians( degrees );

	// Assert
	EXPECT_NEAR( radians, constants::Pi< double > / 2.0, kEpsilon );
}

TEST( AngleConversionTest, DegreesToRadians_OneEighty )
{
	// Arrange
	double degrees = 180.0;

	// Act
	double radians = degreesToRadians( degrees );

	// Assert
	EXPECT_NEAR( radians, constants::Pi< double >, kEpsilon );
}

// Radians to Degrees Tests
TEST( AngleConversionTest, RadiansToDegrees_Zero )
{
	// Arrange
	double radians = 0.0;

	// Act
	double degrees = radiansToDegrees( radians );

	// Assert
	EXPECT_NEAR( degrees, 0.0, kEpsilon );
}

TEST( AngleConversionTest, RadiansToDegrees_PiOverTwo )
{
	// Arrange
	double radians = constants::Pi< double > / 2.0;

	// Act
	double degrees = radiansToDegrees( radians );

	// Assert
	EXPECT_NEAR( degrees, 90.0, kEpsilon );
}

TEST( AngleConversionTest, RadiansToDegrees_Pi )
{
	// Arrange
	double radians = constants::Pi< double >;

	// Act
	double degrees = radiansToDegrees( radians );

	// Assert
	EXPECT_NEAR( degrees, 180.0, kEpsilon );
}

} // namespace pbl::math
