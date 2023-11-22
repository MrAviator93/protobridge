
set(PRIVATE_DEPS
    PBL::Utils
)

create_executable(
    TARGET kalman_filter
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
