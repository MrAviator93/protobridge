
#include "BME680Controller.hpp"
#include "I2CBusController.hpp"

#include <chrono>
#include <thread>
#include <iostream>

namespace PBL::I2C
{

// https://github.com/boschsensortec/BME680_driver/blob/master/bme680.c

namespace
{

// Ids
constexpr std::uint8_t kBme680UniqueId{ 0x61 };

// Addresses
constexpr std::uint8_t kBme680SoftResetAddr{ 0xe0 };
constexpr std::uint8_t kBme680ChipIdAddr{ 0xd0 };
constexpr std::uint8_t kBme680CoeffAddr1{ 0x89 };
constexpr std::uint8_t kBme680CoeffAddr2{ 0xe1 };

// Other coefficient's address
constexpr std::uint8_t BME680_ADDR_RES_HEAT_VAL_ADDR{ 0x00 };
constexpr std::uint8_t BME680_ADDR_RES_HEAT_RANGE_ADDR{ 0x02 };
constexpr std::uint8_t BME680_ADDR_RANGE_SW_ERR_ADDR{ 0x04 };
constexpr std::uint8_t BME680_ADDR_SENS_CONF_START{ 0x5A };
constexpr std::uint8_t BME680_ADDR_GAS_CONF_START{ 0x64 };

// Lenghts
constexpr std::uint8_t BME680_FIELD_LENGTH{ 15 };
constexpr std::uint8_t BME680_FIELD_ADDR_OFFSET{ 17 };
constexpr std::uint8_t kBme680CoeffAddr1Len{ 25 };
constexpr std::uint8_t kBme680CoeffAddr2Len{ 16 };

// Commands
constexpr std::uint8_t kBme680SoftResetCmd{ 0xb6 };

// Delays
constexpr std::chrono::milliseconds kBme680ResetPeriodMs{ 10 };
constexpr std::chrono::milliseconds BME680_POLL_PERIOD_MS{ 10 };

// Array Index to Field data mapping for Calibration Data
constexpr std::uint8_t BME680_T2_LSB_REG{ 1 };
constexpr std::uint8_t BME680_T2_MSB_REG{ 2 };
constexpr std::uint8_t BME680_T3_REG{ 3 };
constexpr std::uint8_t BME680_P1_LSB_REG{ 5 };
constexpr std::uint8_t BME680_P1_MSB_REG{ 6 };
constexpr std::uint8_t BME680_P2_LSB_REG{ 7 };
constexpr std::uint8_t BME680_P2_MSB_REG{ 8 };
constexpr std::uint8_t BME680_P3_REG{ 9 };
constexpr std::uint8_t BME680_P4_LSB_REG{ 11 };
constexpr std::uint8_t BME680_P4_MSB_REG{ 12 };
constexpr std::uint8_t BME680_P5_LSB_REG{ 13 };
constexpr std::uint8_t BME680_P5_MSB_REG{ 14 };
constexpr std::uint8_t BME680_P7_REG{ 15 };
constexpr std::uint8_t BME680_P6_REG{ 16 };
constexpr std::uint8_t BME680_P8_LSB_REG{ 19 };
constexpr std::uint8_t BME680_P8_MSB_REG{ 20 };
constexpr std::uint8_t BME680_P9_LSB_REG{ 21 };
constexpr std::uint8_t BME680_P9_MSB_REG{ 22 };
constexpr std::uint8_t BME680_P10_REG{ 23 };
constexpr std::uint8_t BME680_H2_MSB_REG{ 25 };
constexpr std::uint8_t BME680_H2_LSB_REG{ 26 };
constexpr std::uint8_t BME680_H1_LSB_REG{ 26 };
constexpr std::uint8_t BME680_H1_MSB_REG{ 27 };
constexpr std::uint8_t BME680_H3_REG{ 28 };
constexpr std::uint8_t BME680_H4_REG{ 29 };
constexpr std::uint8_t BME680_H5_REG{ 30 };
constexpr std::uint8_t BME680_H6_REG{ 31 };
constexpr std::uint8_t BME680_H7_REG{ 32 };
constexpr std::uint8_t BME680_T1_LSB_REG{ 33 };
constexpr std::uint8_t BME680_T1_MSB_REG{ 34 };
constexpr std::uint8_t BME680_GH2_LSB_REG{ 35 };
constexpr std::uint8_t BME680_GH2_MSB_REG{ 36 };
constexpr std::uint8_t BME680_GH1_REG{ 37 };
constexpr std::uint8_t BME680_GH3_REG{ 38 };

// Mask definitions
constexpr std::uint8_t BME680_GAS_MEAS_MSK{ 0x30 };
constexpr std::uint8_t BME680_NBCONV_MSK{ 0X0F };
constexpr std::uint8_t BME680_FILTER_MSK{ 0X1C };
constexpr std::uint8_t BME680_OST_MSK{ 0XE0 };
constexpr std::uint8_t BME680_OSP_MSK{ 0X1C };
constexpr std::uint8_t BME680_OSH_MSK{ 0X07 };
constexpr std::uint8_t BME680_HCTRL_MSK{ 0x08 };
constexpr std::uint8_t BME680_RUN_GAS_MSK{ 0x10 };
constexpr std::uint8_t BME680_MODE_MSK{ 0x03 };
constexpr std::uint8_t BME680_RHRANGE_MSK{ 0x30 };
constexpr std::uint8_t BME680_RSERROR_MSK{ 0xf0 };
constexpr std::uint8_t BME680_NEW_DATA_MSK{ 0x80 };
constexpr std::uint8_t BME680_GAS_INDEX_MSK{ 0x0f };
constexpr std::uint8_t BME680_GAS_RANGE_MSK{ 0x0f };
constexpr std::uint8_t BME680_GASM_VALID_MSK{ 0x20 };
constexpr std::uint8_t BME680_HEAT_STAB_MSK{ 0x10 };
constexpr std::uint8_t BME680_MEM_PAGE_MSK{ 0x10 };
constexpr std::uint8_t BME680_SPI_RD_MSK{ 0x80 };
constexpr std::uint8_t BME680_SPI_WR_MSK{ 0x7f };
constexpr std::uint8_t BME680_BIT_H1_DATA_MSK{ 0x0F };

// Ambient humidity shift value for compensation
#define BME680_HUM_REG_SHIFT_VAL UINT8_C( 4 )

// Field settings
#define BME680_FIELD0_ADDR UINT8_C( 0x1d )

// Heater settings
#define BME680_RES_HEAT0_ADDR UINT8_C( 0x5a )
#define BME680_GAS_WAIT0_ADDR UINT8_C( 0x64 )

// Info's
#define BME680_I_MIN_CORRECTION UINT8_C( 1 )
#define BME680_I_MAX_CORRECTION UINT8_C( 2 )

// Over-sampling settings
#define BME680_OS_NONE UINT8_C( 0 )
#define BME680_OS_1X UINT8_C( 1 )
#define BME680_OS_2X UINT8_C( 2 )
#define BME680_OS_4X UINT8_C( 3 )
#define BME680_OS_8X UINT8_C( 4 )
#define BME680_OS_16X UINT8_C( 5 )

// IIR filter settings
#define BME680_FILTER_SIZE_0 UINT8_C( 0 )
#define BME680_FILTER_SIZE_1 UINT8_C( 1 )
#define BME680_FILTER_SIZE_3 UINT8_C( 2 )
#define BME680_FILTER_SIZE_7 UINT8_C( 3 )
#define BME680_FILTER_SIZE_15 UINT8_C( 4 )
#define BME680_FILTER_SIZE_31 UINT8_C( 5 )
#define BME680_FILTER_SIZE_63 UINT8_C( 6 )
#define BME680_FILTER_SIZE_127 UINT8_C( 7 )

// Settings selector
#define BME680_OST_SEL UINT16_C( 1 )
#define BME680_OSP_SEL UINT16_C( 2 )
#define BME680_OSH_SEL UINT16_C( 4 )
#define BME680_GAS_MEAS_SEL UINT16_C( 8 )
#define BME680_FILTER_SEL UINT16_C( 16 )
#define BME680_HCNTRL_SEL UINT16_C( 32 )
#define BME680_RUN_GAS_SEL UINT16_C( 64 )
#define BME680_NBCONV_SEL UINT16_C( 128 )
#define BME680_GAS_SENSOR_SEL ( BME680_GAS_MEAS_SEL | BME680_RUN_GAS_SEL | BME680_NBCONV_SEL )

// Bit position definitions for sensor settings
#define BME680_GAS_MEAS_POS UINT8_C( 4 )
#define BME680_FILTER_POS UINT8_C( 2 )
#define BME680_OST_POS UINT8_C( 5 )
#define BME680_OSP_POS UINT8_C( 2 )
#define BME680_RUN_GAS_POS UINT8_C( 4 )

// Power mode settings
#define BME680_SLEEP_MODE UINT8_C( 0 )
#define BME680_FORCED_MODE UINT8_C( 1 )

#define BME680_MAX_OVERFLOW_VAL INT32_C( 0x40000000 )

// Sensor configuration registers
#define BME680_CONF_HEAT_CTRL_ADDR UINT8_C( 0x70 )
#define BME680_CONF_ODR_RUN_GAS_NBC_ADDR UINT8_C( 0x71 )
#define BME680_CONF_OS_H_ADDR UINT8_C( 0x72 )
#define BME680_MEM_PAGE_ADDR UINT8_C( 0xf3 )
#define BME680_CONF_T_P_MODE_ADDR UINT8_C( 0x74 )
#define BME680_CONF_ODR_FILT_ADDR UINT8_C( 0x75 )

// Macro to combine two 8 bit data's to form a 16 bit data
#define BME680_CONCAT_BYTES( msb, lsb ) ( ( (uint16_t)msb << 8 ) | (uint16_t)lsb )

// Macro to SET and GET BITS of a register
#define BME680_SET_BITS( reg_data, bitname, data )                                                                     \
	( ( reg_data & ~( bitname##_MSK ) ) | ( ( data << bitname##_POS ) & bitname##_MSK ) )

#define BME680_GET_BITS( reg_data, bitname ) ( ( reg_data & ( bitname##_MSK ) ) >> ( bitname##_POS ) )

// Macro variant to handle the bitname position if it is zero
#define BME680_SET_BITS_POS_0( reg_data, bitname, data )                                                               \
	( ( reg_data & ~( bitname##_MSK ) ) | ( data & bitname##_MSK ) )

#define BME680_GET_BITS_POS_0( reg_data, bitname ) ( reg_data & ( bitname##_MSK ) )

} // namespace

/*!
 * @brief Interface selection Enumerations
 */
enum class Bme680Intf
{
	BME680_SPI_INTF, /*! SPI interface */
	BME680_I2C_INTF /*! I2C interface */
};

/*!
 * @brief BME680 sensor settings structure which comprises of ODR,
 * over-sampling and filter settings.
 */
struct Bme680TphSett
{
	uint8_t os_hum; // Humidity oversampling
	uint8_t os_temp; // Temperature oversampling
	uint8_t os_pres; // Pressure oversampling
	uint8_t filter; // Filter coefficient
};

/*!
 * @brief BME680 gas sensor which comprises of gas settings
 *  and status parameters
 */
struct Bme680GasSett
{
	uint8_t nb_conv; /*! Variable to store nb conversion */
	uint8_t heatr_ctrl; /*! Variable to store heater control */
	uint8_t run_gas; /*! Run gas enable value */
	uint16_t heatr_temp; /*! Heater temperature value */
	uint16_t heatr_dur; /*! Duration profile value */
};

struct BME680Controller::BME680CalibData
{
	std::uint16_t par_h1; // Variable to store calibrated humidity data
	std::uint16_t par_h2; // Variable to store calibrated humidity data
	std::int8_t par_h3; // Variable to store calibrated humidity data
	std::int8_t par_h4; // Variable to store calibrated humidity data
	std::int8_t par_h5; // Variable to store calibrated humidity data
	std::uint8_t par_h6; // Variable to store calibrated humidity data
	std::int8_t par_h7; // Variable to store calibrated humidity data
	std::int8_t par_gh1; // Variable to store calibrated gas data
	std::int16_t par_gh2; // Variable to store calibrated gas data
	std::int8_t par_gh3; // Variable to store calibrated gas data
	std::uint16_t par_t1; // Variable to store calibrated temperature data
	std::int16_t par_t2; // Variable to store calibrated temperature data
	std::int8_t par_t3; // Variable to store calibrated temperature data
	std::uint16_t par_p1; // Variable to store calibrated pressure data
	std::int16_t par_p2; // Variable to store calibrated pressure data
	std::int8_t par_p3; // Variable to store calibrated pressure data
	std::int16_t par_p4; // Variable to store calibrated pressure data
	std::int16_t par_p5; // Variable to store calibrated pressure data
	std::int8_t par_p6; // Variable to store calibrated pressure data
	std::int8_t par_p7; // Variable to store calibrated pressure data
	std::int16_t par_p8; // Variable to store calibrated pressure data
	std::int16_t par_p9; // Variable to store calibrated pressure data
	std::uint8_t par_p10; // Variable to store calibrated pressure data

#ifndef BME680_FLOAT_POINT_COMPENSATION
	std::int32_t t_fine; // Variable to store t_fine size
#else
	float t_fine; // Variable to store t_fine size
#endif

	std::uint8_t res_heat_range; // Variable to store heater resistance range
	std::int8_t res_heat_val; // Variable to store heater resistance value
	std::int8_t range_sw_err; // Variable to store error range

	void dump()
	{
		std::cerr << "par_h1: " << std::dec << int{ par_h1 } << std::endl;
		std::cerr << "par_h2: " << std::dec << int{ par_h2 } << std::endl;
		std::cerr << "par_h3: " << std::dec << int{ par_h3 } << std::endl;
		std::cerr << "par_h4: " << std::dec << int{ par_h4 } << std::endl;
		std::cerr << "par_h5: " << std::dec << int{ par_h5 } << std::endl;
		std::cerr << "par_h6: " << std::dec << int{ par_h6 } << std::endl;
		std::cerr << "par_h7: " << std::dec << int{ par_h7 } << std::endl;
		std::cerr << "par_gh1: " << std::dec << int{ par_gh1 } << std::endl;
		std::cerr << "par_gh2: " << std::dec << int{ par_gh2 } << std::endl;
		std::cerr << "par_gh3: " << std::dec << int{ par_gh3 } << std::endl;
		std::cerr << "par_t1: " << std::dec << int{ par_t1 } << std::endl;
		std::cerr << "par_t2: " << std::dec << int{ par_t2 } << std::endl;
		std::cerr << "par_t3: " << std::dec << int{ par_t3 } << std::endl;
		std::cerr << "par_p1: " << std::dec << int{ par_p1 } << std::endl;
		std::cerr << "par_p2: " << std::dec << int{ par_p2 } << std::endl;
		std::cerr << "par_p3: " << std::dec << int{ par_p3 } << std::endl;
		std::cerr << "par_p4: " << std::dec << int{ par_p4 } << std::endl;
		std::cerr << "par_p5: " << std::dec << int{ par_p5 } << std::endl;
		std::cerr << "par_p6: " << std::dec << int{ par_p6 } << std::endl;
		std::cerr << "par_p7: " << std::dec << int{ par_p7 } << std::endl;
		std::cerr << "par_p8: " << std::dec << int{ par_p8 } << std::endl;
		std::cerr << "par_p9: " << std::dec << int{ par_p9 } << std::endl;
		std::cerr << "par_p10: " << std::dec << int{ par_p10 } << std::endl;
	}
};

static int16_t calc_temperature( uint32_t temp_adc, BME680Controller::BME680CalibData& calibData )
{
	int64_t var1;
	int64_t var2;
	int64_t var3;
	int16_t calc_temp;

	var1 = ( (int32_t)temp_adc >> 3 ) - ( (int32_t)calibData.par_t1 << 1 );
	var2 = ( var1 * (int32_t)calibData.par_t2 ) >> 11;
	var3 = ( ( var1 >> 1 ) * ( var1 >> 1 ) ) >> 12;
	var3 = ( ( var3 ) * ( (int32_t)calibData.par_t3 << 4 ) ) >> 14;
	calibData.t_fine = (int32_t)( var2 + var3 );
	calc_temp = (int16_t)( ( ( calibData.t_fine * 5 ) + 128 ) >> 8 );

	return calc_temp;
}

static uint32_t calc_pressure( uint32_t pres_adc, const BME680Controller::BME680CalibData& calibData )
{
	int32_t var1;
	int32_t var2;
	int32_t var3;
	int32_t pressure_comp;

	var1 = ( ( (int32_t)calibData.t_fine ) >> 1 ) - 64000;
	var2 = ( ( ( ( var1 >> 2 ) * ( var1 >> 2 ) ) >> 11 ) * (int32_t)calibData.par_p6 ) >> 2;
	var2 = var2 + ( ( var1 * (int32_t)calibData.par_p5 ) << 1 );
	var2 = ( var2 >> 2 ) + ( (int32_t)calibData.par_p4 << 16 );
	var1 = ( ( ( ( ( var1 >> 2 ) * ( var1 >> 2 ) ) >> 13 ) * ( (int32_t)calibData.par_p3 << 5 ) ) >> 3 ) +
		   ( ( (int32_t)calibData.par_p2 * var1 ) >> 1 );
	var1 = var1 >> 18;
	var1 = ( ( 32768 + var1 ) * (int32_t)calibData.par_p1 ) >> 15;
	pressure_comp = 1048576 - pres_adc;
	pressure_comp = (int32_t)( ( pressure_comp - ( var2 >> 12 ) ) * ( (uint32_t)3125 ) );
	if( pressure_comp >= BME680_MAX_OVERFLOW_VAL )
		pressure_comp = ( ( pressure_comp / var1 ) << 1 );
	else
		pressure_comp = ( ( pressure_comp << 1 ) / var1 );
	var1 = ( (int32_t)calibData.par_p9 * (int32_t)( ( ( pressure_comp >> 3 ) * ( pressure_comp >> 3 ) ) >> 13 ) ) >> 12;
	var2 = ( (int32_t)( pressure_comp >> 2 ) * (int32_t)calibData.par_p8 ) >> 13;
	var3 = ( (int32_t)( pressure_comp >> 8 ) * (int32_t)( pressure_comp >> 8 ) * (int32_t)( pressure_comp >> 8 ) *
			 (int32_t)calibData.par_p10 ) >>
		   17;

	pressure_comp = (int32_t)( pressure_comp ) + ( ( var1 + var2 + var3 + ( (int32_t)calibData.par_p7 << 7 ) ) >> 4 );

	return (uint32_t)pressure_comp;
}

static uint32_t calc_humidity( uint16_t hum_adc, const BME680Controller::BME680CalibData& calibData )
{
	int32_t var1;
	int32_t var2;
	int32_t var3;
	int32_t var4;
	int32_t var5;
	int32_t var6;
	int32_t temp_scaled;
	int32_t calc_hum;

	temp_scaled = ( ( (int32_t)calibData.t_fine * 5 ) + 128 ) >> 8;
	var1 = (int32_t)( hum_adc - ( (int32_t)( (int32_t)calibData.par_h1 * 16 ) ) ) -
		   ( ( ( temp_scaled * (int32_t)calibData.par_h3 ) / ( (int32_t)100 ) ) >> 1 );
	var2 = ( (int32_t)calibData.par_h2 *
			 ( ( ( temp_scaled * (int32_t)calibData.par_h4 ) / ( (int32_t)100 ) ) +
			   ( ( ( temp_scaled * ( ( temp_scaled * (int32_t)calibData.par_h5 ) / ( (int32_t)100 ) ) ) >> 6 ) /
				 ( (int32_t)100 ) ) +
			   (int32_t)( 1 << 14 ) ) ) >>
		   10;
	var3 = var1 * var2;
	var4 = (int32_t)calibData.par_h6 << 7;
	var4 = ( ( var4 ) + ( ( temp_scaled * (int32_t)calibData.par_h7 ) / ( (int32_t)100 ) ) ) >> 4;
	var5 = ( ( var3 >> 14 ) * ( var3 >> 14 ) ) >> 10;
	var6 = ( var4 * var5 ) >> 1;
	calc_hum = ( ( ( var3 + var6 ) >> 10 ) * ( (int32_t)1000 ) ) >> 12;

	if( calc_hum > 100000 ) /* Cap at 100%rH */
		calc_hum = 100000;
	else if( calc_hum < 0 )
		calc_hum = 0;

	return (uint32_t)calc_hum;
}

static uint32_t
calc_gas_resistance( uint16_t gas_res_adc, uint8_t gas_range, const BME680Controller::BME680CalibData& calibData )
{
	int64_t var1;
	uint64_t var2;
	int64_t var3;
	uint32_t calc_gas_res;
	/**Look up table 1 for the possible gas range values */
	uint32_t lookupTable1[ 16 ] = { UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2126008810 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2130303777 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2143188679 ),
									UINT32_C( 2136746228 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2126008810 ),
									UINT32_C( 2147483647 ),
									UINT32_C( 2147483647 ) };

	/**Look up table 2 for the possible gas range values */
	uint32_t lookupTable2[ 16 ] = { UINT32_C( 4096000000 ),
									UINT32_C( 2048000000 ),
									UINT32_C( 1024000000 ),
									UINT32_C( 512000000 ),
									UINT32_C( 255744255 ),
									UINT32_C( 127110228 ),
									UINT32_C( 64000000 ),
									UINT32_C( 32258064 ),
									UINT32_C( 16016016 ),
									UINT32_C( 8000000 ),
									UINT32_C( 4000000 ),
									UINT32_C( 2000000 ),
									UINT32_C( 1000000 ),
									UINT32_C( 500000 ),
									UINT32_C( 250000 ),
									UINT32_C( 125000 ) };

	var1 =
		(int64_t)( ( 1340 + ( 5 * (int64_t)calibData.range_sw_err ) ) * ( (int64_t)lookupTable1[ gas_range ] ) ) >> 16;
	var2 = ( ( (int64_t)( (int64_t)gas_res_adc << 15 ) - (int64_t)( 16777216 ) ) + var1 );
	var3 = ( ( (int64_t)lookupTable2[ gas_range ] * (int64_t)var1 ) >> 9 );
	calc_gas_res = (uint32_t)( ( var3 + ( (int64_t)var2 >> 1 ) ) / (int64_t)var2 );

	return calc_gas_res;
}

typedef int8_t ( *bme680_com_fptr_t )( uint8_t dev_id, uint8_t reg_addr, uint8_t* data, uint16_t len );
typedef void ( *bme680_delay_fptr_t )( uint32_t period );

struct BME680Controller::BME680Device
{
	uint8_t chip_id; // Chip Id
	uint8_t dev_id; // Device Id
	Bme680Intf intf; // SPI/I2C interface
	uint8_t mem_page; // Memory page used
	int8_t amb_temp; // Ambient temperature in Degree C
	BME680CalibData calib; // Sensor calibration data
	Bme680TphSett tph_sett; // Sensor settings
	Bme680GasSett gas_sett; // Gas Sensor settings
	uint8_t power_mode; // Sensor power modes
	uint8_t new_fields; // New sensor fields
	uint8_t info_msg; // Store the info messages
	bme680_com_fptr_t read; // Bus read function pointer
	bme680_com_fptr_t write; // Bus write function pointer
	bme680_delay_fptr_t delay_ms; // delay function pointer
	int8_t com_rslt; // Communication function result
};

uint8_t calc_heater_res( uint16_t temp, std::uint8_t ambientTemp, const BME680Controller::BME680CalibData& calibData )
{
	uint8_t heatr_res;
	int32_t var1;
	int32_t var2;
	int32_t var3;
	int32_t var4;
	int32_t var5;
	int32_t heatr_res_x100;

	if( temp > 400 ) /* Cap temperature */
		temp = 400;

	var1 = ( ( (int32_t)ambientTemp * calibData.par_gh3 ) / 1000 ) * 256;
	var2 = ( calibData.par_gh1 + 784 ) * ( ( ( ( ( calibData.par_gh2 + 154009 ) * temp * 5 ) / 100 ) + 3276800 ) / 10 );
	var3 = var1 + ( var2 / 2 );
	var4 = ( var3 / ( calibData.res_heat_range + 4 ) );
	var5 = ( 131 * calibData.res_heat_val ) + 65536;
	heatr_res_x100 = (int32_t)( ( ( var4 / var5 ) - 250 ) * 34 );
	heatr_res = (uint8_t)( ( heatr_res_x100 + 50 ) / 100 );

	return heatr_res;
}

static uint8_t calc_heater_dur( uint16_t dur )
{
	uint8_t durval{};

	if( dur >= 0xfc0 )
	{
		durval = 0xff; // Max duration
	}
	else
	{
		uint8_t factor{};
		while( dur > 0x3F )
		{
			dur = dur / 4;
			factor += 1;
		}
		durval = (uint8_t)( dur + ( factor * 64 ) );
	}

	return durval;
}

BME680Controller::BME680Controller( I2CBusController& busController, std::uint8_t address )
	: m_busController{ busController }
	, m_bme680Address{ address }
	, m_calibData{ std::make_unique< BME680CalibData >() }
{
	if( !softReset() )
	{
		setLastError( "Failed soft reset" );
		return;
	}

	std::uint8_t chipId{};
	if( !m_busController.read( m_bme680Address, kBme680ChipIdAddr, chipId ) )
	{
		setLastError( "Unable to read from BME680 chip" );
		return;
	}

	if( chipId != kBme680UniqueId )
	{
		setLastError( "Bad chip id retrieved" );
		return;
	}

	if( !retrieveCalibData() )
	{
		setLastError( "Failed to retrieve calibration data" );
		return;
	}

	m_calibData->dump();

	// Note it may be possible that we need to be in a sleep mode
	// to be able to set any settings
	if( !setSensorMode( BME680_SLEEP_MODE ) )
	{
		setLastError( "Unable to set sensor mode" );
		return;
	}

	// // Set oversampling settings
	std::uint16_t required_settings = ( BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_GAS_MEAS_SEL );

	if( !setSensorSettings( required_settings ) )
	{
		setLastError( "Unable to set sensor settings" );
		return;
	}

	// 320 Deg C for 150ms
	if( !setGasConfig( 320, 24, 150 ) )
	{
		setLastError( "Failed to set gas config" );
		return;
	}

	if( !setSensorMode( BME680_FORCED_MODE ) )
	{
		setLastError( "Unable to set sensor mode" );
		return;
	}

	// 1. Configure the oversampling settigns for temp, press & humidity
	// by settings the control resiters osrs_t<2:0> and osrs_h<2:0>, respectively
	// Supported settings range from 16x oversampling down to 0x.
	// 1.1. Set humidity oversampling to 1x by writing 0b001 to osrs_h<2:0>
	// 1.2. Set temp oversamling to 2x by writing 0b010 to osrs_t<2:0>
	// 1.3. Set pressure oversampling to 16x by writing 0b101 to osrs_p<2:0>

	// It is highly recommended to set first osrs_h<2:0> followed by osrs_t<2:0> and osrs_p<2:0> in
	// one write command (see section 3.3)

	// 2. The user shall set at least one gas sensor hot plate temperature set point and heating duration.
	// Up to 10 heating duration can be doncifugred through the control registares gas_wait_x<7:0>, where
	// x ranges from 0 to 9. See section 5.3.3 for definition of register content.
	// The corresponding heater set-points are stored in the registers res_heat_x<7:0>.
	// Section 3.3.5 explains hw to convert the target heater temperature, e.g. 300 deg C into a register code.
	// For forced mode operation, the used heater set point is selected by setting the control register
	// nb_conv<3:0> to the heater profile to be used, e.g. to use
	// gas_wait_0<7:0> and res_heat_0<7:0>, nb_conv<3:0> shall be set to 0x0.
	// Finally has functionallity shall be enabled by setting the run_has_l bit to 1.

	// 4. set gas_wait_0<7:0> to 0x59 to select 100ms heat up duration
	// 5. Set the corresponding heater set-point by writing the target heater resistance to res_heat_0<7:0>
	// 6. Set nb_conv<3:0> to 0x00 to select the previously defined heater settings
	// 7. Set run_gas_l to 1 to enable gas measurements

	// Now a single forced mode measurement with the above settings can be triggered by writing 0b01 to mode <1:0>.
	// For more details on the data readout, please see Section 5.3.1.3

	// 8. Set mode<1:0> to 0b01 to trigger a single measurement

	// Other pathway
	// Configure the BME680 driver gas_sensor.dev_id = BME680_I2C_ADDR_SECONDARY;
	// gas_sensor.intf = BME680_I2C_INTF;
	// gas_sensor.read = bme680I2cRead;
	// gas_sensor.write = bme680I2cWrite;
	// gas_sensor.delay_ms = HAL_Delay;
	// gas_sensor.amb_temp = 25;

	// // Initialize the driver
	// if( bme680_init( &gas_sensor ) != BME680_OK )
	// {
	// 	char bme_msg[] = "BME680 Initialization Error\r\n";
	// 	noteDebugSerialOutput( bme_msg );
	// }
	// else
	// {
	// 	char bme_msg[] = "BME680 Initialized and Ready\r\n";
	// 	noteDebugSerialOutput( bme_msg );
	// }

	// // Select desired oversampling rates
	// gas_sensor.tph_sett.os_hum = BME680_OS_2X;
	// gas_sensor.tph_sett.os_pres = BME680_OS_4X;
	// gas_sensor.tph_sett.os_temp = BME680_OS_8X;

	// // Set sensor to "always on"
	// gas_sensor.power_mode = BME680_FORCED_MODE;

	// // Set oversampling settings
	// uint8_t required_settings = ( BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL );
	// rslt = bme680_set_sensor_settings( required_settings, &gas_sensor );

	// // Set sensor mode
	// rslt = bme680_set_sensor_mode( &gas_sensor );

	// // Query minimum sampling period
	// uint16_t min_sampling_period;
	// bme680_get_profile_dur( &min_sampling_period, &gas_sensor );

	// // Sampling results variable
	// struct bme680_field_data data;

	// You will also need to format the results in human readable format:
	// Format results into a human readable string
	// sprintf(i2c_reading_buf,
	//   "T: %u.%u degC, H %u.%u %%rH\r\n",
	//   (unsigned int)data.temperature / 100,
	//   (unsigned int)data.temperature % 100,
	//   (unsigned int)data.humidity / 1000,
	//   (unsigned int)data.humidity % 1000);
}

BME680Controller::~BME680Controller() = default;

float BME680Controller::temperature()
{
	return {};
}

float BME680Controller::pressure()
{
	return {};
}

float BME680Controller::humidity()
{
	return {};
}

float BME680Controller::voc()
{
	return {};
}

bool BME680Controller::softReset()
{
	auto rslt = m_busController.write( m_bme680Address, kBme680SoftResetAddr, kBme680SoftResetCmd );
	std::this_thread::sleep_for( kBme680ResetPeriodMs );
	return rslt;
}

bool BME680Controller::retrieveCalibData()
{
	bool rslt{ true };

	std::uint8_t data[ 41 ];
	std::uint8_t tempVar{};
	rslt = getRegs( kBme680CoeffAddr1, data, kBme680CoeffAddr1Len );

	if( !rslt )
	{
		return false;
	}

	rslt = getRegs( kBme680CoeffAddr2, data, kBme680CoeffAddr2Len );

	m_calibData->par_t1 =
		( std::uint16_t{ data[ BME680_T1_MSB_REG ] } << 8 ) | std::uint16_t{ data[ BME680_T1_LSB_REG ] };
	m_calibData->par_t2 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_T2_MSB_REG ], data[ BME680_T2_LSB_REG ] ) );
	m_calibData->par_t3 = static_cast< int8_t >( data[ BME680_T3_REG ] );

	// Pressure related coefficients
	m_calibData->par_p1 = (uint16_t)( BME680_CONCAT_BYTES( data[ BME680_P1_MSB_REG ], data[ BME680_P1_LSB_REG ] ) );
	m_calibData->par_p2 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_P2_MSB_REG ], data[ BME680_P2_LSB_REG ] ) );
	m_calibData->par_p3 = static_cast< int8_t >( data[ BME680_P3_REG ] );
	m_calibData->par_p4 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_P4_MSB_REG ], data[ BME680_P4_LSB_REG ] ) );
	m_calibData->par_p5 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_P5_MSB_REG ], data[ BME680_P5_LSB_REG ] ) );
	m_calibData->par_p6 = static_cast< int8_t >( data[ BME680_P6_REG ] );
	m_calibData->par_p7 = static_cast< int8_t >( data[ BME680_P7_REG ] );
	m_calibData->par_p8 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_P8_MSB_REG ], data[ BME680_P8_LSB_REG ] ) );
	m_calibData->par_p9 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_P9_MSB_REG ], data[ BME680_P9_LSB_REG ] ) );
	m_calibData->par_p10 = data[ BME680_P10_REG ];

	// Humidity related coefficients
	m_calibData->par_h1 = (uint16_t)( ( (uint16_t)data[ BME680_H1_MSB_REG ] << BME680_HUM_REG_SHIFT_VAL ) |
									  ( data[ BME680_H1_LSB_REG ] & BME680_BIT_H1_DATA_MSK ) );
	m_calibData->par_h2 = (uint16_t)( ( (uint16_t)data[ BME680_H2_MSB_REG ] << BME680_HUM_REG_SHIFT_VAL ) |
									  ( ( data[ BME680_H2_LSB_REG ] ) >> BME680_HUM_REG_SHIFT_VAL ) );
	m_calibData->par_h3 = static_cast< int8_t >( data[ BME680_H3_REG ] );
	m_calibData->par_h4 = static_cast< int8_t >( data[ BME680_H4_REG ] );
	m_calibData->par_h5 = static_cast< int8_t >( data[ BME680_H5_REG ] );
	m_calibData->par_h6 = data[ BME680_H6_REG ];
	m_calibData->par_h7 = static_cast< int8_t >( data[ BME680_H7_REG ] );

	// Gas heater related coefficients
	m_calibData->par_gh1 = static_cast< int8_t >( data[ BME680_GH1_REG ] );
	m_calibData->par_gh2 = (int16_t)( BME680_CONCAT_BYTES( data[ BME680_GH2_MSB_REG ], data[ BME680_GH2_LSB_REG ] ) );
	m_calibData->par_gh3 = static_cast< int8_t >( data[ BME680_GH3_REG ] );

	// Other coefficients
	rslt = rslt && m_busController.read( m_bme680Address, BME680_ADDR_RES_HEAT_RANGE_ADDR, tempVar );
	m_calibData->res_heat_range = ( ( tempVar & BME680_RHRANGE_MSK ) / 16 );

	rslt = rslt && m_busController.read( m_bme680Address, BME680_ADDR_RES_HEAT_VAL_ADDR, tempVar );
	m_calibData->res_heat_val = static_cast< int8_t >( tempVar );

	rslt = rslt && m_busController.read( m_bme680Address, BME680_ADDR_RANGE_SW_ERR_ADDR, tempVar );

	m_calibData->range_sw_err = ( (int8_t)tempVar & (int8_t)BME680_RSERROR_MSK ) / 16;

	return rslt;
}

