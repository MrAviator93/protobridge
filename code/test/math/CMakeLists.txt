
set(PRIVATE_DEPS
    PBL::Math
    PBL::Utils
)

set(SRC
    main.cpp
    MathTest.cpp
    Matrix2x2Test.cpp
    Matrix3x3Test.cpp
    Matrix4x4Test.cpp
    Matrix6x6Test.cpp
)

create_test_application(
    TARGET test_math
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
