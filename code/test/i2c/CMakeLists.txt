
set(PRIVATE_DEPS
    PBL::Utils
    PBL::Math
    PBL::I2C
)

set(SRC
    BusControllerTests.cpp
)

create_test_application(
    TARGET test_i2c
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
