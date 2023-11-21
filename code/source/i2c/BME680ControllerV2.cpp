#include "BME680ControllerV2.hpp"
#include "I2CBusController.hpp"

// C++
#include <cmath>
#include <thread>
#include <chrono>
#include <algorithm>

// Debug
#include <iostream>


// TODO: There is a newer and better
// library to be used:
// https://github.com/BoschSensortec/BME68x-Sensor-API
extern "C" {
#include "bme680.h"
}

namespace PBL::I2C
{

static I2CBusController* pBusController{ nullptr };

BME680ControllerV2::BME680ControllerV2( I2CBusController& busController, std::uint8_t address )
	: m_busController{ busController }
	, m_bme680Address{ address }
	, m_pBme680{}
{
	pBusController = &busController;

	auto readCallback = []( uint8_t dev_id, uint8_t reg_addr, uint8_t* reg_data, uint16_t len ) -> int8_t {
		auto rslt = pBusController->read( 0x77, reg_addr, reg_data, len );
		return rslt > 0 ? BME680_OK : std::int8_t{ -1 };
	};

	auto writeCallback = []( uint8_t dev_id, uint8_t reg_addr, uint8_t* reg_data, uint16_t len ) -> int8_t {
		auto rslt = pBusController->write( 0x77, reg_addr, reg_data, len );
		return rslt == true ? BME680_OK : std::int8_t{ -1 };
	};

	auto delayCallback = []( std::uint32_t periodMs ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( periodMs ) );
	};

	// Configure the BME680 driver
	m_pBme680->dev_id = BME680_I2C_ADDR_SECONDARY;
	m_pBme680->intf = BME680_I2C_INTF;
	m_pBme680->read = readCallback;
	m_pBme680->write = writeCallback;
	m_pBme680->delay_ms = delayCallback;
	m_pBme680->amb_temp = 25;

	// Initialize the driver
	int8_t rslt{};
	if( rslt = ::bme680_init( m_pBme680.get() ) != BME680_OK )
	{
		std::cerr << "BME680 Initialization Error " << int{ rslt } << std::endl;
	}
	else
	{
		std::cerr << "BME680 Initialized and Ready" << std::endl;
	}

	// Select desired oversampling rates
	m_pBme680->tph_sett.os_hum = BME680_OS_2X;
	m_pBme680->tph_sett.os_pres = BME680_OS_4X;
	m_pBme680->tph_sett.os_temp = BME680_OS_8X;
	m_pBme680->tph_sett.filter = BME680_FILTER_SIZE_3;

	// Set the remaining gas sensor settings and link the heating profile
	m_pBme680->gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
	// Create a ramp heat waveform in 3 steps
	m_pBme680->gas_sett.heatr_temp = 320; // degree Celsius
	m_pBme680->gas_sett.heatr_dur = 150; // milliseconds

	// Set sensor to "always on"
	m_pBme680->power_mode = BME680_FORCED_MODE;

	// Set oversampling settings
	uint8_t required_settings =
		BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

	rslt = ::bme680_set_sensor_settings( required_settings, m_pBme680.get() );

	// Set sensor mode
	rslt = ::bme680_set_sensor_mode( m_pBme680.get() );

	if( rslt != BME680_OK )
	{
		std::cerr << "Configuration has failed" << std::endl;
	}

	// Query minimum sampling period
	::bme680_get_profile_dur( &min_sampling_period, m_pBme680.get() );
}

BME680ControllerV2::~BME680ControllerV2() = default;

void BME680ControllerV2::printMeasurements()
{
	// Sampling results variable
	struct bme680_field_data data;

	// Here we need to check if we are ready to sample
	// withing sampling period range)

	// Query the sample data
	auto rslt = ::bme680_get_sensor_data( &data, m_pBme680.get() );

	auto voc = []( float gasResistance, float humidity ) -> float {
		return std::log( gasResistance ) + 0.04 * ( std::log( gasResistance ) / humidity ) * humidity;
	};

	if( data.status & BME680_NEW_DATA_MSK )
	{
		std::cerr << "sampling period: " << min_sampling_period << std::endl;
		std::cerr << "Temperature [*C]: " << float{ data.temperature } / 100.0f << std::endl;
		std::cerr << "Humidity [%]: " << float{ data.humidity } / 1000.0f << std::endl;
		std::cerr << "Pressure [hPa]: " << float{ data.pressure } / 100.0f << std::endl;
		std::cerr << "Gas Resistance [ohms]: " << float{ data.gas_resistance } << std::endl;

		std::cerr << "Profile index: " << int{ data.gas_index } << std::endl;
		std::cerr << "VOC: " << voc( float{ data.gas_resistance }, float{ data.humidity } / 1000.0f ) << std::endl;

		double humidity = static_cast< float >( data.humidity ) / 1000.0f;
		double temp = static_cast< float >( data.temperature ) / 100.0f;
		double R_gas = static_cast< float >( data.gas_resistance );
		double ph_slope = 0.03;

		double rho_max =
			( 6.112 * 100 * std::exp( ( 17.62 * temp ) / ( 243.12 + temp ) ) ) / ( 461.52 * ( temp + 273.15 ) );
		double hum_abs = humidity * 10 * rho_max;

		double comp_gas = R_gas * std::exp( ph_slope * hum_abs );
		double gas_ceil = comp_gas;

		double AQ = std::min( std::pow( ( comp_gas / gas_ceil ), 2 ), 1.0 ) * 100.0;

		std::cerr << "AQ: " << AQ << std::endl;
	}

	// Trigger the next measurement if you would like to read data out continuously
	if( m_pBme680->power_mode == BME680_FORCED_MODE )
	{
		::bme680_set_sensor_mode( m_pBme680.get() );
	}
}

} // namespace PBL::I2C
