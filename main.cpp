#include <iostream>
#include "big_int.h"

using namespace ecc;

int main() {
    BigInt a(255);
    BigInt b("24386246891236498216984621934812");
    BigInt c = a * b;
    // 'c' now contains the result of a + b
    std::cout << a << " * " << b << " = " << c << '\n';

    return 0;
}