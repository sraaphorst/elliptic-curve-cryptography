add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rapidcheck)

include_directories(rapidcheck)
include_directories(../ecc)

# The second line is necessary to get the files in this directory to recognize GMP includes.
add_executable(test_big_int test_big_int.cpp)
target_include_directories(test_big_int PRIVATE ${GMP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_big_int ecc rapidcheck ${GMP_LIBRARY})
add_test(NAME TestBigInt COMMAND test_big_int)

add_executable(test_modular_int test_modular_int.cpp)
target_include_directories(test_modular_int PRIVATE ${GMP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_modular_int ecc rapidcheck ${GMP_LIBRARY})
add_test(NAME TestModularInt COMMAND test_modular_int)



