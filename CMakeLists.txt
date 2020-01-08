
cmake_minimum_required(
             VERSION 3.0
            )

set         (POWPROF powprof)

project     (${POWPROF} VERSION 1.0.1 DESCRIPTION "powprofiler is a profiling tool to build computation energy models for mobile robot")

file        (GLOB SOURCES 
             program.cpp
             src/*.cpp
            )

set         (CMAKE_CXX_STANDARD 11)
set         (CMAKE_CXX_STANDARD_REQUIRED True)

add_library (${POWPROF}_obj OBJECT ${SOURCES})

set_property(TARGET ${POWPROF}_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library (${POWPROF}_shared SHARED $<TARGET_OBJECTS:${POWPROF}_obj>)
add_library (${POWPROF}_static STATIC $<TARGET_OBJECTS:${POWPROF}_obj>)
include     (GNUInstallDirs)

set_target_properties(${POWPROF}_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${POWPROF}_shared PROPERTIES SOVERSION 1)

set_target_properties(${POWPROF}_shared PROPERTIES PUBLIC_HEADER include/powprof.hpp)


add_executable(
             ${POWPROF} $<TARGET_OBJECTS:${POWPROF}_obj>
            )

target_link_libraries(
             ${POWPROF} pthread
            )

target_include_directories(
             ${POWPROF} PRIVATE
             ${PROJECT_SOURCE_DIR}/include
            )

install     (
	     TARGETS ${POWPROF}_shared
             LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
             PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	    )

