
cmake_minimum_required(
             VERSION 3.0
            )

set         (POWPROF powprof)

set         (POWPROF_HEADERS
             include/async.h
             include/model_2layer.hpp
             include/sampler.hpp
             include/soc_1resistor.hpp
             include/config.hpp
             include/model_battery.hpp
             include/sampler_nano.hpp
             include/utility.hpp
             include/integrator_rk4.hpp
             include/pathn.hpp
             include/sampler_odroid.hpp
             include/vectorn.hpp
             include/model_1layer.hpp
             include/profiler.hpp
             include/sampler_tx2.hpp
            )

project     (${POWPROF} VERSION 1.0.1 DESCRIPTION "powprofiler is a profiling tool to build computation energy models for mobile robot")

file        (GLOB SOURCES 
             program.cpp
             src/*.cpp
            )

set         (CMAKE_CXX_STANDARD 11)
set         (CMAKE_CXX_STANDARD_REQUIRED True)

add_library (${POWPROF}_obj OBJECT ${SOURCES})

set_property(TARGET ${POWPROF}_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library (${POWPROF} SHARED $<TARGET_OBJECTS:${POWPROF}_obj>)
add_library (${POWPROF}_staticlib STATIC $<TARGET_OBJECTS:${POWPROF}_obj>)
include     (GNUInstallDirs)

set_target_properties(
             ${POWPROF} PROPERTIES VERSION ${PROJECT_VERSION}
            )

set_target_properties(
             ${POWPROF} PROPERTIES SOVERSION 1
            )

set_target_properties(
             ${POWPROF} PROPERTIES PUBLIC_HEADER "${POWPROF_HEADERS}"
            )


add_executable(
             ${POWPROF}ile $<TARGET_OBJECTS:${POWPROF}_obj>
            )

target_link_libraries(
             ${POWPROF}ile pthread
            )

target_include_directories(
             ${POWPROF} PRIVATE
             ${PROJECT_SOURCE_DIR}/include
            )

install     (
	         TARGETS ${POWPROF}
             LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
             PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/powprof
	        )

