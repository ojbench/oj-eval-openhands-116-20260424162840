cmake_minimum_required(VERSION 3.5.0)
project(Game)
set(CMAKE_CXX_STANDARD 11)


set(CMAKE_BUILD_TYPE Debug)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(Game Game.cpp)

