
set(PRIVATE_DEPS
    PBL::Utils
)

set(SRC
    main.cpp
)

create_test_executable(
    TARGET test_devices
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
