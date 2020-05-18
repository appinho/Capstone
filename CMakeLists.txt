cmake_minimum_required(VERSION 3.7)

project(Capstone)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

find_package(OpenCV 3.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# Find all executables
file(GLOB project_SRCS src/*.cpp src/*.h)

add_executable(Capstone ${project_SRCS})
target_link_libraries(Capstone ${OpenCV_LIBRARIES})