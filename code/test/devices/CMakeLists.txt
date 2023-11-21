
set(REQUIRED_LIBS
    I2C::I2CUtils
)

create_test_executable(test_devices REQUIRED_LIBS
    main.cpp
)
