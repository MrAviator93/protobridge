#ifndef PBL_I2C_BUS_CONTROLLER_BASE_HPP__
#define PBL_I2C_BUS_CONTROLLER_BASE_HPP__

#include <utils/ErrorCode.hpp>

// C++
#include <bit>
#include <span>
#include <array>
#include <chrono>
#include <vector>
#include <cstdint>

namespace pbl::i2c
{

inline namespace v1
{

class BusControllerBase
{
public:
	virtual ~BusControllerBase() = default;

	[[nodiscard]] virtual bool isReady() = 0;

	/// Read a single byte from specified register
	[[nodiscard]] virtual bool read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::uint8_t& result ) = 0;

	/// Read a two byte array from specified register
	[[nodiscard]] virtual bool
	read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::array< std::uint8_t, 2 >& result ) = 0;

	/// Read a four byte array from specified register
	[[nodiscard]] virtual bool
	read( const std::uint8_t deviceAddr, const std::uint8_t reg, std::array< std::uint8_t, 4 >& result ) = 0;

	/// TBW
	[[nodiscard]] virtual bool read( const std::uint8_t deviceAddr,
									 const std::uint8_t reg,
									 std::int16_t& value,
									 const std::endian endian = std::endian::big ) = 0;

	/// TBW
	[[nodiscard]] virtual bool read( const std::uint8_t deviceAddr,
									 const std::uint8_t reg,
									 std::int32_t& value,
									 const std::endian endian = std::endian::big ) = 0;

	/// Read a data buffer from specified register (user is expected to preallocate the data buffer)
	[[nodiscard]] virtual std::int16_t
	read( const std::uint8_t deviceAdd, const std::uint8_t reg, std::span< std::uint8_t > data ) = 0;

	/// Write a single byte to the specified register
	virtual bool write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::uint8_t data ) = 0;

	/// Write a data buffer to specified register
	virtual bool
	write( const std::uint8_t deviceAddr, const std::uint8_t reg, const std::span< const std::uint8_t > data ) = 0;

	/// Puts asleep calling thread for specified sleep time in milliseconds
	virtual void sleep( const std::chrono::milliseconds sleepTimeMs ) = 0;

	/// Puts asleep calling thread for specified sleep time in microseconds
	virtual void sleep( const std::chrono::microseconds sleepTimeUs ) = 0;

protected:
	BusControllerBase() = default;

private:
	// This class is non-copyable and non-movable
	BusControllerBase( const BusControllerBase& ) = delete;
	BusControllerBase( BusControllerBase&& ) = delete;
	BusControllerBase operator=( const BusControllerBase& ) = delete;
	BusControllerBase operator=( BusControllerBase&& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_BUS_CONTROLLER_BASE_HPP__
