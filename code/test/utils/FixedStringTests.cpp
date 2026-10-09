// PBL
#include <utils/FixedString.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( FixedStringTest, ConstructFromStringLiteral )
{
	// Arrange
	// No additional setup required.

	// Act
	constexpr FixedString str( "hello" );
	const auto strSizeResult = str.size();

	// Assert
	EXPECT_EQ( strSizeResult, 5 );
	EXPECT_STREQ( str, "hello" );
}

TEST( FixedStringTest, EqualityFixedString )
{
	// Arrange
	constexpr FixedString a{ "test" };
	constexpr FixedString b{ "test" };

	// Act
	const auto aBResult = a == b;
	const auto aBResult2 = a != b;

	// Assert
	EXPECT_TRUE( aBResult );
	EXPECT_FALSE( aBResult2 );
}

TEST( FixedStringTest, InequalityFixedStringDifferentSizes )
{
	// Arrange
	constexpr FixedString a( "short" );
	constexpr FixedString b( "longer" );

	// Act
	const auto aBResult = a == b;
	const auto aBResult2 = a != b;

	// Assert
	EXPECT_FALSE( aBResult );
	EXPECT_TRUE( aBResult2 );
}

TEST( FixedStringTest, EqualityCString )
{
	// Arrange
	constexpr FixedString a( "sample" );

	// Act
	const auto aSampleResult = a == "sample";
	const auto aSampleResult2 = a != "sample";
	const auto aOtherResult = a == "other";
	const auto aOtherResult2 = a != "other";

	// Assert
	EXPECT_TRUE( aSampleResult );
	EXPECT_FALSE( aSampleResult2 );
	EXPECT_FALSE( aOtherResult );
	EXPECT_TRUE( aOtherResult2 );
}

TEST( FixedStringTest, EqualityStdString )
{
	// Arrange
	constexpr FixedString a( "stdstr" );
	std::string b = "stdstr";

	// Act
	const auto aBResult = a == b;
	const auto aBResult2 = a != b;
	const auto aStringWrongResult = a == std::string( "wrong" );
	const auto aStringWrongResult2 = a != std::string( "wrong" );

	// Assert
	EXPECT_TRUE( aBResult );
	EXPECT_FALSE( aBResult2 );
	EXPECT_FALSE( aStringWrongResult );
	EXPECT_TRUE( aStringWrongResult2 );
}

TEST( FixedStringTest, ConversionOperators )
{
	// Arrange
	constexpr FixedString str( "convert" );
	const char* cstr = str;
	std::string_view sv = str;

	// Act
	std::string s = std::string( str );

	// Assert
	EXPECT_STREQ( cstr, "convert" );
	EXPECT_EQ( sv, "convert" );
	EXPECT_EQ( s, "convert" );
}

TEST( FixedStringTest, Iterators )
{
	// Arrange
	constexpr FixedString str( "abc" );
	std::string collected;

	// Act
	for( auto ch : str )
	{
		collected += ch;
	}

	// Assert
	EXPECT_EQ( collected, "abc" );
}

TEST( FixedStringTest, FrontBackAccess )
{
	// Arrange
	constexpr FixedString str( "openai" );

	// Act
	const auto first = str.front();
	const auto last = str.back();

	// Assert
	EXPECT_EQ( first, 'o' );
	EXPECT_EQ( last, 'i' );
}

TEST( FixedStringTest, OperatorBracketValidIndex )
{
	// Arrange
	constexpr FixedString str( "index" );

	// Act
	const auto strResult = str[ 0 ];
	const auto strResult2 = str[ 4 ];

	// Assert
	EXPECT_EQ( strResult, 'i' );
	EXPECT_EQ( strResult2, 'x' );
}

TEST( FixedStringTest, OperatorBracketOutOfRangeThrows )
{
	// Arrange
	constexpr FixedString str( "guard" );

	// Act
	const auto operation1 = [ & ] { std::ignore = str[ 5 ]; };
	const auto operation2 = [ & ] { std::ignore = str[ 100 ]; };

	// Assert
	EXPECT_THROW( operation1(), std::out_of_range );
	EXPECT_THROW( operation2(), std::out_of_range );
}

TEST( FixedStringTest, OperatorBracketWithErrorCodeValid )
{
	// Arrange
	constexpr FixedString str( "error" );
	std::error_code ec;

	// Act
	char c = str[ 1, ec ];

	// Assert
	EXPECT_EQ( c, 'r' );
	EXPECT_FALSE( ec ); // ec should be cleared
}

TEST( FixedStringTest, OperatorBracketWithErrorCodeOutOfRange )
{
	// Arrange
	constexpr FixedString str( "oops" );
	std::error_code ec;

	// Act
	char c = str[ 5, ec ];

	// Assert
	EXPECT_EQ( c, '\0' );
	EXPECT_TRUE( ec );
	EXPECT_EQ( ec, std::make_error_code( std::errc::result_out_of_range ) );
}

TEST( FixedStringTest, AtValid )
{
	// Arrange
	constexpr FixedString str( "valid" );

	// Act
	const auto strAtResult = str.at( 3 );

	// Assert
	EXPECT_EQ( strAtResult, 'i' );
}

TEST( FixedStringTest, AtThrowsOutOfRange )
{
	// Arrange
	constexpr FixedString str( "fail" );

	// Act
	const auto operation1 = [ & ] { std::ignore = str.at( 10 ); };

	// Assert
	EXPECT_THROW( operation1(), std::out_of_range );
}

TEST( FixedStringTest, AtWithErrorCodeValid )
{
	// Arrange
	constexpr FixedString str( "error" );
	std::error_code ec;

	// Act
	const auto strAtEcResult = str.at( 2, ec );

	// Assert
	EXPECT_EQ( strAtEcResult, 'r' );
	EXPECT_FALSE( ec );
}

TEST( FixedStringTest, AtWithErrorCodeOutOfRange )
{
	// Arrange
	constexpr FixedString str( "check" );
	std::error_code ec;

	// Act
	char result = str.at( 10, ec );

	// Assert
	EXPECT_EQ( result, '\0' );
	EXPECT_TRUE( ec );
	EXPECT_EQ( ec, std::make_error_code( std::errc::result_out_of_range ) );
}

TEST( FixedStringTest, EmptyString )
{
	// Arrange
	// No additional setup required.

	// Act
	constexpr FixedString< 0 > empty( "" );
	const auto emptyEmptyResult = empty.empty();
	const auto emptySizeResult = empty.size();

	// Assert
	EXPECT_TRUE( emptyEmptyResult );
	EXPECT_EQ( emptySizeResult, 0 );
}

TEST( FixedStringTest, ConstexprAtIndexTemplate )
{
	// Arrange
	constexpr FixedString str( "constexpr" );

	// Act
	constexpr char c = str.at< 4 >();

	// Assert
	static_assert( c == 't', "Expected character 't'" );
}

TEST( FixedStringTest, ThreeWayComparison )
{
	// Arrange
	constexpr FixedString a( "apple" );
	constexpr FixedString b( "banana" );

	// Act
	const auto aBResult = ( a <=> b ) < 0;
	const auto bAResult = ( b <=> a ) > 0;
	const auto aAResult = ( a <=> a ) == 0;

	// Assert
	EXPECT_TRUE( aBResult );
	EXPECT_TRUE( bAResult );
	EXPECT_TRUE( aAResult );
}

} // namespace pbl::utils
