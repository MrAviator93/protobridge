
set(PBL_LIB_HEADERS
    Bit.hpp
    Timer.hpp
    Error.hpp
    Result.hpp
    Counter.hpp
    TimeStep.hpp
    SpinLock.hpp
    ErrorCode.hpp
    TimeTypes.hpp
    FastPimpl.hpp
    TimeStamp.hpp
    PinConfig.hpp
    Overloaded.hpp
    FixedString.hpp
    EnumFlagSet.hpp
    CompactBitset.hpp
    RandomGenerator.hpp
)

set(PBL_LIB_SOURCE
    Error.cpp
    Timer.cpp
    SpinLock.cpp
    TimeStamp.cpp
)

set(PBL_LIB_PRIVATE_DEPS)

set(PBL_LIB_PUBLIC_INCLUDE_DIRS
    $<INSTALL_INTERFACE:include/utils>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

create_static_library(
    LIB_NAMESPACE PBL
    LIB_NAME_BASE Utils
    LIB_PUBLIC_HEADERS ${PBL_LIB_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)
