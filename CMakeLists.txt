cmake_minimum_required(VERSION 3.1)

project(RealGasModel
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O3 -march=native)
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

set(SOURCES
        src/main.cpp
        src/controller.h
        src/controller.cpp
        src/engine.h
        src/engine.cpp
        src/logger.h
)

set(EXTLIBS extlibs/rapidcsv/src/rapidcsv.h)

include_directories(extlibs/rapidcsv/src/)

add_executable(calc.exe ${SOURCES})
add_executable(output_format.exe src/output_formatter.cpp)

target_link_libraries(calc.exe Eigen3::Eigen)
target_link_libraries(calc.exe nlohmann_json::nlohmann_json)
target_link_libraries(calc.exe fmt::fmt)
target_link_libraries(output_format.exe)
