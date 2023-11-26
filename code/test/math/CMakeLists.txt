
set(PRIVATE_DEPS
    PBL::Math
    PBL::Utils
)

set(SRC
    main.cpp
    MathTest.cpp
    Matrix2x2Test.cpp
)

create_test_application(
    TARGET test_math
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
