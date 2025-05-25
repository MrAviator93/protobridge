# ProtoBridge Library

Unlock the full potential of your single-board computer with the ProtoBridge Library, a robust and intuitive toolkit designed for seamless control and communication with a wide array of integrated circuits (ICs). This library transcends mere communication; it empowers users to effortlessly command and interact with diverse ICs, including temperature sensors, environmental monitors, and more, across multiple protocols.

Ideal for both hobbyists and professionals, the ProtoBridge Library simplifies the process of setting up and initiating communication with a wide range of devices inlcuding sensors, motors, accelometers, gyroscopes and others. This enables efficient reading and management of data, ensuring a smooth and streamlined workflow for your projects.

Built on the modern features of C++ 23, the ProtoBridge Library is an essential asset for reliable and effective device control. It offers broad compatibility and versatility, perfectly aligning with various single-board computer models and brands.

Whether your aim is to configure an environmental sensor or manage an I/O expander, the ProtoBridge Library facilitates intuitive and direct interaction with devices. It opens up a realm of possibilities in embedded systems and IoT applications, making sophisticated device control accessible and manageable.

![ProtoBridge](assets/image.png "ProtoBridge")

Please note that the ProtoBridge Library is currently a work in progress, and a stable release is not yet available, see [roadmap](#roadmap) for more details.

## Table of contents

- [ProtoBridge Library](#protobridge-library)
  - [Table of contents](#table-of-contents)
  - [Structure](#structure)
  - [Features](#features)
    - [Examples](#examples)
    - [Reading Temperature from LM75 Sensor](#reading-temperature-from-lm75-sensor)
    - [Blinking Led with MCP23017](#blinking-led-with-mcp23017)
    - [Advanced - Pseudo Thermostat Implementation Example](#advanced---pseudo-thermostat-implementation-example)
  - [Requirements](#requirements)
    - [I2C Tools](#i2c-tools)
  - [Quick Start](#quick-start)
    - [Building from Source](#building-from-source)
  - [Generating documentation](#generating-documentation)
  - [Motivation](#motivation)
  - [Roadmap](#roadmap)
  - [Contributing](#contributing)
    - [Branch Naming Conventions](#branch-naming-conventions)
    - [Commit Message Guidelines](#commit-message-guidelines)
    - [Example of a Commit Message](#example-of-a-commit-message)
  - [License](#license)

## Structure

The library is organized into several key sub-libraries, each offering specific functionality:

- i2c: provides controllers and utilities for I2C communication, including support for various I2C devices.
- spi: facilitates SPI communication, offering tools to interact with SPI devices.
- math: Includes mathematical utilities and controllers, such as PID controllers for implementing control systems.
- utils: Provides various utility functions and tools to assist with general tasks and improve code efficiency.

## Features

- Multi-Protocol Device Control: Facilitates efficient and low-level communication across various protocols (I2C, SPI, Serial, and GPIO).
- Compatibility with various single-board computers.
- Modern C++ 23 Features.
- Customizable and Expandable.
- User-Friendly Device Management.
- Comprehensive documentation support.
- Support for a range of popular ICs, including:
  - [MCP23017 - I/O Expander](./code/examples/i2c/mcp23017/main.cpp)
  - [LM75 - Temperature Sensor](./code/examples/i2c/lm75/main.cpp)
  - [BMP180 - Barometric Pressure Sensor](./code/examples/i2c/bmp180/main.cpp)
  - [BME680 - Environmental Sensor (temperature, humidity, pressure, and gas)](./code/examples/i2c/bme680/main.cpp)
  - [MPU6050 - Inertial Measurement Unit](./code/examples/i2c/mcp23017/main.cpp)

## Examples

Understanding how to use the library is key to integrating it effectively in your projects. Below is a practical example demonstrating how you can utilize the LM75Controller to read temperature data from an LM75 sensor.

### Reading Temperature from LM75 Sensor

This example shows you how to initialize communication with the LM75 sensor and read the temperature.

```cpp
// Include I2C library files
#include <i2c/Controllers.hpp>

// Output
#include <print>

int main(int, char**)
{
    // Create a bus controller for the I2C bus (Raspberry Pi 4)
    pbl::i2c::BusController busController{"/dev/i2c-1"};

    // Check if the I2C bus is open and accessible
    if (busController.isOpen()) 
    {
      // Create an LM75 controller, attached to the bus controller,
      // using the default device address
      pbl::i2c::LM75Controller lm75{busController};

      // Read the temperature in Celsius from the LM75 sensor
      auto temp = lm75.getTemperatureC();

      // Check wether the reading is present (indication of successful read)
      if (temp.has_value())
      {
        // Output the temperature to the console
        std::println("Temperature: {}°C", temp.value());
      }
    }
 
    return 0;
}
```

This simple program initializes the I2C bus, sets up the LM75 sensor, reads the temperature, and outputs it to the console. By following this structure, you can easily read from other sensors and devices using their respective controller classes provided by the library.

### BLinking LED with MCP23017

This example shows how to use the MCP23017 I/O expander to blink an LED by toggling a GPIO pin every 2 seconds using a software timer.

```cpp
#include <utils/Timer.hpp>
#include <i2c/BusController.hpp>
#include <i2c/MCP23017Controller.hpp>

// Output
#include <print>
#include <thread>

int main( int, char** )
{
 // Default name of i2c bus on RPI 4
 std::string deviceName{ "/dev/i2c-1" };

 // Create a bus controller for the I2C bus
 pbl::i2c::BusController busController{ deviceName };

 // Check if the I2C bus is open and accessible
 if( !busController.isOpen() )
 {
  std::println( stderr, "Failed to open I2C device" );
  return 1;
 }

 // Create an MCP23017 controller, attached to the bus controller
 pbl::i2c::MCP23017Controller mcp{ busController };
 auto pin = mcp.portA().pin( pbl::i2c::MCP23017Controller::Port::Pins::PIN_1 );

 // This API allows us to configure individual pins
 if( !pin.setMode( pbl::i2c::MCP23017Controller::Port::PinMode::OUTPUT ) )
 {
  std::println( stderr, "Error occurred when setting pin mode..." );
  return 1;
 }

 pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };

 while( true )
 {
  timer.onTick( [ &pin ]( [[maybe_unused]] auto dt ) -> void {
   if( !pin.switchPinState() )
   {
    std::println( "Failed to set pin state to ON..." );
   }
  } );

  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
 }

 return 0;
}
```

### Advanced - Pseudo Thermostat Implementation Example

```cpp
class Thermostat
{
 using PIDController = pbl::math::PIDController< float >;

public:
 Thermostat( pbl::i2c::BusController& busController )
  : m_pid{ 0.5, 0.2, 0.25 }
  , m_adc{ busController }
  , m_lm75{ busController }
  , m_thermostat{ busController }

 { }

 [[nodiscard]] utils::Result< void > update( float dt )
 {
  using PIDInput = PIDController::Input;

  return m_adc
   .readDesiredTemp()
   // Step 1: Read the desired temperature from the ADC (returns Result<float>)
   .and_then( [ this ]( float desiredTemp ) -> utils::Result< PIDInput > {
    // Step 2: Try to get the current temperature from the LM75 sensor (returns Result<float>)
    // If successful, combine both temperatures into a PIDInput structure
    // If getTemperatureC() fails, the failure propagates automatically
    return m_lm75.getTemperatureC().transform( [ & ]( float currentTemp ) {
     // If successful, produce a PIDInput with both temps
     return PIDInput{ desiredTemp, currentTemp };
    } );
   } )

   // Step 3: Apply the PID controller to compute the control signal
   .transform( [ this, dt ]( PIDInput input ) -> float {
    // The result is post-processed:
    // - Clamped to [0.0, 10.0] using `Cap`
    // - Squared using `Pow2`
    return m_pid( dt, input ) | pbl::math::Cap{ 0.0f, 10.0f } | pbl::math::Pow2{};
   } )

   // Step 4: Use the control signal to adjust the thermostat (returns Result<void>)
   .and_then( [ this ]( float controlSignal ) {
    // If this fails, the error also propagates
    return m_thermostat.adjust( controlSignal );
   } );
 }

private:
 PIDController m_pid;
 pbl::i2c::ADCController m_adc;
 pbl::i2c::LM75Controller m_lm75;
 pbl::i2c::ThermostatController m_thermostat;
};
```

```cpp

// Include I2C library files
#include "Thermostat.hpp"
#include <utils/Timer.hpp>

// Output
#include <print>

int main( const int argc, const char* const* const argv )
{
 const std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );

 // Default name of i2c bus on RPI 4
 std::string deviceName{ "/dev/i2c-1" };

 if( args.size() >= 2 )
 {
  deviceName = args[ 1 ];
 }

 // Create a bus controller for the I2C bus
 pbl::i2c::BusController busController{ deviceName };

 // Check if the I2C bus is open and accessible
 if( !busController.isOpen() )
 {
  std::println( "Failed to open I2C device" );
  return 1;
 }

 pbl::examples::Thermostat thermostat{ busController };
 pbl::utils::Timer timer{ std::chrono::milliseconds( 100 ) };

 while( true )
 {
  auto onTick = [ & ] [[nodiscard]] ( auto dt ) -> bool {
   auto rslt = thermostat.update( dt );

   std::println( "{:12f}", dt );

   if( !rslt )
   {
    std::println( stderr, "{}", pbl::utils::toStringView( rslt.error() ) );
    return false;
   }

   return true;
  };

  if( !timer.onTick( std::move( onTick ) ) )
  {
   break;
  }
 }

 return 0;
}

```

For more detailed examples, including how to interact with other types of sensors and devices, please check out the Code/Examples directory in the project repository. These examples provide a broader range of use cases and more complex scenarios, helping you to get the most out of this library.

## Requirements

- CMake (version 3.29.0 or newer).
- A C++ 23 compliant compiler (GCC-14.2, Clang-18.1).
- A single-board computer (e.g., Raspberry Pi, BeagleBone) running an OS that supports I2C ioctl system calls.

### I2C Tools

You might need additional user-space I2C tools. You can install them as follows:

```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install i2c-tools libi2c-dev
```

The i2c-tools package contains a collection of utilities for I2C, and libi2c-dev provides the header files and static libraries for I2C development.

## Quick Start

### Building from Source

Clone the repository into your local machine using the following command:

```bash
git clone git@github.com:MrAviator93/protobridge.git
```

Standard build procedure, navigate to the project directory and build the library using CMake and make:

```bash
mkdir build && cd build
cmake ../code/ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
sudo cmake --install .
```

This will compile the source code and create the necessary output directories and files.

## Generating documentation

The project is configured to generate documentation through Doxygen. You can create a comprehensive set of documentation by following these steps:

```bash
cd build
make docs
```

The HTML documentation will be generated and available in the DocDoxygen/html/ directory. Open the index.html file in your web browser to view it.

## Motivation

Diving into embedded systems and single-board computers opened a whole new world for me, one filled with endless possibilities and the sheer excitement of building something tangible. Yet, amidst this excitement, I hit a roadblock. The world of I2C communication was a jumbled mess of libraries, each marching to the beat of its own drum. For someone who thrives on C++, it felt like wading through a sea of mismatched puzzles, all with their own complicated rules. The landscape was cluttered with low-level operations, requiring developers to manipulate bits directly and wrestle with hardware specifics, often leading to frustration and lost time.

What I sought was something clean, something elegant—a single library, written in a consistent style, harnessing the full power of modern C++. I dreamt of something cleaner, more intuitive. A tool that didn't force us to get lost in the weeds with every new IC, but rather, one that cleared the path, making device interaction almost second nature.

That's why I rolled up my sleeves and started crafting this solution myself. I was determined to build a library that wasn't just a showcase of C++ 23's capabilities but also a haven for developers seeking clarity and efficiency. A place where code flows naturally, and modern development feels like a breeze. More than that, I wanted to create a comprehensive package, a one-stop-shop for all kinds of SBC to IC communication, accommodating numerous configurations for various ICs all under one roof.

This library is the fruit of that ambition: a testament to a belief in a world where sophisticated development doesn't have to be synonymous with complexity. It's here to provide a seamless, enjoyable experience for fellow developers, helping them bring their inventive projects to life with less hassle and more confidence.

## Roadmap

**ProtoBridge Alpha Release Target: September 1, 2025:**

The ProtoBridge Alpha release focuses on delivering a clean, modern C++ foundation for embedded device control. It includes a focused set of I2C, SPI, Serial, and GPIO devices to demonstrate the core architecture and enable real-world prototyping and development.

### Alpha Scope

**Interface Supported Devices:**

- I2C:
  - [x] LM75;
  - [x] BMP180;
  - [ ] TMP102;
  - [ ] SHT31;
  - [ ] MCP23017;
  - [ ] ADS1015;
  - [ ] MPU6050;
  - [ ] PCA9685.
- SPI:
  - [ ] MCP3008 (ADC);
  - [ ] TMC2130 (Stepper Motor Controller);
- Serial:
  - [ ] HC12 (transparent serial);
  - [ ] NEO-M8N GPS (raw NMEA).
- GPIO:
  - [ ] Raspberry Pi 5 GPIO (basic input/output abstraction).

### Further Plan

#### Beta Release — Target: October 25, 2025

The Beta release will expand device support, introduce initial testing tools, and launch the first ProtoBridge-compatible hardware kits.

**Planned Milestones:**

- Improve wiring documentation and code examples;
- Start community feedback loop (GitHub Discussions, Issues).

---

#### 🎄 v1.0.0 Stable Release — Target: December 20–24, 2025

The first official production-ready version of ProtoBridge, featuring stable APIs, full documentation, and real-world use cases.

**Planned Milestones:**

- Finalize and freeze core API;
- Reach 15+ supported devices with examples;
- Publish full documentation and usage guides;
- Launch “Built with ProtoBridge” showcase.

## Contributing

Contributions, issues, and feature requests are welcome! Feel free to check issues page. For major changes, please open an issue first to discuss what you would like to change.

### Branch Naming Conventions

When creating a new branch, please use the following naming conventions:

- For new features: feature/your-new-feature (e.g., feature/i2c-error-handling)
- For bug fixes: bugfix/your-bug-fix (e.g., bugfix/temperature-reading-error)
- For hot fixes: hotfix/your-hotfix (e.g., hotfix/memory-leak-fix)
- For documents: docs/your-document (.e.g., docs/document-local-gcc-build)

Please ensure that the branch names are plural, reflecting that they may include multiple changes or updates related to the same feature or fix.

### Commit Message Guidelines

A good commit message should be clear and concise, yet descriptive enough to understand the change without having to look at the code. Here's a suggested format for commit messages:

1. Subject Line: This should be a brief summary of the change, ideally not exceeding 50 characters. It should be written in imperative mood, as if commanding the codebase to change. Example: "Add temperature conversion feature."

2. Body: Following a blank line after the subject, the body should provide a detailed description of the change. You should explain the problem being addressed, the changes made to address it, and any other relevant information. The body is wrapped at 72 characters.

3. Footer: This section is used for referencing issue tracker IDs and giving credits.

### Example of a Commit Message

```txt
Add support for BMP180 sensor

- Implement BMP180 read functionality in I2C library.
- Update documentation to include BMP180 usage instructions.
- Add unit tests for BMP180 sensor readings.

Resolves: #123
Credit: @MrAviator93 for initial implementation
```

## License

Copyright 2025 MrAviator93

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
