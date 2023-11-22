function(create_static_library)
    set(options)
    set(oneValueArgs LIB_NAME_BASE LIB_NAMESPACE)
    set(multiValueArgs LIB_SOURCES LIB_PRIVATE_LINK_LIBS LIB_PUBLIC_LINK_LIBS LIB_PUBLIC_HEADERS LIB_PUBLIC_INCLUDE_DIRS)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # set( LIB_VERSION "${I2C_UTILS_LIB_MAJOR.I2C_UTILS_LIB_MINOR.I2C_UTILS_LIB_PATCH}" )

    # Create the library
    add_library(${ARG_LIB_NAME_BASE} STATIC ${ARG_LIB_SOURCES})

    add_library(${ARG_LIB_NAMESPACE}::${ARG_LIB_NAME_BASE} ALIAS ${ARG_LIB_NAME_BASE} )

    # If public headers are provided, make them part of the library's interface
    if(ARG_PUBLIC_HEADERS)
        target_sources(${ARG_LIB_NAME_BASE} PUBLIC ${ARG_LIB_PUBLIC_HEADERS})
    endif()

    # Link private and public libraries
    target_link_libraries(${ARG_LIB_NAME_BASE} PRIVATE ${ARG_LIB_PRIVATE_LINK_LIBS})
    target_link_libraries(${ARG_LIB_NAME_BASE} PUBLIC ${ARG_LIB_PUBLIC_LINK_LIBS})

    target_include_directories(${ARG_LIB_NAME_BASE} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>)

    # Set target compile features
    target_compile_features(${ARG_LIB_NAME_BASE} PUBLIC cxx_std_20)
endfunction()