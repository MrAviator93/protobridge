function(create_static_library LIB_NAMESPACE LIB_NAME_BASE SOURCES HEADERS PRIVATE_LINK_LIBS PUBLIC_INCLUDE_DIRS)
    # set( LIB_VERSION "${I2C_UTILS_LIB_MAJOR.I2C_UTILS_LIB_MINOR.I2C_UTILS_LIB_PATCH}" )

    set(LIB_NAME_OUTPUT ${LIB_NAME_BASE})

    add_library(${LIB_NAME_OUTPUT} STATIC ${${SOURCES}})

    add_library(${LIB_NAMESPACE}::${LIB_NAME_BASE} ALIAS ${LIB_NAME_OUTPUT})

    target_link_libraries(${LIB_NAME_OUTPUT} PRIVATE ${${PRIVATE_LINK_LIBS}})
    target_include_directories(${LIB_NAME_OUTPUT} PUBLIC ${${PUBLIC_INCLUDE_DIRS}})

    target_compile_features(${LIB_NAME_OUTPUT} PUBLIC cxx_std_20)
endfunction()