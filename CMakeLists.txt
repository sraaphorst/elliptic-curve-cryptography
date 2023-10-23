cmake_minimum_required(VERSION 3.26)
project(elliptic_curve_cryptography)

set(CMAKE_CXX_STANDARD 20)
#add_compile_options(-fconcepts)

find_library(GMP_LIBRARY NAMES gmp)
find_path(GMP_INCLUDE_DIR gmp.h)

if(NOT GMP_LIBRARY)
    message(FATAL_ERROR "GMP not found.")
endif()

add_library(BigInt OBJECT big_int.cpp)
target_include_directories(BigInt PRIVATE ${GMP_INCLUDE_DIR})

add_executable(elliptic_curve_cryptography main.cpp)
target_include_directories(elliptic_curve_cryptography PRIVATE ${GMP_INCLUDE_DIR})
target_link_libraries(elliptic_curve_cryptography ${GMP_LIBRARY} BigInt)
