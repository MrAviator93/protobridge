
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

set(PBL_LIB_SOURCE
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

set(PBL_LIB_PRIVATE_DEPS
    PBL::Utils
)

set(PBL_LIB_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    LIB_NAMESPACE PBL
    LIB_NAME_BASE I2C
    LIB_PUBLIC_HEADERS ${I2C_DEVICES_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)
