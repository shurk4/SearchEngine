cmake_minimum_required(VERSION 3.21)

add_executable(SearchEngine
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/json_converter.cpp
        ${SOURCE_DIR}/invertedIndex.cpp
        ${SOURCE_DIR}/searchServer.cpp)

target_include_directories(SearchEngine PUBLIC ${INCLUDE_DIR})

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json Threads::Threads)
else()
    target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)
endif(UNIX)
