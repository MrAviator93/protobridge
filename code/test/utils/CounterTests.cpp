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
	EXPECT_EQ( BasicTracked::count(), 0 );
	{
		BasicTracked a;
		EXPECT_EQ( BasicTracked::count(), 1 );
	}
	EXPECT_EQ( BasicTracked::count(), 0 );
}

TEST( CounterTest, MultipleInstancesTrackCorrectly )
{
	EXPECT_EQ( BasicTracked::count(), 0 );
	{
		BasicTracked a, b, c;
		EXPECT_EQ( BasicTracked::count(), 3 );
	}
	EXPECT_EQ( BasicTracked::count(), 0 );
}

TEST( CounterTest, CopyConstructionIncrementsCount )
{
	{
		BasicTracked a;
		std::size_t count_before = BasicTracked::count();
		BasicTracked b = a;
		EXPECT_EQ( BasicTracked::count(), count_before + 1 );
	}
	EXPECT_EQ( BasicTracked::count(), 0 );
}

TEST( CounterTest, MoveConstructionIncrementsCount )
{
	{
		BasicTracked a;
		std::size_t count_before = BasicTracked::count();
		BasicTracked b = std::move( a );
		EXPECT_EQ( BasicTracked::count(), count_before + 1 );
	}
	EXPECT_EQ( BasicTracked::count(), 0 );
}

TEST( CounterTest, CopyAssignmentDoesNotChangeCount )
{
	BasicTracked a, b;
	EXPECT_EQ( BasicTracked::count(), 2 );
	b = a;
	EXPECT_EQ( BasicTracked::count(), 2 );
}

TEST( CounterTest, MoveAssignmentDoesNotChangeCount )
{
	BasicTracked a, b;
	EXPECT_EQ( BasicTracked::count(), 2 );
	b = std::move( a );
	EXPECT_EQ( BasicTracked::count(), 2 );
}

TEST( CounterTest, DestructorDecrementsCount )
{
	EXPECT_EQ( BasicTracked::count(), 0 );
	{
		BasicTracked a, b;
		EXPECT_EQ( BasicTracked::count(), 2 );
	}
	EXPECT_EQ( BasicTracked::count(), 0 );
}

} // namespace pbl::utils
