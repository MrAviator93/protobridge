#ifndef PBL_SPI_MCP3008_CONTROLLER_HPP__
#define PBL_SPI_MCP3008_CONTROLLER_HPP__

#include <utils/Result.hpp>

// C++
#include <array>
#include <cstdint>

namespace pbl::spi
{

inline namespace v1
{

class MCP3008Controller // : public Device
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Channel : std::uint8_t
	{
		CH0 = 0,
		CH1,
		CH2,
		CH3,
		CH4,
		CH5,
		CH6,
		CH7
	};

	/// Differential direction enum (fixed pairs only)
	enum class DiffDirection : std::uint8_t
	{
		CH0_CH1 = 0, // CH0+ CH1-
		CH1_CH0 = 1, // CH1+ CH0-
		CH2_CH3 = 2,
		CH3_CH2 = 3,
		CH4_CH5 = 4,
		CH5_CH4 = 5,
		CH6_CH7 = 6,
		CH7_CH6 = 7
	};

	/// TBW
	[[nodiscard]] static Result< MCP3008Controller > open( const std::string& devPath );

	/// TBW
	[[nodiscard]] Result< float > readVoltage( Channel channel, float vref = 3.3f );

	/// TBW
	[[nodiscard]] Result< float > readDifferential( DiffDirection pair, float vref = 3.3f );

	/// TBW
	[[nodiscard]] Result< std::array< float, 8 > > readAllVoltages( float vref = 3.3f );

private:
	MCP3008Controller( Device&& dev )
		: Device( std::move( dev ) )
	{ }
};

} // namespace v1
} // namespace pbl::spi
#endif // PBL_SPI_MCP3008_CONTROLLER_HPP__
