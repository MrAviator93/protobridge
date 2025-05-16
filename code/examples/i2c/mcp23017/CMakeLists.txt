
set(PRIVATE_DEPS
    PBL::I2C
    PBL::Math
    PBL::Utils
)

create_application(
    TARGET mcp23017_blinking_led_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES BlinkingLedExample.cpp
)

create_application(
    TARGET mcp23017_interrupt_pull_up_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES InterruptPullUpExample.cpp
)

create_application(
    TARGET mcp23017_interrupt_pull_down_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES InterruptPullDownExample.cpp
)

create_application(
    TARGET mcp23017_pull_up_resistor_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES PullUpResistorExample.cpp
)

create_application(
    TARGET mcp23017_pull_down_resistor_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES PullDownResistorExample.cpp
)

