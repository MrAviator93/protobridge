#include "BMP388Controller.hpp"
#include "BusController.hpp"

#include <math/Math.hpp>
#include <math/Dynamics.hpp>

namespace pbl::i2c
{

namespace
{

constexpr std::uint8_t kChipIdReg = 0x00; // Should return 0x50
constexpr std::uint8_t kResetReg = 0xE0; // Soft reset
constexpr std::uint8_t kStatusReg = 0x03; // Status flags
constexpr std::uint8_t kDataReg = 0x04; // Data register start (pressure + temp)
constexpr std::uint8_t kPwrCtrlReg = 0x1B; // Power control
constexpr std::uint8_t kOsrReg = 0x1C; // Oversampling control
constexpr std::uint8_t kOdrReg = 0x1D; // Output data rate
constexpr std::uint8_t kConfigReg = 0x1F; // IIR filter settings
constexpr std::uint8_t kCalibDataStart = 0x31; // Calibration data range

constexpr std::uint8_t kExpectedChipId = 0x50;

[[nodiscard]] constexpr float compensateTemperature( std::int32_t rawTemperature, const std::array< float, 3 >& t_cal ) noexcept
{
	const float pd1 = static_cast<float>(rawTemperature) / 16384.0f - t_cal[ 0 ];
	const float pd2 = pd1 * t_cal[ 1 ];
	return pd2 + t_cal[ 2 ];
}

[[nodiscard]] constexpr float compensatePressure( std::int32_t rawPressure, float temp, const std::array< float, 9 >& p_cal ) noexcept
{
	const float pd1 = p_cal[ 0 ] + p_cal[ 1 ] * temp + p_cal[ 2 ] * temp * temp + p_cal[ 3 ] * temp * temp * temp;
	const float pd2 = static_cast< float >( rawPressure ) / 1048576.0f;
	const float pd3 = pd2 * pd2;
	const float pd4 = pd3 * pd2;
	return pd1 + pd2 * p_cal[ 4 ] + pd3 * p_cal[ 5 ] + pd4 * p_cal[ 6 ] + pd2 * pd2 * p_cal[ 7 ] +
		   pd2 * pd4 * p_cal[ 8 ];
}

} // namespace

} // namespace pbl::i2c
