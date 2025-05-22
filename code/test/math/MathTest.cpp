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
	double degrees = 0.0;
	double radians = degreesToRadians( degrees );
	EXPECT_NEAR( radians, 0.0, kEpsilon );
}

TEST( AngleConversionTest, DegreesToRadians_Ninety )
{
	double degrees = 90.0;
	double radians = degreesToRadians( degrees );
	EXPECT_NEAR( radians, constants::Pi< double > / 2.0, kEpsilon );
}

TEST( AngleConversionTest, DegreesToRadians_OneEighty )
{
	double degrees = 180.0;
	double radians = degreesToRadians( degrees );
	EXPECT_NEAR( radians, constants::Pi< double >, kEpsilon );
}

// Radians to Degrees Tests
TEST( AngleConversionTest, RadiansToDegrees_Zero )
{
	double radians = 0.0;
	double degrees = radiansToDegrees( radians );
	EXPECT_NEAR( degrees, 0.0, kEpsilon );
}

TEST( AngleConversionTest, RadiansToDegrees_PiOverTwo )
{
	double radians = constants::Pi< double > / 2.0;
	double degrees = radiansToDegrees( radians );
	EXPECT_NEAR( degrees, 90.0, kEpsilon );
}

TEST( AngleConversionTest, RadiansToDegrees_Pi )
{
	double radians = constants::Pi< double >;
	double degrees = radiansToDegrees( radians );
	EXPECT_NEAR( degrees, 180.0, kEpsilon );
}

} // namespace pbl::math
