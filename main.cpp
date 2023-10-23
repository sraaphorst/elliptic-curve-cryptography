/**
 * big_int.h
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#include "big_int.h"
#include "printable.h"
#include "operations.h"

using namespace ecc;
using namespace ecc::printable;
using namespace ecc::operations;

int main() {
    BigInt a(255);
    BigInt b("24386246891236498216984621934812");
    BigInt c("4382957847829743892748927432");
    BigInt d = (-a * b) % c + 3;
    std::cout << '(' << -a << " * " << b << ") % " << c << " + 3 = " << d << '\n';

    std::cout << b << " >= " << c << ": " << (b >= c) << '\n';
}