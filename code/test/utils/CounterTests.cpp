// PBL
#include <utils/Counter.hpp>

// Third Party
#include <gtest/gtest.h>

namespace pbl::utils
{

namespace
{

struct BasicTracked : private Counter< BasicTracked >
{
	using Counter< BasicTracked >::count;
};

} // namespace

TEST( CounterTest, DefaultConstructionIncrementsCount )
{
	// Arrange
	const auto initialCount = BasicTracked::count();
	std::size_t constructedCount{};

	// Act
	{
		BasicTracked instance;
		constructedCount = BasicTracked::count();
	}
	const auto destroyedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( initialCount, 0 );
	EXPECT_EQ( constructedCount, 1 );
	EXPECT_EQ( destroyedCount, 0 );
}

TEST( CounterTest, MultipleInstancesTrackCorrectly )
{
	// Arrange
	const auto initialCount = BasicTracked::count();
	std::size_t constructedCount{};

	// Act
	{
		BasicTracked a, b, c;
		constructedCount = BasicTracked::count();
	}
	const auto destroyedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( initialCount, 0 );
	EXPECT_EQ( constructedCount, 3 );
	EXPECT_EQ( destroyedCount, 0 );
}

TEST( CounterTest, CopyConstructionIncrementsCount )
{
	// Arrange
	std::size_t initialCount{};
	std::size_t copiedCount{};

	// Act
	{
		BasicTracked original;
		initialCount = BasicTracked::count();
		BasicTracked copy = original;
		copiedCount = BasicTracked::count();
	}
	const auto destroyedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( copiedCount, initialCount + 1 );
	EXPECT_EQ( destroyedCount, 0 );
}

TEST( CounterTest, MoveConstructionIncrementsCount )
{
	// Arrange
	std::size_t initialCount{};
	std::size_t movedCount{};

	// Act
	{
		BasicTracked original;
		initialCount = BasicTracked::count();
		BasicTracked moved = std::move( original );
		movedCount = BasicTracked::count();
	}
	const auto destroyedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( movedCount, initialCount + 1 );
	EXPECT_EQ( destroyedCount, 0 );
}

TEST( CounterTest, CopyAssignmentDoesNotChangeCount )
{
	// Arrange
	BasicTracked a, b;
	const auto initialCount = BasicTracked::count();

	// Act
	b = a;
	const auto assignedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( initialCount, 2 );
	EXPECT_EQ( assignedCount, initialCount );
}

TEST( CounterTest, MoveAssignmentDoesNotChangeCount )
{
	// Arrange
	BasicTracked a, b;
	const auto initialCount = BasicTracked::count();

	// Act
	b = std::move( a );
	const auto assignedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( initialCount, 2 );
	EXPECT_EQ( assignedCount, initialCount );
}

TEST( CounterTest, DestructorDecrementsCount )
{
	// Arrange
	const auto initialCount = BasicTracked::count();
	std::size_t liveCount{};

	// Act
	{
		BasicTracked a, b;
		liveCount = BasicTracked::count();
	}
	const auto destroyedCount = BasicTracked::count();

	// Assert
	EXPECT_EQ( initialCount, 0 );
	EXPECT_EQ( liveCount, 2 );
	EXPECT_EQ( destroyedCount, 0 );
}

} // namespace pbl::utils
