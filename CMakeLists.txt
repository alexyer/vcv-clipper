cmake_minimum_required(VERSION 3.13)
project(vcv_clipper)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(../Rack-SDK ../Rack-SDK/include)

add_executable(vcv_clipper
        src/MyModule.cpp
        src/Template.cpp
        src/Template.hpp)
