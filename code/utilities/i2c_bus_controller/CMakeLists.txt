
set(REQUIRED_LIBS
    I2C::I2CDevices
    I2C::I2CUtils
)

create_executable(bus_controller REQUIRED_LIBS main.cpp)
