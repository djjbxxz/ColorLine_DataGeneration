cmake_minimum_required(VERSION 3.4...3.18)
project(gen_colorline_data_tensorflow LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
AUX_SOURCE_DIRECTORY(. SRC_LIST)
add_subdirectory(extern/pybind11)
pybind11_add_module(gen_colorline_data_tensorflow ${SRC_LIST})


add_definitions(-D_EXPORT)
add_definitions(-DSYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
add_definitions(-DSYSTEM_VERSION="${CMAKE_SYSTEM_VERSION}")
string(TIMESTAMP CURRENT_DATE)
add_definitions(-DCOMPILE_TIME="${CURRENT_DATE}")

find_package(PythonInterp REQUIRED)
set(PYTHON_VERSION_STRING ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})
add_definitions(-DPYVERSION="${PYTHON_VERSION_STRING}")
