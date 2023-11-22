#ifndef PBL_I2C_BME680_CONTROLLER_HPP__
#define PBL_I2C_BME680_CONTROLLER_HPP__

// C++
#include <memory>
#include <string>
#include <cstdint>

namespace PBL::I2C
{
class BusController;

class BME680Controller final
{
	struct BME680Device;

public:
	struct BME680CalibData;

	explicit BME680Controller( BusController& busController, std::uint8_t address = 0x77 );

	~BME680Controller();

	[[nodiscard]] float temperature();
	[[nodiscard]] float pressure();
	[[nodiscard]] float humidity();
	[[nodiscard]] float voc();

	bool readFieldData();

	[[nodiscard]] auto lastError() const { return m_lastError; }

private:
	bool softReset();
	bool retrieveCalibData();
	bool getRegs( std::uint8_t regAddr, std::uint8_t* regData, std::uint16_t dataLen );
	bool setSensorSettings( std::uint16_t desiredSettings );

	bool setSensorMode( std::uint8_t mode );
	bool setGasConfig( std::uint16_t heatrTemp, std::uint16_t ambientTemp, std::uint16_t duration );

	std::int8_t
	boundaryCheck( std::uint8_t& value, const std::uint8_t min, const std::uint8_t max, std::uint8_t& infoMsg );

	bool setRegs( std::uint8_t* regAddr, std::uint8_t* regData, std::uint8_t len );

	void setLastError( const std::string& msg ) { m_lastError = msg; }

private:
	const std::uint8_t m_bme680Address;
	BusController& m_busController;
	std::string m_lastError{ "no errors" };
	std::unique_ptr< BME680CalibData > m_calibData;
};

} // namespace PBL::I2C
#endif // PBL_I2C_BME680_CONTROLLER_HPP__