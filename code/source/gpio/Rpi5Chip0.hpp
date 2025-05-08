#ifndef PBL_GPIO_RPI_5_CHIP_0_HPP__
#define PBL_GPIO_RPI_5_CHIP_0_HPP__

#include "GpioLine.hpp"

// C++
#include <memory>
#include <expected>
#include <functional>
#include <string_view>

namespace pbl::gpio
{

inline namespace v1
{

class Rpi5Chip0 final
{
	struct Impl;
	static constexpr std::string_view kChipName{ "gpiochip0" };
	static constexpr std::size_t kGpioLineCount = 22;

public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

	/// Usable GPIOs from the 40-pin header (BCM numbers)
	enum class Pin : std::uint8_t
	{
		GPIO2 = 2,
		GPIO3 = 3,
		GPIO4 = 4,
		GPIO5 = 5,
		GPIO6 = 6,
		GPIO7 = 7,
		GPIO8 = 8,
		GPIO9 = 9,
		GPIO10 = 10,
		GPIO11 = 11,
		GPIO12 = 12,
		GPIO13 = 13,
		GPIO14 = 14,
		GPIO15 = 15,
		GPIO16 = 16,
		GPIO17 = 17,
		GPIO18 = 18,
		GPIO19 = 19,
		GPIO20 = 20,
		GPIO21 = 21,
		GPIO22 = 22,
		GPIO23 = 23,
		GPIO24 = 24,
		GPIO25 = 25,
		GPIO26 = 26,
		GPIO27 = 27
	};

	Rpi5Chip0();
	~Rpi5Chip0();

	[[nodiscard]] bool isReady() const noexcept;

	/// TBW
	[[nodiscard]] Result< std::reference_wrapper< GpioLine > >
	line( Pin pin, GpioLine::Direction direction = GpioLine::Direction::Output );

private:
	std::unique_ptr< Impl > m_pImpl;
};

} // namespace v1
} // namespace pbl::gpio
#endif // PBL_GPIO_RPI_5_CHIP_0_HPP__
