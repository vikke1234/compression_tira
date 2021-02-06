
cmake_minimum_required(VERSION 3.10)

project(tira)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
set(CMAKE_BUILD_TYPE Debug)
find_package(Catch2 REQUIRED)
add_executable(tests src/tests/HeapTest.cpp src/heap.cpp)
add_executable(tira src/main.cpp src/huffman.cpp src/tree.cpp src/heap.cpp)

target_include_directories(tira PRIVATE headers)
target_include_directories(tests PRIVATE headers)
target_link_libraries(tira PRIVATE m)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

