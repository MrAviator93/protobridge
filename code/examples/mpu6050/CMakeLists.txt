
set(REQUIRED_LIBS
    I2C::I2CDevices
    I2C::I2CUtils
)

create_executable(mpu6050_example REQUIRED_LIBS main.cpp)
