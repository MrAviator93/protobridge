#ifndef PBL_I2C_MPU6050_DEFINITIONS_HPP__
#define PBL_I2C_MPU6050_DEFINITIONS_HPP__

// C++
#include <cstdint>

namespace pbl::i2c::mpu6050
{

// MPU-6050 Register Addresses
constexpr std::uint8_t kAuxVddIoRegister{ 0x01 }; //!< Auxiliary voltage level for the interface I/O
constexpr std::uint8_t kSampleRateDividerRegister{ 0x19 }; //!< Sample Rate Divider
constexpr std::uint8_t kConfigRegister{ 0x1A }; //!< General configuration
constexpr std::uint8_t kGyroConfigRegister{ 0x1B }; //!< Gyroscope configuration
constexpr std::uint8_t kAccelConfigRegister{ 0x1C }; //!< Accelerometer configuration
constexpr std::uint8_t kFifoEnableRegister{ 0x23 }; //!< FIFO buffer enable
constexpr std::uint8_t kI2CMasterControlRegister{ 0x24 }; //!< I2C master control settings
constexpr std::uint8_t kI2CDevice0AddressRegister{ 0x25 }; //!< I2C Device 0 address
constexpr std::uint8_t kI2CDevice0Register{ 0x26 }; //!< I2C Device 0 data to write
constexpr std::uint8_t kI2CDevice0ControlRegister{ 0x27 }; //!< I2C Device 0 control
constexpr std::uint8_t kI2CDevice1AddressRegister{ 0x28 }; //!< I2C Device 1 address
constexpr std::uint8_t kI2CDevice1Register{ 0x29 }; //!< I2C Device 1 data to write
constexpr std::uint8_t kI2CDevice1ControlRegister{ 0x2A }; //!< I2C Device 1 control
constexpr std::uint8_t kI2CDevice2AddressRegister{ 0x2B }; //!< I2C Device 2 address
constexpr std::uint8_t kI2CDevice2Register{ 0x2C }; //!< I2C Device 2 data to write
constexpr std::uint8_t kI2CDevice2ControlRegister{ 0x2D }; //!< I2C Device 2 control
constexpr std::uint8_t kI2CDevice3AddressRegister{ 0x2E }; //!< I2C Device 3 address
constexpr std::uint8_t kI2CDevice3Register{ 0x2F }; //!< I2C Device 3 data to write
constexpr std::uint8_t kI2CDevice3ControlRegister{ 0x30 }; //!< I2C Device 3 control
constexpr std::uint8_t kI2CDevice4AddressRegister{ 0x31 }; //!< I2C Device 4 address
constexpr std::uint8_t kI2CDevice4Register{ 0x32 }; //!< I2C Device 4 data to write
constexpr std::uint8_t kI2CDevice4DORegister{ 0x33 }; //!< I2C Device 4 direct data operation
constexpr std::uint8_t kI2CDevice4ControlRegister{ 0x34 }; //!< I2C Device 4 control
constexpr std::uint8_t kI2CDevice4DIRegister{ 0x35 }; //!< I2C Device 4 direct data input
constexpr std::uint8_t kI2CMasterStatusRegister{ 0x36 }; //!< I2C master status
constexpr std::uint8_t kIntPinConfigRegister{ 0x37 }; //!< Interrupt pin configuration
constexpr std::uint8_t kIntEnableRegister{ 0x38 }; //!< Interrupt enable
constexpr std::uint8_t kIntStatusRegister{ 0x3A }; //!< Interrupt status
constexpr std::uint8_t kAccelXOutHRegister{ 0x3B }; //!< Accelerometer X-axis high byte
constexpr std::uint8_t kAccelXOutLRegister{ 0x3C }; //!< Accelerometer X-axis low byte
constexpr std::uint8_t kAccelYOutHRegister{ 0x3D }; //!< Accelerometer Y-axis high byte
constexpr std::uint8_t kAccelYOutLRegister{ 0x3E }; //!< Accelerometer Y-axis low byte
constexpr std::uint8_t kAccelZOutHRegister{ 0x3F }; //!< Accelerometer Z-axis high byte
constexpr std::uint8_t kAccelZOutLRegister{ 0x40 }; //!< Accelerometer Z-axis low byte
constexpr std::uint8_t kTempOutHRegister{ 0x41 }; //!< Temperature high byte
constexpr std::uint8_t kTempOutLRegister{ 0x42 }; //!< Temperature low byte
constexpr std::uint8_t kGyroXOutHRegister{ 0x43 }; //!< Gyroscope X-axis high byte
constexpr std::uint8_t kGyroXOutLRegister{ 0x44 }; //!< Gyroscope X-axis low byte
constexpr std::uint8_t kGyroYOutHRegister{ 0x45 }; //!< Gyroscope Y-axis high byte
constexpr std::uint8_t kGyroYOutLRegister{ 0x46 }; //!< Gyroscope Y-axis low byte
constexpr std::uint8_t kGyroZOutHRegister{ 0x47 }; //!< Gyroscope Z-axis high byte
constexpr std::uint8_t kGyroZOutLRegister{ 0x48 }; //!< Gyroscope Z-axis low byte

// Registers 0x49-0x60 are additional sensor data registers and are not individually named.
// They can be used to read sensor data for auxiliary I2C devices.
constexpr std::uint8_t kExtSensorData00Register{ 0x49 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData01Register{ 0x4A }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData02Register{ 0x4B }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData03Register{ 0x4C }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData04Register{ 0x4D }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData05Register{ 0x4E }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData06Register{ 0x4F }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData07Register{ 0x50 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData08Register{ 0x51 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData09Register{ 0x52 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData10Register{ 0x53 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData11Register{ 0x54 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData12Register{ 0x55 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData13Register{ 0x56 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData14Register{ 0x57 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData15Register{ 0x58 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData16Register{ 0x59 }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData17Register{ 0x5A }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData18Register{ 0x5B }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData19Register{ 0x5C }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData20Register{ 0x5D }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData21Register{ 0x5E }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData22Register{ 0x5F }; //!< External Sensor Data
constexpr std::uint8_t kExtSensorData23Register{ 0x60 }; //!< External Sensor Data
constexpr std::uint8_t kI2CSlave0DataOutRegister{ 0x63 }; //!< I2C Slave 0 Data Out
constexpr std::uint8_t kI2CSlave1DataOutRegister{ 0x64 }; //!< I2C Slave 1 Data Out
constexpr std::uint8_t kI2CSlave2DataOutRegister{ 0x65 }; //!< I2C Slave 2 Data Out
constexpr std::uint8_t kI2CSlave3DataOutRegister{ 0x66 }; //!< I2C Slave 3 Data Out
constexpr std::uint8_t kI2CMasterDelayControlRegister{ 0x67 }; //!< I2C Master Delay Control
constexpr std::uint8_t kSignalPathResetRegister{ 0x68 }; //!< Signal path reset
constexpr std::uint8_t kMotionDetectControlRegister{ 0x69 }; //!< Motion detection control
constexpr std::uint8_t kUserControlRegister{ 0x6A }; //!< User control
constexpr std::uint8_t kPowerManagement1Register{ 0x6B }; //!< Power management 1
constexpr std::uint8_t kPowerManagement2Register{ 0x6C }; //!< Power management 2
constexpr std::uint8_t kFifoCountHRegister{ 0x72 }; //!< FIFO count registers (high byte)
constexpr std::uint8_t kFifoCountLRegister{ 0x73 }; //!< FIFO count registers (low byte)
constexpr std::uint8_t kFifoReadWriteRegister{ 0x74 }; //!< FIFO read/write
constexpr std::uint8_t kWhoAmIRegister{ 0x75 }; //!< Device ID (Who Am I)

enum class PowerMode : std::uint8_t
{
	SLEEP = 0x40, // Put the device in sleep mode
	CYCLE = 0x20, // Cycle between sleep mode and waking up to take single sample readings
	TEMP_DIS = 0x08, // Disable temperature sensor
	CLKSEL_PLL_X = 0x01, // Clock Source: PLL with X-axis gyroscope reference
	CLKSEL_PLL_Y = 0x02, // Clock Source: PLL with Y-axis gyroscope reference
	CLKSEL_PLL_Z = 0x03, // Clock Source: PLL with Z-axis gyroscope reference
	CLKSEL_INTERNAL_8MHZ = 0x00, // Clock Source: Internal 8MHz oscillator
};

enum class FIFOBufferMode : std::uint8_t
{
	/// FIFO buffer is disabled.
	DISABLED = 0x00,

	/// When FIFO is full, new data will not be written to FIFO.
	HOLD = 0x01,

	/// When FIFO is full, the oldest data will be discarded, and new data will be written (circular buffer).
	OVERWRITE = 0x02,
};

enum class InterruptEnable : std::uint8_t
{
	DATA_RDY_INT = 0x01, // Data Ready interrupt: Triggered when new data is available.
	I2C_MST_INT = 0x08, // I2C Master interrupt: Triggered by I2C Master events (requires the I2C_MST_EN bit to be set).
	FIFO_OFLOW_INT = 0x10, // FIFO Overflow interrupt: Triggered when the FIFO buffer becomes full.
	ZMOT_INT = 0x20, // Zero Motion Detection interrupt: Triggered when zero motion is detected.
	MOT_INT = 0x40, // Motion Detection interrupt: Triggered by acceleration.
	FF_INT = 0x80, // Free Fall interrupt: Triggered when free fall is detected.
};

enum class InterruptPinConfig : std::uint8_t
{
	ACTIVE_HIGH = 0x00,
	ACTIVE_LOW = 0x01,
	OPEN_DRAIN = 0x02,
	PUSH_PULL = 0x03,
	LATCH_INT_EN = 0x04,
	INT_ANYRD_CLEAR = 0x10,
};

enum class MotionDetection : std::uint8_t
{
	MOTION_THRESHOLD = 0x1F, // Value for motion detection threshold.
	MOTION_DURATION = 0x20, // Duration for which motion must be present.
	ZERO_MOTION_THRESHOLD = 0x21,
	ZERO_MOTION_DURATION = 0x22,
};

enum class I2CMasterControl : std::uint8_t
{
	MULT_MST_EN = 0x80,
	WAIT_FOR_ES = 0x40,
	SLV_3_FIFO_EN = 0x20,
	I2C_MST_P_NSR = 0x10,
	I2C_MST_CLK = 0x0F, // You may need individual settings for different clock values.
};

enum class SampleRateDivider : std::uint8_t
{
	DIV_0 = 0x00, // This would give you the highest sampling rate (i.e., the internal rate).
	DIV_1 = 0x01, // This would divide the internal sampling rate by 2.
	DIV_2 = 0x02, // This would divide the internal sampling rate by 3.
	// ...
	DIV_254 = 0xFE, // This would divide the internal sampling rate by 255.
	DIV_255 = 0xFF // This would divide the internal sampling rate by 256 (lowest sampling rate).
};

enum class UserControl : std::uint8_t
{
	SIG_COND_RESET = 0x01,
	I2C_MST_RESET = 0x02,
	FIFO_RESET = 0x04,
	I2C_IF_DIS = 0x10, // Disable I2C, use SPI only (if supported).
	I2C_MST_EN = 0x20, // Enable I2C Master mode.
	FIFO_EN = 0x40, // Enable FIFO buffer.
};

enum class AccelSensitivity : std::uint8_t
{
	G_2 = 0x00, // +/- 2g (default)
	G_4 = 0x08, // +/- 4g
	G_8 = 0x10, // +/- 8g
	G_16 = 0x18 // +/- 16g
};

enum class GyroSensitivity : std::uint8_t
{
	DPS_250 = 0x00, // +/- 250 degrees/s (default)
	DPS_500 = 0x08, // +/- 500 degrees/s
	DPS_1000 = 0x10, // +/- 1000 degrees/s
	DPS_2000 = 0x18 // +/- 2000 degrees/s
};

} // namespace pbl::i2c::mpu6050
#endif // PBL_I2C_MPU6050_DEFINITIONS_HPP__
