add_subdirectory(math)
add_subdirectory(utils)

if(PBL_BUILD_I2C_LIB)
    add_subdirectory(i2c)
endif()

if(PBL_BUILD_SPI_LIB)
    add_subdirectory(spi)
endif()

if(PBL_BUILD_GPIO_LIB)
    # TODO
endif()

if(PBL_BUILD_SERIAL_LIB)
    # TODO
endif()

if(PBL_BUILD_DEVICES)
    # TODO
endif()