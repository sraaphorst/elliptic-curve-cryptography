# CMake generated Testfile for 
# Source directory: /Users/vorpal/dev/C++/elliptic-curve-cryptography/src/tests
# Build directory: /Users/vorpal/dev/C++/elliptic-curve-cryptography/build/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[TestBigInt]=] "/Users/vorpal/dev/C++/elliptic-curve-cryptography/build/src/tests/test_big_int")
set_tests_properties([=[TestBigInt]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/vorpal/dev/C++/elliptic-curve-cryptography/src/tests/CMakeLists.txt;14;add_test;/Users/vorpal/dev/C++/elliptic-curve-cryptography/src/tests/CMakeLists.txt;0;")
subdirs("rapidcheck")
