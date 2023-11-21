
#ifndef I2C_DEVICES_BME680_CONTROLLER_V2_HPP__
#define I2C_DEVICES_BME680_CONTROLLER_V2_HPP__

// I2C
#include <utils/FastPimpl.hpp>

// C++
#include <cstdint>

struct bme680_dev;

namespace I2C
{

class I2CBusController;

// A wrapper around bme680 library
class BME680ControllerV2 final
{
	static constexpr std::size_t kPointerSize{ sizeof( void* ) };

public:
	explicit BME680ControllerV2( I2CBusController& busController, std::uint8_t address = 0x77 );
	~BME680ControllerV2();

	void printMeasurements();

private:
	I2CBusController& m_busController;
	const std::uint8_t m_bme680Address;

	FastPimpl< bme680_dev, 112, kPointerSize > m_pBme680;
	
	uint16_t min_sampling_period{};
};

} // namespace I2C
#endif // I2C_DEVICES_BME680_CONTROLLER_V2_HPP__
