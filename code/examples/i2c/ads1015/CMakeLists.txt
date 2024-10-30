
set(PRIVATE_DEPS
    PBL::I2C
    PBL::Math
    PBL::Utils
)

create_application(
    TARGET ads1015_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
