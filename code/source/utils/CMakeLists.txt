
set(I2C_UTILS_SOURCES
    Timer.cpp
)

set(I2C_UTILS_HEADERS
    Math.hpp
    Timer.hpp
    Vectors.hpp
    TimeStep.hpp
    TimeTypes.hpp
    FastPimpl.hpp
    KalmanFilter.hpp
    StaticString.hpp
    CompactBitset.hpp
    PredicionModel.hpp
    RandomGenerator.hpp
    ComplementaryFilter.hpp
)

set(I2C_UTILS_PRIVATE_LINK_LIBS)

set(I2C_UTILS_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    I2C
    I2CUtils
    I2C_UTILS_SOURCES
    I2C_UTILS_HEADERS
    I2C_UTILS_PRIVATE_LINK_LIBS
    I2C_UTILS_PUBLIC_INCLUDE_DIRS
)
