
#ifndef I2C_EXAMPLES_EXAMPLE_APP_HPP__
#define I2C_EXAMPLES_EXAMPLE_APP_HPP__

#include <string>
#include <vector>

namespace I2C::Examples
{

/**
 * @brief Represents a basic application example, storing command-line arguments.
 * 
 * The class stores a view into the original `argv` arguments. It's essential 
 * that the lifetime of the provided `argv` outlives the `ExampleApp` object 
 * to ensure valid access to arguments.
 * 
 * @todo Consider moving to Utils library.
 * @todo Consider renaming to DeviceExampleApp
 * 
 * @tparam I2CControllerBusType 
 */
template < typename I2CControllerBusType >
class ExampleApp
{
public:
	virtual ~ExampleApp() = default;

	[[nodiscard]] auto& args() const { return m_args; }
	[[nodiscard]] auto& busController() const { return m_controller; }

protected:
	ExampleApp( const int argc, const char* const* const argv )
		: m_args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) )
	{ }

private:
	std::vector< std::string_view > m_args;
	I2CControllerBusType m_busController;
};

} // namespace I2C::Examples
#endif // I2C_EXAMPLES_EXAMPLE_APP_HPP__
