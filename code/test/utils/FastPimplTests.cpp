// PBL
#include <utils/FastPimpl.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{
namespace
{

struct SimpleType
{
	int x;
	std::string name;

	SimpleType( int val, std::string n )
		: x( val )
		, name( std::move( n ) )
	{ }

	int doubleX() const { return 2 * x; }
};

constexpr std::size_t SimpleTypeSize = sizeof( SimpleType );
constexpr std::size_t SimpleTypeAlignment = alignof( SimpleType );

static bool destroyed = false;

struct TrackDestruction
{
	~TrackDestruction() { destroyed = true; }
};


} // namespace

TEST( FastPimplTest, ConstructAndAccess )
{
	FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > pimpl( 42, "Test" );
	EXPECT_NE( pimpl.get(), nullptr );
	EXPECT_EQ( pimpl->x, 42 );
	EXPECT_EQ( pimpl->name, "Test" );
	EXPECT_EQ( pimpl->doubleX(), 84 );
}

TEST( FastPimplTest, ConstAccess )
{
	const FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > pimpl( 10, "Const" );
	EXPECT_EQ( pimpl->x, 10 );
	EXPECT_EQ( pimpl->name, "Const" );
	EXPECT_EQ( pimpl->doubleX(), 20 );
}

TEST( FastPimplTest, StorageAlignmentAndSizeMatch )
{
	static_assert( sizeof( FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > ) == SimpleTypeSize,
				   "Size mismatch" );
	static_assert( alignof( FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > ) == SimpleTypeAlignment,
				   "Alignment mismatch" );
}

TEST( FastPimplTest, CompileTimeValidationFailsOnWrongSize )
{
	// The following static_assert is commented out to avoid compile error during normal build.
	// Uncomment to test behavior manually.
	// FastPimpl<SimpleType, SimpleTypeSize - 1, SimpleTypeAlignment> invalidSizePimpl(1, "fail");
}

TEST( FastPimplTest, CompileTimeValidationFailsOnWrongAlignment )
{
	// The following static_assert is commented out to avoid compile error during normal build.
	// Uncomment to test behavior manually.
	// FastPimpl<SimpleType, SimpleTypeSize, SimpleTypeAlignment - 1> invalidAlignmentPimpl(1, "fail");
}

TEST( FastPimplTest, DestructorCleansUpResources )
{
	destroyed = false;
	{
		FastPimpl< TrackDestruction, sizeof( TrackDestruction ), alignof( TrackDestruction ) > temp;
		EXPECT_FALSE( destroyed );
	}
	EXPECT_TRUE( destroyed );
}

} // namespace pbl::utils
