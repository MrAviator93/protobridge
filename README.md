# ProtoBridge Library

Unlock the full potential of your single-board computer with the ProtoBridge Library, a robust and intuitive toolkit designed for seamless control and communication with a wide array of integrated circuits (ICs). This library transcends mere communication; it empowers users to effortlessly command and interact with diverse ICs, including temperature sensors, environmental monitors, and more, across multiple protocols.

Ideal for both hobbyists and professionals, the ProtoBridge Library simplifies the process of setting up and initiating communication with devices like the LM75 temperature sensor. This enables efficient reading and management of data, ensuring a smooth and streamlined workflow for your projects.

Built on the modern features of C++20, the ProtoBridge Library is an essential asset for reliable and effective device control. It offers broad compatibility and versatility, perfectly aligning with various single-board computer models and brands.

Whether your aim is to configure an environmental sensor or manage an I/O expander, the ProtoBridge Library facilitates intuitive and direct interaction with devices. It opens up a realm of possibilities in embedded systems and IoT applications, making sophisticated device control accessible and manageable.

## Table of contents

- [ProtoBridge Library](#protobridge-library)
  - [Table of contents](#table-of-contents)
  - [Features](#features)
  - [Examples](#examples)
    - [Reading Temperature from LM75 Sensor](#reading-temperature-from-lm75-sensor)
  - [Requirements](#requirements)
    - [I2C Tools](#i2c-tools)
  - [Quick Start](#quick-start)
    - [Building from Source](#building-from-source)
  - [Generating documentation](#generating-documentation)
  - [Motivation](#motivation)
  - [Contributing](#contributing)
  - [License](#license)

## Features

- Multi-Protocol Device Control: Facilitates efficient and low-level communication across various protocols (I2C, SPI, Serial, and GPIO).
- Compatibility with various single-board computers.
- Modern C++20 Features.
- Customizable and Expandable.
- User-Friendly Device Management.
- Comprehensive documentation support.
- Support for a range of popular ICs, including:
  - [MCP23017 - I/O Expander](./code/examples/mcp23017/main.cpp)
  - [LM75 - Temperature Sensor](./code/examples/lm75/main.cpp)
  - [BMP180 - Barometric Pressure Sensor](./code/examples/bmp180/main.cpp)
  - [BME680 - Environmental Sensor (temperature, humidity, pressure, and gas)](./code/examples/bme680/main.cpp)
  - [MPU6050 - Inertial Measurement Unit](./code/examples/mcp23017/main.cpp)

## Examples

Understanding how to use the library is key to integrating it effectively in your projects. Below is a practical example demonstrating how you can utilize the LM75Controller to read temperature data from an LM75 sensor.

### Reading Temperature from LM75 Sensor

This example shows you how to initialize communication with the LM75 sensor and read the temperature.

```cpp
// Include I2C library files
#include <i2c/Controllers.hpp>

// Output
#include <format>
#include <iostream>

int main(int, char**)
{
    // Create a bus controller for the I2C bus (Raspberry Pi 4)
    I2C::I2CBusController busController{"/dev/i2c-1"};

    // Check if the I2C bus is open and accessible
    if (busController.isOpen()) 
    {
      // Create an LM75 controller, attached to the bus controller,
      // using the default device address
      I2C::LM75Controller lm75{busController};

      // Read the temperature in Celsius from the LM75 sensor
      auto temp = lm75.getTemperatureC();

      // Check wether the reading is present (indication of successful read)
      if (temp.has_value())
      {
        // Output the temperature to the console
        std::cout << std::format( "Temperature: {}°C", temp.value() ) << std::endl;
      }
    }
 
    return 0;
}
```

This simple program initializes the I2C bus, sets up the LM75 sensor, reads the temperature, and outputs it to the console. By following this structure, you can easily read from other sensors and devices using their respective controller classes provided by the library.

For more detailed examples, including how to interact with other types of sensors and devices, please check out the Code/Examples directory in the project repository. These examples provide a broader range of use cases and more complex scenarios, helping you to get the most out of this library.

## Requirements

- Doctest
- CMake (version 3.22.0 or newer).
- A C++20 compliant compiler (GCC-13.2, Clang-17, etc.).
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
git clone git@github.com:MrAviator93/I2CDeviceLibrary.git
```

Standard build procedure, navigate to the project directory and build the library using CMake and make:

```bash
mkdir Build
cd Build
cmake ../Code/ -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

This will compile the source code and create the necessary output directories and files.

## Generating documentation

The project is configured to generate documentation through Doxygen. You can create a comprehensive set of documentation by following these steps:

```bash
cd Build
make docs
```

The HTML documentation will be generated and available in the DocDoxygen/html/ directory. Open the index.html file in your web browser to view it.

## Motivation

Diving into embedded systems and single-board computers opened a whole new world for me, one filled with endless possibilities and the sheer excitement of building something tangible. Yet, amidst this excitement, I hit a roadblock. The world of I2C communication was a jumbled mess of libraries, each marching to the beat of its own drum. For someone who thrives on C++, it felt like wading through a sea of mismatched puzzles, all with their own complicated rules. The landscape was cluttered with low-level operations, requiring developers to manipulate bits directly and wrestle with hardware specifics, often leading to frustration and lost time.

What I sought was something clean, something elegant—a single library, written in a consistent style, harnessing the full power of modern C++. I dreamt of something cleaner, more intuitive. A tool that didn't force us to get lost in the weeds with every new IC, but rather, one that cleared the path, making device interaction almost second nature.

That's why I rolled up my sleeves and started crafting this solution myself. I was determined to build a library that wasn't just a showcase of C++20's capabilities but also a haven for developers seeking clarity and efficiency. A place where code flows naturally, and modern development feels like a breeze. More than that, I wanted to create a comprehensive package, a one-stop-shop for I2C communication, accommodating numerous configurations for various ICs all under one roof.

This library is the fruit of that ambition: a testament to a belief in a world where sophisticated development doesn't have to be synonymous with complexity. It's here to provide a seamless, enjoyable experience for fellow developers, helping them bring their inventive projects to life with less hassle and more confidence.

## Contributing

Contributions, issues, and feature requests are welcome! Feel free to check issues page. For major changes, please open an issue first to discuss what you would like to change.

## License

Copyright 2023 MrAviator93

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
