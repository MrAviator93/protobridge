
#include <math/Math.hpp>

// 3rd Party
#include <doctest/doctest.h>

SCENARIO( "Converting degrees to radians" )
{
	GIVEN( "A value in degrees" )
	{
		WHEN( "the value is 0" )
		{
			double degrees{ 0.0 };
			double radians = pbl::math::degreesToRadians( degrees );

			THEN( "the result should be 0 radians" )
			{
				CHECK( radians == doctest::Approx( 0.0 ) );
			}
		}

		WHEN( "the value is 90" )
		{
			double degrees{ 90.0 };
			double radians = pbl::math::degreesToRadians( degrees );

			THEN( "the result should be pi/2 radians" )
			{
				CHECK( radians == doctest::Approx( pbl::math::PI / 2.0 ) );
			}
		}

		WHEN( "the value is 180" )
		{
			double degrees{ 180.0 };
			double radians = pbl::math::degreesToRadians( degrees );

			THEN( "the result should be pi radians" )
			{
				CHECK( radians == doctest::Approx( pbl::math::PI ) );
			}
		}
	}
}

SCENARIO( "Converting radians to degrees" )
{
	GIVEN( "A value in radians" )
	{
		WHEN( "the value is 0" )
		{
			double radians{ 0.0 };
			double degrees{ pbl::math::radiansToDegrees( radians ) };

			THEN( "the result should be 0 degrees" )
			{
				CHECK( degrees == doctest::Approx( 0.0 ) );
			}
		}

		WHEN( "the value is pi/2" )
		{
			double radians = pbl::math::PI / 2.0;
			double degrees{ pbl::math::radiansToDegrees( radians ) };

			THEN( "the result should be 90 degrees" )
			{
				CHECK( degrees == doctest::Approx( 90.0 ) );
			}
		}

		WHEN( "the value is pi" )
		{
			double radians = pbl::math::PI;
			double degrees{ pbl::math::radiansToDegrees( radians ) };
			
			THEN( "the result should be 180 degrees" )
			{
				CHECK( degrees == doctest::Approx( 180.0 ) );
			}
		}
	}
}
