cmake_minimum_required(VERSION 3.15)
project(wrapper)

set(CMAKE_CXX_STANDARD 17)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

add_executable(test_parsing test_parsing.cpp)
add_executable(test_construct test_construction.cpp)
add_compile_definitions(RAPIDJSON_NO_SIZETYPEDEFINE=1)
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/rapidjson/include")

include_directories(BEFORE
        "${RAPIDJSON_INCLUDE_DIR}"
        )

add_definitions(-DRAPIDJSON_HAS_STDSTRING)

set (src
        rapidjson.hpp)

add_library(wrapper MODULE wrapper.cpp)
python_extension_module(wrapper)

add_cython_target(primes CXX)
add_library(primes MODULE ${primes})
python_extension_module(primes)

# add_cython_target(messaging CXX)
# add_library(messaging MODULE ${messaging})
# python_extension_module(messaging)

install(TARGETS wrapper LIBRARY DESTINATION wrapper)
install(TARGETS primes LIBRARY DESTINATION primes)
# install(TARGETS messaging LIBRARY DESTINATION messaging)