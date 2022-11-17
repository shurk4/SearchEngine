cmake_minimum_required(VERSION 3.21)

add_executable(SearchEngine
        main.cpp
        json_converter.cpp
        invertedIndex.cpp
        searchServer.cpp)

target_include_directories(SearchEngine PUBLIC ${INCLUDE_DIR})

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)