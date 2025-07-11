
set(PBL_LIB_HEADERS
    Gpio.hpp
    Utils.hpp
    GpioFwd.hpp
    GpioLine.hpp
    Rpi5Chip0.hpp
)

set(PBL_LIB_SOURCE
    Utils.cpp
    GpioLine.cpp
    Rpi5Chip0.cpp
)

set(PBL_LIB_PRIVATE_DEPS
    PBL::Utils
    PBL::Math
)

set(PBL_LIB_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    LIB_NAMESPACE PBL
    LIB_NAME_BASE Gpio
    LIB_PUBLIC_HEADERS ${PBL_LIB_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)
