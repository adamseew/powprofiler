
cmake_minimum_required(VERSION 3.5)

project (powprofiler)

set(SOURCES
    program.cpp
    src/config.cpp
    src/integrator_rk4.cpp
    src/model_1layer.cpp
    src/model_battery.cpp
    src/pathn.cpp
    src/profiler.cpp
    src/sampler_nano.cpp
    src/sampler_odroid.cpp
    src/sampler_tx2.cpp
    src/soc_1resistor.cpp
    src/utility.cpp
    src/vectorn.cpp
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread" CACHE STRING "Set C++ Compiler Flags" FORCE)

add_executable(powprofiler ${SOURCES})

target_include_directories(powprofiler
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
