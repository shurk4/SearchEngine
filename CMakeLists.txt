cmake_minimum_required(VERSION 3.21)
project(SearchEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(src) # добавление поддиректории c main.cpp к проекту
add_subdirectory(test)

