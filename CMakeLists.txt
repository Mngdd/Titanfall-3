﻿cmake_minimum_required(VERSION 3.20)

project("Titanfall-3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Dcpp_variable")

cmake_path(SET LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(
        ${LIB_DIR}
)

add_subdirectory (Titanfall-3)