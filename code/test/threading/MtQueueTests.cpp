// PBL
#include <threading/MtQueue.hpp>

// C++
#include <thread>

// Third Party
#include <gtest/gtest.h>

namespace pbl::threading
{

TEST( MtQueueTests, DefaultConstructorCreatesEmptyQueue )
{
	// Arrange
	MtQueue< int > queue;

	// Act & Assert
	EXPECT_TRUE( queue.empty() );
	EXPECT_EQ( queue.size(), 0u );
}

TEST( MtQueueTests, SizeConstructorInitializesWithDefaultValues )
{
	// Arrange
	MtQueue< int > queue( 5 );

	// Assert
	EXPECT_EQ( queue.size(), 5u );
}

TEST( MtQueueTests, InitializerListConstructorWorks )
{
	// Arrange
	MtQueue< int > queue{ 1, 2, 3 };

	// Act
	auto values = queue.get( 3 );

	// Assert
	EXPECT_EQ( queue.size(), 0u );
	EXPECT_EQ( values, ( std::vector< int >{ 1, 2, 3 } ) );
}

TEST( MtQueueTests, PushAndGetSingleValue )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	queue.push( 42 );
	auto result = queue.get();

	// Assert
	EXPECT_TRUE( result.has_value() );
	EXPECT_EQ( result.value(), 42 );
	EXPECT_TRUE( queue.empty() );
}

TEST( MtQueueTests, GetFromEmptyQueueReturnsNullopt )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	auto result = queue.get();

	// Assert
	EXPECT_FALSE( result.has_value() );
}

TEST( MtQueueTests, BulkGetReturnsCorrectNumberOfElements )
{
	// Arrange
	MtQueue< int > queue;
	for( int i = 0; i < 10; ++i )
		queue.push( i );

	// Act
	auto values = queue.get( 5 );

	// Assert
	EXPECT_EQ( values.size(), 5u );
	EXPECT_EQ( values, ( std::vector< int >{ 0, 1, 2, 3, 4 } ) );
	EXPECT_EQ( queue.size(), 5u );
}

TEST( MtQueueTests, ClearEmptiesTheQueue )
{
	// Arrange
	MtQueue< int > queue;
	queue.push( 1 );
	queue.push( 2 );

	// Act
	queue.clear();

	// Assert
	EXPECT_TRUE( queue.empty() );
}

TEST( MtQueueTests, MoveConstructorPreservesElements )
{
	// Arrange
	MtQueue< int > queue;
	queue.push( 100 );

	// Act
	MtQueue< int > movedQueue( std::move( queue ) );
	auto val = movedQueue.get();

	// Assert
	EXPECT_EQ( movedQueue.size(), 0u );
	ASSERT_TRUE( val.has_value() );
	EXPECT_EQ( val.value(), 100 );
}

TEST( MtQueueTests, CopyConstructorCreatesValidCopy )
{
	// Arrange
	MtQueue< int > queue;
	queue.push( 7 );

	// Act
	MtQueue< int > copiedQueue( queue );
	auto val = copiedQueue.get();

	// Assert
	EXPECT_EQ( copiedQueue.size(), 0u );
	ASSERT_TRUE( val.has_value() );
	EXPECT_EQ( val.value(), 7 );
}

TEST( MtQueueTests, ThreadSafetyUnderConcurrentPush )
{
	// Arrange
	MtQueue< int > queue;
	constexpr int numThreads = 4;
	constexpr int numElementsPerThread = 100;

	auto pushJob = [ &queue ] {
		for( int i = 0; i < numElementsPerThread; ++i )
		{
			queue.push( i );
		}
	};

	std::vector< std::thread > threads;

	// Act
	for( int i = 0; i < numThreads; ++i )
	{
		threads.emplace_back( pushJob );
	}

	for( auto& t : threads )
	{
		t.join();
	}

	// Assert
	EXPECT_EQ( queue.size(), numThreads * numElementsPerThread );
}

} // namespace pbl::threading
