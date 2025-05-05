
set(PRIVATE_DEPS
    PBL::Gpio
    PBL::Math
    PBL::Utils
    PkgConfig::GPIOD
)

create_application(
    TARGET rpi5_gpio_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp
)
