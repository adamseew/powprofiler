
cmake_minimum_required(VERSION 3.5)

project (pplanner)

set(SOURCES
    program.cpp
    src/config.cpp
    src/integrator_rk4.cpp
    src/model_1layer.cpp
    src/model_3layer.cpp
    src/pathn.cpp
    src/profiler.cpp
    src/sampler_tx2.cpp
    src/soc_1resistor.cpp
    src/utility.cpp
    src/vectorn.cpp
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread" CACHE STRING "Set C++ Compiler Flags" FORCE)

add_executable(pplanner ${SOURCES})

target_include_directories(pplanner
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
