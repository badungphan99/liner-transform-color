cmake_minimum_required(VERSION 3.12)
project(LinearTransformColor)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(LinearTransformColor main.cpp)

target_link_libraries(LinearTransformColor ${OpenCV_LIBS})