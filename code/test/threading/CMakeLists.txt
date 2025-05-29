
set(PRIVATE_DEPS
    PBL::Threading
)

set(SRC
    MtQueueTests.cpp
)

create_test_application(
    TARGET test_threading
    PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}
    SRC_FILES ${SRC}
)
