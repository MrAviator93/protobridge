
set(PRIVATE_DEPS
    PBL::I2C
    PBL::Utils
)

create_executable(
    TARGET mpu9250_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
