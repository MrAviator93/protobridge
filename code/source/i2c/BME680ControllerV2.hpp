#ifndef PBL_I2C__BME680_CONTROLLER_V2_HPP__
#define PBL_I2C__BME680_CONTROLLER_V2_HPP__

#include <utils/FastPimpl.hpp>

// C++
#include <cstdint>

struct bme680_dev;

namespace PBL::I2C
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

	Utils::FastPimpl< bme680_dev, 112, kPointerSize > m_pBme680;
	
	uint16_t min_sampling_period{};
};

} // namespace PBL::I2C
#endif // PBL_I2C__BME680_CONTROLLER_V2_HPP__
