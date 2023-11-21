
set(REQUIRED_LIBS
    I2C::I2CUtils
)

create_executable(kalman_filter REQUIRED_LIBS main.cpp)
