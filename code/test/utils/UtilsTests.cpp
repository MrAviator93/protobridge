// PBL
#include <utils/Utils.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( UtilsToggleTest, ToggleAlternatesBetweenValues )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto toggleResult = toggle( 1, 1, 2 );
	const auto toggleResult2 = toggle( 2, 1, 2 );
	const auto toggleAABResult = toggle( 'a', 'a', 'b' );
	const auto toggleBABResult = toggle( 'b', 'a', 'b' );

	// Assert
	EXPECT_EQ( toggleResult, 2 );
	EXPECT_EQ( toggleResult2, 1 );
	EXPECT_EQ( toggleAABResult, 'b' );
	EXPECT_EQ( toggleBABResult, 'a' );
}

TEST( UtilsToggleTest, ToggleReturnsBIfCurrentNotA )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto toggleResult = toggle( 5, 3, 4 );

	// Assert
	EXPECT_EQ( toggleResult, 3 );
}

TEST( UtilsToggleStructTest, ToggleFunctorBehavior )
{
	// Arrange
	Toggle< int > toggler{ 10, 20 };

	// Act
	const auto togglerResult = toggler( 10 );
	const auto togglerResult2 = toggler( 20 );
	const auto togglerResult3 = toggler( 15 );

	// Assert
	EXPECT_EQ( togglerResult, 20 );
	EXPECT_EQ( togglerResult2, 10 );
	EXPECT_EQ( togglerResult3, 10 );
}

TEST( UtilsSelectTest, SelectReturnsCorrectValue )
{
	// Arrange
	// No additional setup required.

	// Act
	const auto selectResult = select( true, 42, 99 );
	const auto selectResult2 = select( false, 42, 99 );
	const auto selectXYResult = select( true, 'x', 'y' );
	const auto selectXYResult2 = select( false, 'x', 'y' );

	// Assert
	EXPECT_EQ( selectResult, 42 );
	EXPECT_EQ( selectResult2, 99 );
	EXPECT_EQ( selectXYResult, 'x' );
	EXPECT_EQ( selectXYResult2, 'y' );
}

TEST( UtilsSelectStructTest, SelectFunctorBehavior )
{
	// Arrange
	Select< std::string > selector{ "hello", "world" };

	// Act
	const auto selectorResult = selector( true );
	const auto selectorResult2 = selector( false );

	// Assert
	EXPECT_EQ( selectorResult, "hello" );
	EXPECT_EQ( selectorResult2, "world" );
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
