cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(Horizon VERSION 0.1.0 LANGUAGES CXX)

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

find_package(OpenCV REQUIRED)

add_executable(Horizon_v1 ${PROJECT_SOURCE_DIR}/v1/main.cpp)
target_link_libraries(Horizon_v1 ${OpenCV_LIBS})

add_executable(Horizon_v2 ${PROJECT_SOURCE_DIR}/v2/main.cpp)
target_link_libraries(Horizon_v2 ${OpenCV_LIBS})