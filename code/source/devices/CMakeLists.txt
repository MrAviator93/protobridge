
set(I2C_DEVICES_SOURCES
    # Controllers
    I2CBusController.cpp

    # Devinces
    ICBase.cpp
    LM75Controller.cpp
    BMP180Controller.cpp
    ADS1015Controller.cpp
    MPU6050Controller.cpp
    MPU9250Controller.cpp
    PCA9685Controller.cpp
    MCP23017Controller.cpp

    # BME680 To be refactored
    bme680.c
    BME680Controller.cpp
    BME680ControllerV2.cpp
)

set(I2C_DEVICES_HEADERS
    I2CBase.hpp
    Controllers.hpp
    LM75Controller.hpp
    I2CBusController.hpp
    BMP180Controller.hpp
    MPU6050Controller.hpp
    MPU9250Controller.hpp
    ADS1015Controller.hpp
    PCA9685Controller.hpp
    MCP23017Controller.hpp

    # BME680 To be refactored
    bme680.h
    bme680_defs.h
    BME680Controller.hpp
    BME680ControllerV2.hpp
)

set(I2C_DEVICES_PRIVATE_LINK_LIBS
    I2C::I2CUtils
)

set(I2C_DEVICES_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    I2C
    I2CDevices
    I2C_DEVICES_SOURCES
    I2C_DEVICES_HEADERS
    I2C_DEVICES_PRIVATE_LINK_LIBS
    I2C_DEVICES_PUBLIC_INCLUDE_DIRS
)