bool BME680Controller::getRegs( std::uint8_t regAddr, std::uint8_t* regData, std::uint16_t dataLen )
{
	return m_busController.read( m_bme680Address, regAddr, regData, dataLen );
}

#define BME680_REG_BUFFER_LENGTH UINT8_C( 6 )

bool BME680Controller::setSensorSettings( std::uint16_t desiredSettings )
{
	int8_t rslt{};
	uint8_t reg_addr{};
	uint8_t data{};
	uint8_t count{};
	uint8_t reg_array[ BME680_REG_BUFFER_LENGTH ] = { 0 };
	uint8_t data_array[ BME680_REG_BUFFER_LENGTH ] = { 0 };

	/* Selecting heater T,P oversampling for the sensor */
	if( desiredSettings & ( BME680_OST_SEL | BME680_OSP_SEL ) )
	{
		reg_addr = BME680_CONF_T_P_MODE_ADDR;
		if( !m_busController.read( m_bme680Address, reg_addr, data ) )
		{
			return false;
		}

		if( desiredSettings & BME680_OST_SEL )
		{
			data = BME680_SET_BITS( data, BME680_OST, BME680_OS_8X );
		}

		if( desiredSettings & BME680_OSP_SEL )
		{
			data = BME680_SET_BITS( data, BME680_OSP, BME680_OS_4X );
		}

		reg_array[ count ] = reg_addr;
		data_array[ count ] = data;
		count++;
	}

	// Selecting humidity oversampling for the sensor
	if( desiredSettings & BME680_OSH_SEL )
	{
		reg_addr = BME680_CONF_OS_H_ADDR;
		if( !m_busController.read( m_bme680Address, reg_addr, data ) )
		{
			return false;
		}

		data = BME680_SET_BITS_POS_0( data, BME680_OSH, BME680_OS_2X );

		reg_array[ count ] = reg_addr; /* Append configuration */
		data_array[ count ] = data;
		count++;
	}

	// Selecting the runGas and NB conversion settings for the sensor
	if( desiredSettings & ( BME680_RUN_GAS_SEL | BME680_NBCONV_SEL ) )
	{
		// rslt = boundary_check( &dev->gas_sett.run_gas, BME680_RUN_GAS_DISABLE, BME680_RUN_GAS_ENABLE, dev );
		// if( rslt == BME680_OK )
		// {
		// 	/* Validate boundary conditions */
		// 	rslt = boundary_check( &dev->gas_sett.nb_conv, BME680_NBCONV_MIN, BME680_NBCONV_MAX, dev );
		// }

		// Run gas enable and disable settings
#define BME680_RUN_GAS_DISABLE UINT8_C( 0 )
#define BME680_RUN_GAS_ENABLE UINT8_C( 1 )

// Number of conversion settings
#define BME680_NBCONV_MIN UINT8_C( 0 )
#define BME680_NBCONV_MAX UINT8_C( 10 )

		reg_addr = BME680_CONF_ODR_RUN_GAS_NBC_ADDR;
		if( !m_busController.read( m_bme680Address, reg_addr, data ) )
		{
			return false;
		}

		if( desiredSettings & BME680_RUN_GAS_SEL )
			data = BME680_SET_BITS( data, BME680_RUN_GAS, BME680_RUN_GAS_ENABLE );

		if( desiredSettings & BME680_NBCONV_SEL )
			data = BME680_SET_BITS_POS_0( data, BME680_NBCONV, BME680_NBCONV_MIN );

		reg_array[ count ] = reg_addr; // Append configuration
		data_array[ count ] = data;
		count++;
	}

	setRegs( reg_array, data_array, count );

	return true;
}

