include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(invertedIndex_test invertedIndex_test.cpp ${INCLUDE_DIR}/invertedIndex.h ${SOURCE_DIR}/invertedIndex.cpp)
target_link_libraries(invertedIndex_test PRIVATE gtest_main)
target_include_directories(invertedIndex_test PUBLIC ${INCLUDE_DIR})

add_executable(searchServer_test searchServer_test.cpp ${INCLUDE_DIR}/searchServer.h ${SOURCE_DIR}/searchServer.cpp ${INCLUDE_DIR}/invertedIndex.h ${SOURCE_DIR}/invertedIndex.cpp)
target_link_libraries(searchServer_test PRIVATE gtest_main)
target_include_directories(searchServer_test PUBLIC ${INCLUDE_DIR})