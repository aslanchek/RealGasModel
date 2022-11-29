cmake_minimum_required(VERSION 3.1)

project(SFMLRealGasModel
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O3 -march=native) # CMake 2.8.12 or newer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp" )
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

## this is for local library compiling
include_directories(extlibs/eigen/)
add_subdirectory(extlibs/eigen/)

include_directories(extlibs/json/single_include/)
add_subdirectory(extlibs/json/)

include_directories(extlibs/fmt/include/)
add_subdirectory(extlibs/fmt/)

find_library(Eigen3 COMPONETS Dense)
find_library(nlohmann_json COMPONENTS json)
find_library(fmt COMPONENTS fmt)
##

set(EXECUTABLE_NAME executable)
set(SOURCES
        src/main.cpp
        src/controller.h
        src/controller.cpp
        src/engine.h
        src/engine.cpp
        src/logger.h
	)

set(EXTLIBS )
include_directories(extlibs/)

add_executable(${EXECUTABLE_NAME} ${EXTLIBS} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} Eigen3::Eigen)
target_link_libraries(${EXECUTABLE_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${EXECUTABLE_NAME} fmt::fmt)