bool BME680Controller::setSensorMode( std::uint8_t mode )
{
	bool rslt;
	uint8_t tmp_pow_mode;
	uint8_t pow_mode = 0;
	uint8_t reg_addr = BME680_CONF_T_P_MODE_ADDR;

	// Call repeatedly until in sleep
	do
	{
		rslt = m_busController.read( m_bme680Address, BME680_CONF_T_P_MODE_ADDR, tmp_pow_mode );
		if( !rslt )
		{
			// Put to sleep before changing mode
			pow_mode = ( tmp_pow_mode & BME680_MODE_MSK );

			if( pow_mode != BME680_SLEEP_MODE )
			{
				tmp_pow_mode = tmp_pow_mode & ( ~BME680_MODE_MSK ); // Set to sleep
				rslt = m_busController.write( m_bme680Address, reg_addr, tmp_pow_mode );
				std::this_thread::sleep_for( BME680_POLL_PERIOD_MS );
			}
		}
	} while( pow_mode != BME680_SLEEP_MODE );

	// Already in sleep
	if( mode != BME680_SLEEP_MODE )
	{
		tmp_pow_mode = ( tmp_pow_mode & ~BME680_MODE_MSK ) | ( mode & BME680_MODE_MSK );
		rslt = m_busController.write( m_bme680Address, reg_addr, tmp_pow_mode );
	}

	return rslt;
}

