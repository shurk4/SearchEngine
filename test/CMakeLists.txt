include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(Tests
        invertedIndex_test.cpp
        searchServer_test.cpp
        )

set(Source
        ${SOURCE_DIR}/invertedIndex.cpp
        ${SOURCE_DIR}/searchServer.cpp
        )

add_executable(GTestTests ${Tests} ${Source})

target_link_libraries(GTestTests PRIVATE gtest_main)
target_include_directories(GTestTests PUBLIC ${INCLUDE_DIR})