function(create_application)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs PRIVATE_DEPENDENCIES PUBLIC_DEPENDENCIES SRC_FILES)

    # Parse the arguments passed to the function
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${ARG_TARGET} ${ARG_SRC_FILES})

    if(UNIX)
        list(APPEND ARG_PRIVATE_DEPENDENCIES "Threads::Threads")
        set_target_properties(${ARG_TARGET} PROPERTIES LINK_FLAGS "-no-pie")
    endif()

    target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_PRIVATE_DEPENDENCIES})
    target_link_libraries(${ARG_TARGET} PUBLIC ${ARG_PUBLIC_DEPENDENCIES})
endfunction()

function(create_test_application)
    create_application(${ARGN})
    add_test(NAME ${ARGV1} COMMAND ${ARGV1})
endfunction()