bool BME680Controller::setGasConfig( std::uint16_t heatrTemp, std::uint16_t ambientTemp, std::uint16_t duration )
{
	// bool rslt{true};
	// uint8_t reg_addr[ 2 ] = {0};
	// uint8_t reg_data[ 2 ] = {0};

	// if( dev->power_mode == BME680_FORCED_MODE )
	// {
	// 	reg_addr[ 0 ] = BME680_RES_HEAT0_ADDR;
	// 	reg_data[ 0 ] = calc_heater_res( dev->gas_sett.heatr_temp, dev );
	// 	reg_addr[ 1 ] = BME680_GAS_WAIT0_ADDR;
	// 	reg_data[ 1 ] = calc_heater_dur( dev->gas_sett.heatr_dur );
	// 	dev->gas_sett.nb_conv = 0;
	// }
	// else
	// {
	// 	rslt = BME680_W_DEFINE_PWR_MODE;
	// }
	// if( rslt == BME680_OK )
	// 	rslt = bme680_set_regs( reg_addr, reg_data, 2, dev );

	// return rslt;

	uint8_t reg_addr[ 2 ] = { 0 };
	uint8_t reg_data[ 2 ] = { 0 };

	reg_addr[ 0 ] = BME680_RES_HEAT0_ADDR;
	reg_data[ 0 ] = calc_heater_res( heatrTemp, ambientTemp, *m_calibData );
	reg_addr[ 1 ] = BME680_GAS_WAIT0_ADDR;
	reg_data[ 1 ] = calc_heater_dur( duration );
	// dev->gas_sett.nb_conv = 0;

	// return m_busController.write( m_bme680Address, reg_addr, tmp_pow_mode );
	return setRegs( reg_addr, reg_data, 2 );
}

