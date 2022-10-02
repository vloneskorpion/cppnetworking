cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME NETWORK)
project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)