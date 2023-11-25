
set(PBL_LIB_HEADERS
    Math.hpp
    Linear.hpp
    Matrix.hpp
    KalmanFilter.hpp
    PredicionModels.hpp
    ComplementaryFilter.hpp
)

set(PBL_LIB_SOURCE
    Dummy.cpp
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
    LIB_NAME_BASE Math
    LIB_PUBLIC_HEADERS ${PBL_LIB_HEADERS}
    LIB_SOURCES ${PBL_LIB_SOURCE}
    LIB_PRIVATE_LINK_LIBS ${PBL_LIB_PRIVATE_DEPS}
    LIB_PUBLIC_INCLUDE_DIRS ${PBL_LIB_PUBLIC_INCLUDE_DIRS}
)