bool BME680Controller::readFieldData()
{
	bool rslt{ true };
	uint8_t buff[ BME680_FIELD_LENGTH ] = { 0 };
	uint8_t gas_range{};
	uint32_t adc_temp{};
	uint32_t adc_pres{};
	uint16_t adc_hum{};
	uint16_t adc_gas_res{};
	int tries{ 10 };

	do
	{
		if( !m_busController.read( m_bme680Address, BME680_FIELD0_ADDR, buff, BME680_FIELD_LENGTH ) )
		{
			return false;
		}

		std::uint8_t status = buff[ 0 ] & BME680_NEW_DATA_MSK;
		std::uint8_t gas_index = buff[ 0 ] & BME680_GAS_INDEX_MSK;
		std::uint8_t meas_index = buff[ 1 ];

		adc_pres =
			(uint32_t)( ( (uint32_t)buff[ 2 ] * 4096 ) | ( (uint32_t)buff[ 3 ] * 16 ) | ( (uint32_t)buff[ 4 ] / 16 ) );
		adc_temp =
			(uint32_t)( ( (uint32_t)buff[ 5 ] * 4096 ) | ( (uint32_t)buff[ 6 ] * 16 ) | ( (uint32_t)buff[ 7 ] / 16 ) );
		adc_hum = (uint16_t)( ( (uint32_t)buff[ 8 ] * 256 ) | (uint32_t)buff[ 9 ] );
		adc_gas_res = (uint16_t)( (uint32_t)buff[ 13 ] * 4 | ( ( (uint32_t)buff[ 14 ] ) / 64 ) );
		gas_range = buff[ 14 ] & BME680_GAS_RANGE_MSK;

		status |= buff[ 14 ] & BME680_GASM_VALID_MSK;
		status |= buff[ 14 ] & BME680_HEAT_STAB_MSK;

		if( tries == 10 )
		{
			std::cerr << adc_pres << std::endl;
			std::cerr << adc_temp << std::endl;
			std::cerr << adc_hum << std::endl;

			std::cerr << "raw temp [*C]: " << calc_temperature( adc_temp, *m_calibData ) << std::endl;
			std::cerr << "raw pressure [hPa]: " << calc_pressure( adc_temp, *m_calibData ) / 100.0 << std::endl;
			std::cerr << "raw humidity [%]: " << calc_humidity( adc_hum, *m_calibData ) << std::endl;
			std::cerr << "raw gas resistance [KOhms]: "
					  << calc_gas_resistance( adc_gas_res, gas_range, *m_calibData ) / 1000.0 << std::endl;

			// Normal values should be as follows:
			// 26*C
			// 1005hPa
			// 50%
			// 66-70kOhms
		}

		if( status & BME680_NEW_DATA_MSK )
		{
			std::cerr << "new data is present" << std::endl;
			// data->temperature = calc_temperature( adc_temp, dev );
			// data->pressure = calc_pressure( adc_pres, dev );
			// data->humidity = calc_humidity( adc_hum, dev );
			// data->gas_resistance = calc_gas_resistance( adc_gas_res, gas_range, dev );

			break;
		}

		std::this_thread::sleep_for( BME680_POLL_PERIOD_MS );
		--tries;
	} while( tries );

	return true;
}

