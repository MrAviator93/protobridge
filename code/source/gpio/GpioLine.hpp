#ifndef PBL_GPIO_GPIO_LINE_HPP__
#define PBL_GPIO_GPIO_LINE_HPP__

#include "GpioFwd.hpp"
#include <utils/ErrorCode.hpp>

// C++
#include <cstdint>

namespace pbl::gpio
{

class GpioLine
{
public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

	enum class Direction
	{
		Input,
		Output
	};

private:
	GpioLine( const GpioLine& ) = delete;
	GpioLine& operator=( const GpioLine& ) = delete;

private:
	gpiod_line* m_pLine{ nullptr };
	std::int32_t m_lineNumber{};
};

} // namespace pbl::gpio
#endif // PBL_GPIO_GPIO_LINE_HPP__