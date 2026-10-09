
set(PRIVATE_DEPS
    PBL::Utils
    PBL::Math
    PBL::I2C
)

set(SRC
    BMP180ControllerTests.cpp
)

create_test_application(
    TARGET test_i2c
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)

# Keep syscall wrapping isolated from controller tests and other test executables.
create_test_application(
    TARGET test_i2c_bus
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES BusControllerTests.cpp
)

target_link_options(test_i2c_bus PRIVATE
    "LINKER:--wrap=open"
    "LINKER:--wrap=close"
    "LINKER:--wrap=ioctl"
)
