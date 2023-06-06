cmake_minimum_required(VERSION 3.25)
project(calculator_matrix)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/include/x86_64-linux-gnu/qt5)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(calculator_matrix main.cpp)

target_link_libraries(calculator_matrix PRIVATE Eigen3::Eigen Qt5::Widgets)