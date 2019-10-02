
cmake_minimum_required(
             VERSION 3.0
            )

set         (POWPROF powprofiler)
set         (LIBPOWPROF powprof)

project     (${POWPROF} VERSION 1.0)

file        (GLOB SOURCES 
             program.cpp
             src/*.cpp
            )

set         (CMAKE_CXX_STANDARD 11)
set         (CMAKE_CXX_STANDARD_REQUIRED True)

add_library (powprof_obj OBJECT ${SOURCES})

set_property(TARGET powprof_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library (${LIBPOWPROF}_shared SHARED $<TARGET_OBJECTS:powprof_obj>)
add_library (${LIBPOWPROF}_static STATIC $<TARGET_OBJECTS:powprof_obj>)

add_executable(
             ${POWPROF} $<TARGET_OBJECTS:powprof_obj>
            )

target_link_libraries(
             ${POWPROF} pthread
            )

target_include_directories(
             ${POWPROF} PRIVATE
             ${PROJECT_SOURCE_DIR}/include
            )
