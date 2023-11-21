
#include <libftdi1/ftdi.h>

#include <format>
#include <cstdint>
#include <iostream>

namespace
{

constexpr int kFtdiChipVendorId{ 0x0403 };
constexpr int kFtdiChipProductId{ 0x6014 };

} // namespace

int main()
{
	int ret{};
	struct ftdi_context ftdi = {};

	// Initialize context for subsequent function calls
	::ftdi_init( &ftdi );

	// Open FTDI device based on FT232H vendor & product IDs
	ret = ::ftdi_usb_open( &ftdi, kFtdiChipVendorId, kFtdiChipProductId );
	if( ret < 0 )
	{
		auto errMsg = std::format( "Unable to open device: {}", ::ftdi_get_error_string( &ftdi ) );
		std::cerr << errMsg << std::endl;
		return 1;
	}

	// Set up the high-speed I2C clock
	// The specific calls and setup sequences will depend heavily on your device's characteristics
	// and the libftdi functions. Please refer to the FTDI and libftdi documentation for details.

	// Write and Read from your I2C device
	// This will involve specific sequences of writing and reading bytes
	// according to the protocol of the I2C device you are communicating with.

	// Don't forget error handling for each step

	// Close the device when done
	::ftdi_usb_close( &ftdi );
	::ftdi_deinit( &ftdi );

	return 0;
}
