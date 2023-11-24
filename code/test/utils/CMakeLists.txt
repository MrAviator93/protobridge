
set(PRIVATE_DEPS
    PBL::Math
    PBL::Utils
)

set(SRC
    main.cpp
    MathTest.cpp
    CompactBitsetTest.cpp
)

create_test_application(
    TARGET test_utils
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
