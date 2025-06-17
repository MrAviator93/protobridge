
set(PBL_LIB_HEADERS
    ICBase.hpp
    Controllers.hpp
    BusController.hpp
    LM75Controller.hpp
    SHT31Controller.hpp
    BMP180Controller.hpp
    TMP102Controller.hpp
    MPU6050Controller.hpp
    MPU9250Controller.hpp
    ADS1015Controller.hpp
    PCA9685Controller.hpp
    MCP23017Controller.hpp
    MCP23017Controller.ipp
)

set(PBL_LIB_SOURCE
    BusController.cpp
    ICBase.cpp
    LM75Controller.cpp
    SHT31Controller.cpp
    BMP180Controller.cpp
    TMP102Controller.cpp
    ADS1015Controller.cpp
    MPU6050Controller.cpp
    MPU9250Controller.cpp
    PCA9685Controller.cpp
    MCP23017Controller.cpp
)

set(PBL_LIB_PRIVATE_DEPS
    PBL::Utils
    PBL::Math
)

set(PBL_LIB_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/i2c>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    LIB_NAMESPACE PBL
    LIB_NAME_BASE I2C
    LIB_PUBLIC_HEADERS ${PBL_LIB_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)
