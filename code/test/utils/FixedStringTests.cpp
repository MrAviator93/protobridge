// PBL
#include <utils/FixedString.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

TEST( FixedStringTest, ConstructFromStringLiteral )
{
	constexpr FixedString str( "hello" );
	EXPECT_EQ( str.size(), 5 );
	EXPECT_STREQ( str, "hello" );
}

TEST( FixedStringTest, EqualityFixedString )
{
	constexpr FixedString a{ "test" };
	constexpr FixedString b{ "test" };
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );
}

TEST( FixedStringTest, InequalityFixedStringDifferentSizes )
{
	constexpr FixedString a( "short" );
	constexpr FixedString b( "longer" );
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
}

TEST( FixedStringTest, EqualityCString )
{
	constexpr FixedString a( "sample" );
	EXPECT_TRUE( a == "sample" );
	EXPECT_FALSE( a != "sample" );
	EXPECT_FALSE( a == "other" );
	EXPECT_TRUE( a != "other" );
}

TEST( FixedStringTest, EqualityStdString )
{
	constexpr FixedString a( "stdstr" );
	std::string b = "stdstr";
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );
	EXPECT_FALSE( a == std::string( "wrong" ) );
	EXPECT_TRUE( a != std::string( "wrong" ) );
}

TEST( FixedStringTest, ConversionOperators )
{
	constexpr FixedString str( "convert" );
	const char* cstr = str;
	EXPECT_STREQ( cstr, "convert" );
	std::string_view sv = str;
	EXPECT_EQ( sv, "convert" );
	std::string s = std::string( str );
	EXPECT_EQ( s, "convert" );
}

TEST( FixedStringTest, Iterators )
{
	constexpr FixedString str( "abc" );
	std::string collected;
	for( auto ch : str )
		collected += ch;
	EXPECT_EQ( collected, "abc" );
}

TEST( FixedStringTest, FrontBackAccess )
{
	constexpr FixedString str( "openai" );
	EXPECT_EQ( str.front(), 'o' );
	EXPECT_EQ( str.back(), 'i' );
}

TEST( FixedStringTest, OperatorBracketValidIndex )
{
	constexpr FixedString str( "index" );
	EXPECT_EQ( str[ 0 ], 'i' );
	EXPECT_EQ( str[ 4 ], 'x' );
}

TEST( FixedStringTest, OperatorBracketOutOfRangeThrows )
{
	constexpr FixedString str( "guard" );
	EXPECT_THROW( std::ignore = str[ 5 ], std::out_of_range );
	EXPECT_THROW( std::ignore = str[ 100 ], std::out_of_range );
}

TEST( FixedStringTest, OperatorBracketWithErrorCodeValid )
{
	constexpr FixedString str( "error" );
	std::error_code ec;
	char c = str[ 1, ec ];
	EXPECT_EQ( c, 'r' );
	EXPECT_FALSE( ec ); // ec should be cleared
}

TEST( FixedStringTest, OperatorBracketWithErrorCodeOutOfRange )
{
	constexpr FixedString str( "oops" );
	std::error_code ec;
	char c = str[ 5, ec ];
	EXPECT_EQ( c, '\0' );
	EXPECT_TRUE( ec );
	EXPECT_EQ( ec, std::make_error_code( std::errc::result_out_of_range ) );
}

TEST( FixedStringTest, AtValid )
{
	constexpr FixedString str( "valid" );
	EXPECT_EQ( str.at( 3 ), 'i' );
}

TEST( FixedStringTest, AtThrowsOutOfRange )
{
	constexpr FixedString str( "fail" );
	EXPECT_THROW( std::ignore = str.at( 10 ), std::out_of_range );
}

TEST( FixedStringTest, AtWithErrorCodeValid )
{
	constexpr FixedString str( "error" );
	std::error_code ec;
	EXPECT_EQ( str.at( 2, ec ), 'r' );
	EXPECT_FALSE( ec );
}

TEST( FixedStringTest, AtWithErrorCodeOutOfRange )
{
	constexpr FixedString str( "check" );
	std::error_code ec;
	char result = str.at( 10, ec );
	EXPECT_EQ( result, '\0' );
	EXPECT_TRUE( ec );
	EXPECT_EQ( ec, std::make_error_code( std::errc::result_out_of_range ) );
}

TEST( FixedStringTest, EmptyString )
{
	constexpr FixedString< 0 > empty( "" );
	EXPECT_TRUE( empty.empty() );
	EXPECT_EQ( empty.size(), 0 );
}

TEST( FixedStringTest, ConstexprAtIndexTemplate )
{
	constexpr FixedString str( "constexpr" );
	constexpr char c = str.at< 4 >();
	static_assert( c == 't', "Expected character 't'" );
}

TEST( FixedStringTest, ThreeWayComparison )
{
	constexpr FixedString a( "apple" );
	constexpr FixedString b( "banana" );
	EXPECT_TRUE( ( a <=> b ) < 0 );
	EXPECT_TRUE( ( b <=> a ) > 0 );
	EXPECT_TRUE( ( a <=> a ) == 0 );
}

} // namespace pbl::utils
