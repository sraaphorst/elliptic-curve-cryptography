add_library(ecc
        big_int.cpp
        modular_int.cpp
        gmp_rng.cpp
)

target_include_directories(ecc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ecc PRIVATE ${GMP_INCLUDE_DIR})
target_link_libraries(ecc ${GMP_LIBRARY})
