
cmake_minimum_required(VERSION 3.10)

project(tira)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
find_package(CxxTest)
if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
  CXXTEST_ADD_TEST(unittest_huffman huffmantree.cpp
    ${PROJECT_SOURCE_DIR}/src/tree.cpp ${PROJECT_SOURCE_DIR}/headers/tests/HuffmanTreeTests.h)
endif(CXXTEST_FOUND)

add_executable(tira src/main.cpp src/huffman.cpp src/tree.cpp)
target_include_directories(tira PRIVATE headers)
