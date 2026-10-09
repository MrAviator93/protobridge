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
	// Arrange
	// No additional setup required.

	// Act
	FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > pimpl( 42, "Test" );
	const auto pimplGetResult = pimpl.get();
	const auto pimplXResult = pimpl->x;
	const auto pimplNameResult = pimpl->name;
	const auto pimplDoubleXResult = pimpl->doubleX();

	// Assert
	EXPECT_NE( pimplGetResult, nullptr );
	EXPECT_EQ( pimplXResult, 42 );
	EXPECT_EQ( pimplNameResult, "Test" );
	EXPECT_EQ( pimplDoubleXResult, 84 );
}

TEST( FastPimplTest, ConstAccess )
{
	// Arrange
	// No additional setup required.

	// Act
	const FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > pimpl( 10, "Const" );
	const auto pimplXResult = pimpl->x;
	const auto pimplNameResult = pimpl->name;
	const auto pimplDoubleXResult = pimpl->doubleX();

	// Assert
	EXPECT_EQ( pimplXResult, 10 );
	EXPECT_EQ( pimplNameResult, "Const" );
	EXPECT_EQ( pimplDoubleXResult, 20 );
}

TEST( FastPimplTest, StorageAlignmentAndSizeMatch )
{
	// Arrange
	// No additional setup required.

	// Act
	constexpr auto storageSize = sizeof( FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > );
	constexpr auto storageAlignment = alignof( FastPimpl< SimpleType, SimpleTypeSize, SimpleTypeAlignment > );

	// Assert
	static_assert( storageSize == SimpleTypeSize, "Size mismatch" );
	static_assert( storageAlignment == SimpleTypeAlignment, "Alignment mismatch" );
}

TEST( FastPimplTest, CompileTimeValidationFailsOnWrongSize )
{
	// Arrange
	// Invalid template arguments require a separate compile-failure test.

	// Act
	// No runtime operation can instantiate an intentionally invalid type.

	// Assert
	GTEST_SKIP() << "Requires a compile-failure test harness";
}

TEST( FastPimplTest, CompileTimeValidationFailsOnWrongAlignment )
{
	// Arrange
	// Invalid template arguments require a separate compile-failure test.

	// Act
	// No runtime operation can instantiate an intentionally invalid type.

	// Assert
	GTEST_SKIP() << "Requires a compile-failure test harness";
}

TEST( FastPimplTest, DestructorCleansUpResources )
{
	// Arrange
	destroyed = false;
	bool destroyedWhileAlive{};

	// Act
	{
		FastPimpl< TrackDestruction, sizeof( TrackDestruction ), alignof( TrackDestruction ) > temp;
		destroyedWhileAlive = destroyed;
	}
	const bool destroyedAfterScope = destroyed;

	// Assert
	EXPECT_FALSE( destroyedWhileAlive );
	EXPECT_TRUE( destroyedAfterScope );
}

} // namespace pbl::utils
