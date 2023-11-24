
set(PRIVATE_DEPS
    PBL::Utils
)

create_application(
    TARGET kalman_filter
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
