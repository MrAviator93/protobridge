#ifndef PBL_GPIO_GPIO_LINE_HPP__
#define PBL_GPIO_GPIO_LINE_HPP__

#include "GpioFwd.hpp"
#include <utils/Result.hpp>

// C++
#include <cstdint>
#include <expected>

namespace pbl::gpio
{

inline namespace v1
{

class GpioLine
{
	struct PrivateTag
	{ };

public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Direction
	{
		Input,
		Output
	};

	GpioLine( gpiod_line* pLine, std::int32_t lineNumber, PrivateTag )
		: m_pLine{ pLine }
		, m_lineNumber{ lineNumber }
	{ }

	// Move constructor
	GpioLine( GpioLine&& other ) noexcept;

	// Move assignment operator
	GpioLine& operator=( GpioLine&& other ) noexcept;

	~GpioLine();

	void release();

	static Result< GpioLine > open( gpiod_chip* pChip, std::int32_t lineNumber, Direction direction );

private:
	GpioLine( const GpioLine& ) = delete;
	GpioLine& operator=( const GpioLine& ) = delete;

private:
	gpiod_line* m_pLine{ nullptr };
	std::int32_t m_lineNumber{};
};

} // namespace v1
} // namespace pbl::gpio
#endif // PBL_GPIO_GPIO_LINE_HPP__
