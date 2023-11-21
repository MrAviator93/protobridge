
set(REQUIRED_LIBS
    I2C::I2CUtils
)

create_test_executable(test_utils REQUIRED_LIBS
    main.cpp
    MathTest.cpp
    CompactBitsetTest.cpp
)
