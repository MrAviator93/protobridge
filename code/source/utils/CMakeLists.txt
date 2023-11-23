
set(I2C_DEVICES_HEADERS
    Math.hpp
    Timer.hpp
    Linear.hpp
    Matrix.hpp
    TimeStep.hpp
    SpinLock.hpp
    TimeTypes.hpp
    FastPimpl.hpp
    KalmanFilter.hpp
    StaticString.hpp
    CompactBitset.hpp
    PredicionModels.hpp
    RandomGenerator.hpp
    ComplementaryFilter.hpp
)

set(PBL_LIB_SOURCE
    Timer.cpp
    SpinLock.cpp
)

set(PBL_LIB_PRIVATE_DEPS)

set(PBL_LIB_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    LIB_NAMESPACE PBL
    LIB_NAME_BASE Utils
    LIB_PUBLIC_HEADERS ${I2C_DEVICES_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)