std::int8_t BME680Controller::boundaryCheck( std::uint8_t& value,
											 const std::uint8_t min,
											 const std::uint8_t max,
											 std::uint8_t& infoMsg )
{
	int8_t rslt{};

	// Check if value is below minimum value
	if( value < min )
	{
		// Auto correct the invalid value to minimum value
		value = min;
		infoMsg |= BME680_I_MIN_CORRECTION;
	}

	// Check if value is above maximum value
	if( value > max )
	{
		// Auto correct the invalid value to maximum value
		value = max;
		infoMsg |= BME680_I_MAX_CORRECTION;
	}

	return rslt;
}

#define BME680_TMP_BUFFER_LENGTH UINT8_C( 40 )

bool BME680Controller::setRegs( std::uint8_t* regAddr, std::uint8_t* regData, std::uint8_t len )
{
	int8_t rslt;
	/* Length of the temporary buffer is 2*(length of register)*/
	uint8_t tmpBuff[ BME680_TMP_BUFFER_LENGTH ] = { 0 };
	uint16_t index;

	if( len < BME680_TMP_BUFFER_LENGTH / 2 )
	{
		// Interleave the 2 arrays
		for( index = 0; index < len; index++ )
		{
			tmpBuff[ ( 2 * index ) ] = regAddr[ index ];
			tmpBuff[ ( 2 * index ) + 1 ] = regData[ index ];
		}

		// Write the interleaved array
		return m_busController.write( m_bme680Address, tmpBuff[ 0 ], &tmpBuff[ 1 ], ( 2 * len ) - 1 );
	}

	return false;
}

} // namespace PBL::I2C
