set(PRIVATE_DEPS
    PBL::I2C
    PBL::Math
    PBL::Utils
)

create_application(
    TARGET pseudo_thermostat_controller
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
