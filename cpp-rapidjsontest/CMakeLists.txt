cmake_minimum_required(VERSION 3.15)
project(cpp-jsontest)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)
add_compile_definitions(RAPIDJSON_NO_SIZETYPEDEFINE=1)
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/rapidjson/include")

include_directories(BEFORE
        "${RAPIDJSON_INCLUDE_DIR}"
        )

add_definitions(-DRAPIDJSON_HAS_STDSTRING)

set (src
        src/rapidjson.hpp)