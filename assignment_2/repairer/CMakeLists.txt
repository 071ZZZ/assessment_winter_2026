cmake_minimum_required(VERSION 3.16)
project(repairer CXX)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(repairer
    repairer.cpp
)

target_link_libraries(repairer
    ${OpenCV_LIBS}
)