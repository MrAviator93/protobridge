
set(PRIVATE_DEPS
    PBL::Utils
)

set(SRC
    BitTests.cpp
    CounterTests.cpp
    FastPimplTests.cpp
    FixedStringTests.cpp
    CompactBitsetTests.cpp
)

create_test_application(
    TARGET test_utils
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
