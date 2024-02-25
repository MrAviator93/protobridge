#ifndef PBL_I2C_IC_BASE_HPP__
#define PBL_I2C_IC_BASE_HPP__

// C++
#include <span>
#include <chrono>
#include <cstdint>
#include <type_traits>

namespace PBL::I2C
{

class BusController;

/**
 * @brief A concept that checks for valid I2C address types.
 *
 * `AddressType` is a concept used to constrain a type to be either a `std::uint8_t` 
 * or an enum with an underlying type of `std::uint8_t`. This is particularly useful 
 * when defining types representing I2C addresses in integrated circuit communications, 
 * ensuring type-safety and correctness of the used types.
 *
 * @tparam T The type to be checked against the concept.
 */
template < typename T >
concept AddressType = std::is_same_v< T, std::uint8_t > ||
					  ( std::is_enum_v< T > && std::same_as< std::underlying_type_t< T >, std::uint8_t > );

/**
 * @class ICBase
 * @brief This class serves as a base for various integrated circuit (IC) controllers, providing fundamental 
 *        functionalities and interface that all IC controllers must have.
 *
 * ICBase is an abstract class that cannot be instantiated on its own. It encapsulates behaviors and 
 * state common to all ICs, such as maintaining a reference to the I2C bus controller, storing the IC's 
 * unique address, and standard methods all IC controllers are expected to leverage (like sleep).
 * Derived classes should provide the specific implementation details for the respective ICs.
 */
class ICBase
{
public:
	virtual ~ICBase() = default;

	/// The address of the IC on the I2C bus.
	[[nodiscard]] auto address() const noexcept { return m_icAddress; }

	[[nodiscard]] bool write( std::uint8_t reg, std::uint8_t value );
	[[nodiscard]] bool write( std::uint8_t reg, std::span< std::uint8_t > data );
	[[nodiscard]] bool write( std::uint8_t reg, std::uint8_t* pData, std::uint8_t size );

	[[nodiscard]] bool read( std::uint8_t reg, std::uint8_t& result );
	[[nodiscard]] std::int16_t read( std::uint8_t reg, std::uint8_t* pData, std::uint16_t size );

protected:
	/**
     * @brief Protected constructor; Initializes the IC controller with a reference to the I2C bus and the IC address.
     *
	 * @tparam AddressType
     * @param busController Reference to the I2C bus controller, facilitating communication with the I2C bus.
     * @param icAddress The unique address of the IC on the I2C bus.
	 */
	template < AddressType T >
	ICBase( BusController& busController, const T icAddress ) noexcept
		: m_busController{ busController }
		, m_icAddress{ static_cast< std::uint8_t >( icAddress ) }
	{ }

	void sleep( std::chrono::milliseconds sleepTimeMs );
	void sleep( std::chrono::microseconds sleepTimeUs );

	[[nodiscard]] auto& controller() { return m_busController; }
	[[nodiscard]] auto& controller() const { return m_busController; }

	// TODO: GCC 14
	// [[nodiscard]] auto& controller(this auto&& self) { return self.m_busController; }

private:
	BusController& m_busController;
	std::uint8_t m_icAddress;
};

} // namespace PBL::I2C
#endif // PBL_I2C_IC_BASE_HPP__
