cmake_minimum_required(VERSION 3.1)

project(2DRealGasModel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## this is for local library compiling
#include_directories(extlibs/SFML/include/)
#add_subdirectory(extlibs/SFML/)

#include_directories(extlibs/eigen/)
#add_subdirectory(extlibs/eigen/)

#include_directories(extlibs/json/single_include)
#add_subdirectory(extlibs/json/)

#find_library(SFML COMPONENTS system window graphics)
#find_library(Eigen3 COMPONETS Dense)
#find_library(nlohmann_json COMPONENTS json)
##

## this is for library installed in system
find_library(SFML COMPONENTS system window graphics)
find_library(Eigen3 COMPONENTS Dense)
find_package(nlohmann_json COMPONENTS json)
##

set(EXECUTABLE_NAME project)

add_executable(${EXECUTABLE_NAME} src/main.cpp)

target_link_libraries(${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system)
## for local library compiling
#target_link_libraries(${EXECUTABLE_NAME} Eigen3::Eigen)
#
target_link_libraries(${EXECUTABLE_NAME} Eigen)
target_link_libraries(${EXECUTABLE_NAME} nlohmann_json::nlohmann_json)
