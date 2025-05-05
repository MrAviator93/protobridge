#ifndef PBL_I2C_BMP388_CONTROLLER_HPP__
#define PBL_I2C_BMP388_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <optional>
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

class BMP388Controller final : public ICBase, public utils::Counter< BMP388Controller >
{
public:
	enum class Address : std::uint8_t
	{
		DEFAULT = 0x76, ///< SDO to GND
		ALTERNATIVE = 0x77 ///< SDO to VCC
	};

	enum class PowerMode : std::uint8_t
	{
		SLEEP = 0x00,
		FORCED = 0x01,
		NORMAL = 0x03
	};

	enum class Oversampling : std::uint8_t
	{
		SKIPPED = 0x00,
		X2 = 0x01,
		X4 = 0x02,
		X8 = 0x03,
		X16 = 0x04,
		X32 = 0x05
	};

	enum class IIRFilter : std::uint8_t
	{
		OFF = 0x00,
		S2 = 0x01,
		S4 = 0x02,
		S8 = 0x03,
		S16 = 0x04,
		S32 = 0x05,
		S64 = 0x06,
		S128 = 0x07
	};

	using enum Address;

private:
	BMP388Controller( const BMP388Controller& ) = delete;
	BMP388Controller& operator=( const BMP388Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_BMP388_CONTROLLER_HPP__
