
cmake_minimum_required(VERSION 3.10)

project(tira)
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(Catch2 REQUIRED)
include(CTest)
enable_testing()

add_executable(${PROJECT_TEST_NAME}
  src/tests/HeapTest.cpp
  src/heap.cpp
  )
add_executable(tira
  src/main.cpp
  src/huffman.cpp
  src/heap.cpp
  )

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})

include(CodeCoverage)
append_coverage_compiler_flags()
setup_target_for_coverage_gcovr_html(
  NAME coverage
  EXECUTABLE ctest
  DEPENDENCIES ${PROJECT_TEST_NAME}
  )

target_include_directories(${PROJECT_NAME} PRIVATE headers)
target_include_directories(${PROJECT_TEST_NAME} PRIVATE headers)
target_link_libraries(${PROJECT_NAME} PRIVATE m)
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE gcov)

