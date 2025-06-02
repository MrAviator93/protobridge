// PBL
#include <utils/Utils.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( UtilsToggleTest, ToggleAlternatesBetweenValues )
{
	EXPECT_EQ( toggle( 1, 1, 2 ), 2 );
	EXPECT_EQ( toggle( 2, 1, 2 ), 1 );
	EXPECT_EQ( toggle( 'a', 'a', 'b' ), 'b' );
	EXPECT_EQ( toggle( 'b', 'a', 'b' ), 'a' );
}

TEST( UtilsToggleTest, ToggleReturnsBIfCurrentNotA )
{
	EXPECT_EQ( toggle( 5, 3, 4 ), 3 );
}

TEST( UtilsToggleStructTest, ToggleFunctorBehavior )
{
	Toggle< int > toggler{ 10, 20 };

	EXPECT_EQ( toggler( 10 ), 20 );
	EXPECT_EQ( toggler( 20 ), 10 );
	EXPECT_EQ( toggler( 15 ), 10 );
}

TEST( UtilsSelectTest, SelectReturnsCorrectValue )
{
	EXPECT_EQ( select( true, 42, 99 ), 42 );
	EXPECT_EQ( select( false, 42, 99 ), 99 );

	EXPECT_EQ( select( true, 'x', 'y' ), 'x' );
	EXPECT_EQ( select( false, 'x', 'y' ), 'y' );
}

TEST( UtilsSelectStructTest, SelectFunctorBehavior )
{
	Select< std::string > selector{ "hello", "world" };

	EXPECT_EQ( selector( true ), "hello" );
	EXPECT_EQ( selector( false ), "world" );
}

static_assert( toggle( 1, 1, 2 ) == 2 );
static_assert( toggle( 2, 1, 2 ) == 1 );

constexpr Toggle< int > ctToggler{ 3, 4 };
static_assert( ctToggler( 3 ) == 4 );
static_assert( ctToggler( 4 ) == 3 );

static_assert( select( true, 10, 20 ) == 10 );
static_assert( select( false, 10, 20 ) == 20 );

constexpr Select< int > ctSelector{ 100, 200 };
static_assert( ctSelector( true ) == 100 );
static_assert( ctSelector( false ) == 200 );

} // namespace pbl::utils
