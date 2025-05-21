#ifndef PBL_SERIAL_BAUD_RATE_HPP__
#define PBL_SERIAL_BAUD_RATE_HPP__

// C++
#include <cstdint>
#include <compare>

namespace pbl::serial
{

class BaudRate
{
public:
	/// All supported baudrate types TODO: Rename to ????
	enum class Type : std::uint32_t
	{
		BR_110 = 110,
		BR_300 = 300,
		BR_600 = 600,
		BR_1200 = 1'200,
		BR_2400 = 2'400,
		BR_4800 = 4'800,
		BR_9600 = 9'600,
		BR_14400 = 14'400,
		BR_19200 = 19'200,
		BR_38400 = 38'400,
		BR_56000 = 56'000,
		BR_57600 = 57'600,
		BR_115200 = 115'200,
		BR_128000 = 128'000,
		BR_256000 = 256'000
	};

	using enum Type;

	/// Constructs a default baud rate of 9'600
	constexpr BaudRate() noexcept
		: m_value{ static_cast< std::uint32_t >( Type::BR_9600 ) }
		, m_isStandard{ true }
	{ }

	/// Constructs from a standard enum type
	constexpr explicit BaudRate( Type type ) noexcept
		: m_value{ static_cast< std::uint32_t >( type ) }
		, m_isStandard{ true }
	{ }

	/// Returns true if this is a standard baud rate
	[[nodiscard]] constexpr bool isStandard() const noexcept { return m_isStandard; }

	/// Returns the numeric baud rate
	[[nodiscard]] constexpr std::uint32_t value() const noexcept { return m_value; }

	/// Converts to raw integer automatically
	constexpr operator std::uint32_t() const noexcept { return m_value; }

	/// Comparison
	constexpr auto operator<=>( const BaudRate& ) const = default;

private:
	std::uint32_t m_value{};
	bool m_isStandard{ false };
};

} // namespace pbl::serial
#endif // PBL_SERIAL_BAUD_RATE_HPP__
