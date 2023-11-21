
set(REQUIRED_LIBS
    I2C::I2CDevices
    I2C::I2CUtils
)

create_executable(mcp23017_example REQUIRED_LIBS main.cpp)
