
cmake_minimum_required(VERSION 3.10)

project(tira)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)

add_executable(tira src/main.cpp)
