
set(PRIVATE_DEPS
    PBL::I2C
    PBL::Math
    PBL::Utils
)

create_application(
    TARGET mcp23017_blinking_led_example
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES main.cpp # rename to blinking_led_example.cpp
)

# create_application(
#     TARGET mcp23017_interrupt_button_click_example
#     PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
#     SRC_FILES interrupt_button_click_example.cpp
# )
