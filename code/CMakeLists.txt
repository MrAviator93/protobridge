cmake_minimum_required(VERSION "3.29.0")

project(protobridge VERSION 1.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(PBL_BUILD_BENCH "Build benchmarks" ON)
option(PBL_BUILD_TESTS "Build unit tests" ON)
option(PBL_BUILD_EXAMPLES "Build examples" ON)
option(PBL_BUILD_UTILITIES "Build utilities" ON)
option(PBL_CORE_GENERATE_DOC "Generate documentation." ON)
option(PBL_USE_CCACHE "Use ccache to speed up recompilation" ON)

if(PBL_USE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    endif()
endif()

# Enable/Disable build individual libraries
option(PBL_BUILD_I2C_LIB "Build I2C library" ON)
option(PBL_BUILD_SPI_LIB "Build SPI library" ON)
option(PBL_BUILD_GPIO_LIB "Build Gpio library" ON)
option(PBL_BUILD_SERIAL_LIB "Build Serial library" ON)
option(PBL_BUILD_DEVICES_LIB "Build Devices library" ON)

include(CreateApplication)
include(CreateStaticLibrary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) 

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # TODO: Add more stricter compiler flags
    add_compile_options(-O2 -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wshadow -Wunused -fexceptions -pedantic)
endif()

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

if (PBL_BUILD_GPIO_LIB)
    pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)
endif()

add_subdirectory(source)

if (PBL_BUILD_BENCH)
    find_package(benchmark CONFIG REQUIRED)
    add_subdirectory(bench)
endif()

if (PBL_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)
    include(CTest)
    set(BUILD_TESTING ON)
    set(CMAKE_CTEST_ARGUMENTS "--verbose")
    add_subdirectory(test)
endif()

if (PBL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (PBL_BUILD_UTILITIES)
    add_subdirectory(utilities)
endif()

if (PBL_CORE_GENERATE_DOC)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # Check for dot tool of Graphviz
        if (NOT DOXYGEN_DOT_EXECUTABLE)
            message(WARNING "Graphviz's dot tool not found. Graphs in documentation might not be generated.")
        endif()
        
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    else()
        message("Doxygen has not been found on the current system. Install it in order to generate the documentation.")
    endif()
endif()

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list( APPEND CMAKE_CXX_CPPCHECK 
            "--enable=all"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )

    add_custom_target(analyze
        COMMAND ${CMAKE_CXX_CPPCHECK} ${CMAKE_SOURCE_DIR}/source/
        COMMENT "Static code analysis using ${CPP_CHECK_VERSION}"
    )
endif()
