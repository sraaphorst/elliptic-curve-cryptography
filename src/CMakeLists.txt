add_subdirectory(ecc)
add_subdirectory(tests)

add_executable(main main.cpp)
target_include_directories(main PRIVATE ${GMP_INCLUDE_DIR})
target_link_libraries(main ecc)
