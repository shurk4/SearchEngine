include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(json_test ../temp/json_converter_test.cpp ${INCLUDE_DIR}/json_converter.h ${SOURCE_DIR}/json_converter.cpp)
target_link_libraries(json_test PRIVATE gtest_main)
target_link_libraries(json_test PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(json_test PUBLIC ${INCLUDE_DIR})

add_executable(invertedindex_test invertedindex_test.cpp ${INCLUDE_DIR}/invertedindex.h ${SOURCE_DIR}/invertedindex.cpp)
target_link_libraries(invertedindex_test PRIVATE gtest_main)
target_include_directories(invertedindex_test PUBLIC ${INCLUDE_DIR})

add_executable(searchServer_test searchServer_test.cpp ${INCLUDE_DIR}/searchServer.h ${SOURCE_DIR}/searchServer.cpp ${INCLUDE_DIR}/invertedindex.h ${SOURCE_DIR}/invertedindex.cpp)
target_link_libraries(searchServer_test PRIVATE gtest_main)
target_include_directories(searchServer_test PUBLIC ${INCLUDE_DIR})