cmake_minimum_required(VERSION 3.19)
project(QtQuick_Project VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
set(CMAKE_AUTOMOC ON)
# qt_standard_project_setup(REQUIRES 6.8)

# Add the src subdirectory which contains the main CMakeLists.txt
add_subdirectory(src)
