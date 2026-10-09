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
	// No additional setup required.

	// Act
	MtQueue< int > queue;
	const auto queueEmptyResult = queue.empty();
	const auto queueSizeResult = queue.size();

	// Assert
	EXPECT_TRUE( queueEmptyResult );
	EXPECT_EQ( queueSizeResult, 0u );
}

TEST( MtQueueTests, SizeConstructorInitializesWithDefaultValues )
{
	// Arrange
	// No additional setup required.

	// Act
	MtQueue< int > queue( 5 );
	const auto queueSizeResult = queue.size();

	// Assert
	EXPECT_EQ( queueSizeResult, 5u );
}

TEST( MtQueueTests, InitializerListConstructorWorks )
{
	// Arrange
	MtQueue< int > queue{ 1, 2, 3 };

	// Act
	auto values = queue.get( 3 );
	const auto queueSizeResult = queue.size();

	// Assert
	EXPECT_EQ( queueSizeResult, 0u );
	EXPECT_EQ( values, ( std::vector< int >{ 1, 2, 3 } ) );
}

TEST( MtQueueTests, PushAndGetSingleValue )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	queue.push( 42 );
	auto result = queue.get();
	const auto resultHasValueResult = result.has_value();
	const auto queueEmptyResult = queue.empty();

	// Assert
	EXPECT_TRUE( resultHasValueResult );
	EXPECT_EQ( result.value(), 42 );
	EXPECT_TRUE( queueEmptyResult );
}

TEST( MtQueueTests, GetFromEmptyQueueReturnsNullopt )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	auto result = queue.get();
	const auto resultHasValueResult = result.has_value();

	// Assert
	EXPECT_FALSE( resultHasValueResult );
}

TEST( MtQueueTests, BulkGetReturnsCorrectNumberOfElements )
{
	// Arrange
	MtQueue< int > queue;

	for( int i = 0; i < 10; ++i )
	{
		queue.push( i );
	}

	// Act
	auto values = queue.get( 5 );
	const auto valuesSizeResult = values.size();
	const auto queueSizeResult = queue.size();

	// Assert
	EXPECT_EQ( valuesSizeResult, 5u );
	EXPECT_EQ( values, ( std::vector< int >{ 0, 1, 2, 3, 4 } ) );
	EXPECT_EQ( queueSizeResult, 5u );
}

TEST( MtQueueTests, ClearEmptiesTheQueue )
{
	// Arrange
	MtQueue< int > queue;

	queue.push( 1 );
	queue.push( 2 );

	// Act
	queue.clear();
	const auto queueEmptyResult = queue.empty();

	// Assert
	EXPECT_TRUE( queueEmptyResult );
}

TEST( MtQueueTests, MoveConstructorPreservesElements )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	queue.push( 100 );
	MtQueue< int > movedQueue( std::move( queue ) );
	auto val = movedQueue.get();
	const auto movedQueueSizeResult = movedQueue.size();
	const auto valHasValueResult = val.has_value();

	// Assert
	EXPECT_EQ( movedQueueSizeResult, 0u );
	ASSERT_TRUE( valHasValueResult );
	EXPECT_EQ( val.value(), 100 );
}

TEST( MtQueueTests, CopyConstructorCreatesValidCopy )
{
	// Arrange
	MtQueue< int > queue;

	// Act
	queue.push( 7 );
	MtQueue< int > copiedQueue( queue );
	auto val = copiedQueue.get();
	const auto copiedQueueSizeResult = copiedQueue.size();
	const auto valHasValueResult = val.has_value();

	// Assert
	EXPECT_EQ( copiedQueueSizeResult, 0u );
	ASSERT_TRUE( valHasValueResult );
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
	const auto queueSizeResult = queue.size();

	// Assert
	EXPECT_EQ( queueSizeResult, numThreads * numElementsPerThread );
}

} // namespace pbl::threading
