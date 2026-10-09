#ifndef PBL_I2C_TRANSPORT_HPP__
#define PBL_I2C_TRANSPORT_HPP__

#include <utils/Result.hpp>

// C++
#include <cstdint>
#include <span>

namespace pbl::i2c
{
inline namespace v1
{

/// Byte transport. Success means the full transfer completed; buffers are borrowed for the call.
class Transport
{
public:
	/// Unshifted 7-bit I2C device address.
	using Address = std::uint8_t;

	virtual ~Transport() = default;

	[[nodiscard]] virtual utils::Result< void > write( Address address, std::span< const std::uint8_t > data ) = 0;
	[[nodiscard]] virtual utils::Result< void > read( Address address, std::span< std::uint8_t > data ) = 0;

	/// Writes a request and reads the response in one combined transaction.
	[[nodiscard]] virtual utils::Result< void >
	writeRead( Address address, std::span< const std::uint8_t > request, std::span< std::uint8_t > response ) = 0;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_TRANSPORT_HPP__
