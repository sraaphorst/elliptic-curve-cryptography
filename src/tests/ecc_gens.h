/**
 * ecc_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <iostream>
#include <rapidcheck.h>
#include <big_int.h>
#include <modular_int.h>

#include "gmp_gens.h"

namespace rc {
    template<>
    struct Arbitrary<ecc::BigInt> {
        static Gen<ecc::BigInt> arbitrary() {
            return gen::map(gen::arbitrary<gmp_mpz_t>(), [](const gmp_mpz_t& value) {
                return ecc::BigInt{value.value};
            });
        }
    };
}

namespace rc {
//    const auto arbitraryBigIntAndProbablyPrimePair = gen::suchThat<std::pair<ecc::BigInt, ecc::BigInt>>([](const std::pair<ecc::BigInt, ecc::BigInt>& pair) {
//        if (pair.second.is_probably_prime())
//            std::cerr << "Found " << pair.first.to_string() << " and probably prime: " << pair.second.to_string() << '\n';
//        return pair.second.is_probably_prime();
//    });
//
//    template<>
//    struct Arbitrary<ecc::ModularInt> {
//        static Gen<ecc::ModularInt> arbitrary() {
//            return gen::map(gen::suchThat<std::pair<gmp_mpz_t, gmp_mpz_t>>([](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
//                const auto &[value, mod] = pair;
//                const auto prob_prime = mpz_probab_prime_p(mod.value, 1);
//                if (mpz_probab_prime_p(mod.value, 1))
//                    std::cerr << "In suchThat, value=" << mpz_get_str(nullptr, 10, value.value)
//                              << ", mod=" << mpz_get_str(nullptr, 10, mod.value)
//                              << ", prime: " << prob_prime << '\n';
//                return prob_prime;
//            }), [](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
//                const auto &[value, mod] = pair;
//                const auto prob_prime = mpz_probab_prime_p(mod.value, 1);
//                std::cerr << "In map, value=" << mpz_get_str(nullptr, 10, value.value)
//                          << ", mod=" << mpz_get_str(nullptr, 10, mod.value)
//                          << ", prime: " << prob_prime << '\n';
//                ecc::ModularInt mi{value.value, mod.value};
//                return mi;
//            });
//        }
//    };

        // Generate a random gmp_mpz_t
        Gen<gmp_mpz_t> arbitraryRandomGmp() {
            return gen::exec([]() {
                mpz_t random_num;
                mpz_init(random_num);
                mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);

                std::cerr << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

                std::cerr << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
                return v;
            });
        }

        // Generate a probably prime gmp_mpz_t
        Gen<gmp_mpz_t> arbitraryPrimeGmp() {
            return gen::exec([]() {
                mpz_t random_num;
                mpz_init(random_num);

                // Generate a probably prime number
                do {
                    mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);
                } while (!mpz_probab_prime_p(random_num, 1));

                std::cerr << "generated prime: " << mpz_get_str(nullptr, 10, random_num) << '\n';
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

                std::cerr << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
                return v;
            });
        }


//    static Gen<gmp_mpz_t> arbitraryRandomGmp() {
//        return gen::exec([]() {
//            mpz_t random_num;
//            mpz_init(random_num);
//            mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);
//
//            std::cerr << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
//            gmp_mpz_t v{random_num};
//
//            mpz_clear(random_num);
//
//            std::cerr << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
//            return v;
//        });
//    }
//
//    // Generate a probably prime gmp_mpz_t
//    static Gen<gmp_mpz_t> arbitraryPrimeGmp() {
//        return gen::exec([]() {
//            mpz_t random_num;
//            mpz_init(random_num);
//
//            // Generate a probably prime number
//            do {
//                mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);
//            } while (!mpz_probab_prime_p(random_num, 1));
//
//            std::cerr << "generated prime: " << mpz_get_str(nullptr, 10, random_num) << '\n';
//            gmp_mpz_t v{random_num};
//
//            mpz_clear(random_num);
//
//            std::cerr << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
//            return v;
//        });
//    }
//
//    template<>
//    struct Arbitrary<ecc::ModularInt> {
//        static Gen<ecc::ModularInt> arbitrary() {
//            // Generate a random gmp_mpz_t
//            auto randomGen = arbitraryRandomGmp();
//
//            // Generate a probably prime gmp_mpz_t
//            auto primeGen = arbitraryPrimeGmp();
//
//            // Combine them to create ModularInt
//            return gen::map(std::make_pair(randomGen, primeGen), [](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
//                const auto &[rand, prime] = pair;
//                return ecc::ModularInt{rand.value, prime.value};
//            });
//        }
//    };

    template<>
    struct Arbitrary<ecc::ModularInt> {
        static Gen<ecc::ModularInt> arbitrary() {
            return gen::map(gen::pair(arbitraryRandomGmp(), arbitraryPrimeGmp()),
                                             [](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
                const auto &[value, mod] = pair;
                std::cerr << "In arbitrary ModularInt, value=" << mpz_get_str(nullptr, 10, value.value)
                          << ", mod=" << mpz_get_str(nullptr, 10, mod.value)
                          << ", prime=" << mpz_probab_prime_p(mod.value, 1) << '\n';
                return ecc::ModularInt{value.value, mod.value};
            });
        }
    };
//            const auto arbitraryBigInt = gen::arbitrary<ecc::BigInt>();
//        const auto zzz = gen::pair<gmp_mpz_t, gmp_mpz_t>(arbitraryRandomGmp(), arbitraryPrimeGmp());
//
//        gen::arbitrary(zzz);
//        const auto aaa = gen::arbitrary<std::pair<gmp_mpz_t, gmp_mpz_t>>(zzz);
//            const auto modularIntAsTuple = gen::tuple(arbitraryBigInt, probablyPrimeBigInt);
//            return gen::map(gen::tuple(gen::arbitrary<ecc::BigInt>(), probablyPrimeBigInt), [](const auto &vals) {
//                const auto &[value, mod] = vals;
//                return ecc::ModularInt{value, mod};
//            });
//            return gen::map(modularIntAsTuple,
//                    [] (const auto &vals) {
//                        const auto &[value, mod] = vals;
//                        std::cerr << "value=" << value.to_string() << ", mod=" << mod.to_string() << '\n';
//                        return ecc::ModularInt{value, mod};
//                    });
//            return gen::map(gen::arbitrary<std::tuple<ecc::BigInt, ecc::BigInt>>(),
//                    [](const auto &vals) {
//                        const auto &[value, mod] = vals;
//                        return ecc::ModularInt{value, mod};
//                    });
}