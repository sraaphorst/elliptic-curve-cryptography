enable_testing()

add_subdirectory(rapidcheck)

include_directories(rapidcheck)
include_directories(../ecc)

add_executable(test_big_int test_big_int.cpp)

# This line is necessary to get the files in this directory to recognize GMP includes.
target_include_directories(test_big_int PRIVATE ${GMP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_big_int ecc rapidcheck ${GMP_LIBRARY})

add_test(NAME TestBigInt COMMAND test_big_int)
