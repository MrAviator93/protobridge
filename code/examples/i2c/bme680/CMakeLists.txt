
set(PRIVATE_DEPS
    PBL::I2C
    PBL::Utils
)

create_application(
    TARGET bme680_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
