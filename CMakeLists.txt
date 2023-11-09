cmake_minimum_required(VERSION 3.21)
project(elliptic_curve_cryptography)

set(CMAKE_CXX_STANDARD 20)
#add_compile_options(-fconcepts)

find_library(GMP_LIBRARY NAMES gmp)
find_path(GMP_INCLUDE_DIR gmp.h)

if(NOT GMP_LIBRARY)
    message(FATAL_ERROR "GMP not found.")
endif()

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY     https://github.com/fmtlib/fmt.git
        GIT_TAG            10.1.1
)
FetchContent_MakeAvailable(fmt)

enable_testing()
add_subdirectory(src)
