
cmake_minimum_required(VERSION 3.13)

project(tira LANGUAGES CXX)
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(Catch2)

include(CTest)
enable_testing()

source_group("Tests" FILES src/tests/HeapTest.cpp)
source_group("Source files" FILES
  src/main.cpp
  src/huffman.cpp
  src/heap.cpp
  )

if (TARGET Catch2::Catch2)
  add_executable(${PROJECT_TEST_NAME}
    src/tests/HeapTest.cpp
    src/heap.cpp
    src/bitstring.cpp
    )

  target_compile_options(${PROJECT_TEST_NAME} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -fsanitize=address -fno-omit-frame-pointer)
  target_link_options(${PROJECT_TEST_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)

  target_link_libraries(${PROJECT_TEST_NAME} PRIVATE Catch2::Catch2WithMain)
  target_link_libraries(${PROJECT_TEST_NAME} PRIVATE gcov)

  target_include_directories(${PROJECT_TEST_NAME} PRIVATE headers)
  add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
endif()


add_executable(tira
  src/main.cpp
  src/huffman.cpp
  src/heap.cpp
  src/bitstring.cpp
  )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -fsanitize=address -fno-omit-frame-pointer)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
  target_compile_options(${PROJECT_NAME} PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /w44265 /w44061 /w44062 )
endif()



target_include_directories(${PROJECT_NAME} PRIVATE headers)
target_link_libraries(${PROJECT_NAME} PRIVATE m)
