/**
 * ecc_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <rapidcheck.h>
#include <big_int.h>
#include <modular_int.h>
#include <vector>

#include "gmp_gens.h"

namespace rc {
    template<>
    struct Arbitrary<ecc::BigInt> {
        static Gen<ecc::BigInt> arbitrary() {
            return gen::map<gmp_mpz_t>([](gmp_mpz_t &&value) {
                return ecc::BigInt{value.value};
            });
        }
    };

//    const auto arbitraryBigIntAndProbablyPrimePair = gen::suchThat<std::pair<ecc::BigInt, ecc::BigInt>>([](const std::pair<ecc::BigInt, ecc::BigInt>& pair) {
//        if (pair.second.is_probably_prime())
//            std::clog << "Found " << pair.first.to_string() << " and probably prime: " << pair.second.to_string() << '\n';
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
//                    std::clog << "In suchThat, value=" << mpz_get_str(nullptr, 10, value.value)
//                              << ", mod=" << mpz_get_str(nullptr, 10, mod.value)
//                              << ", prime: " << prob_prime << '\n';
//                return prob_prime;
//            }), [](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
//                const auto &[value, mod] = pair;
//                const auto prob_prime = mpz_probab_prime_p(mod.value, 1);
//                std::clog << "In map, value=" << mpz_get_str(nullptr, 10, value.value)
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

                std::clog << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

                std::clog << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
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
                } while (!mpz_probab_prime_p(random_num, 25));

#ifdef DEBUG
                std::clog << "arbitrary prime: " << mpz_get_str(nullptr, 10, random_num) << '\n';
#endif
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

#ifdef DEBUG
                std::clog << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
#endif
                return v;
            });
        }


//    static Gen<gmp_mpz_t> arbitraryRandomGmp() {
//        return gen::exec([]() {
//            mpz_t random_num;
//            mpz_init(random_num);
//            mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);
//
//            std::clog << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
//            gmp_mpz_t v{random_num};
//
//            mpz_clear(random_num);
//
//            std::clog << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
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
//            std::clog << "generated prime: " << mpz_get_str(nullptr, 10, random_num) << '\n';
//            gmp_mpz_t v{random_num};
//
//            mpz_clear(random_num);
//
//            std::clog << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
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

    const auto wtf = gen::suchThat<std::vector<int>>([](std::vector<int> &&v) {
        return !v.empty() && v.size() % 7 == 0;
    });

    template<>
    struct Arbitrary<ecc::ModularInt> {
        static Gen<ecc::ModularInt> arbitrary() {
            return gen::exec([]() {
                const auto value = *gen::arbitrary<ecc::BigInt>();

                // To avoid overhead of copying extra objects, use the gmp prime generation.
                const ecc::BigInt mod{(*arbitraryPrimeGmp()).value};
                const auto pp = mod.is_probably_prime(25);
                std::clog << "Picked " << value.to_string() << " and " << mod.to_string() << " with pp=" << pp << '\n';
                return ecc::ModularInt{value, mod};
            });
        }
    };
//    template<>
//    struct Arbitrary<ecc::ModularInt> {
//        static Gen<ecc::ModularInt> arbitrary() {
//            return gen::map(gen::pair(arbitraryRandomGmp(), arbitraryPrimeGmp()),
//                                             [](const std::pair<gmp_mpz_t, gmp_mpz_t> &pair) {
//                const auto &[value, mod] = pair;
//                std::clog << "In arbitrary ModularInt, value=" << mpz_get_str(nullptr, 10, value.value)
//                          << ", mod=" << mpz_get_str(nullptr, 10, mod.value)
//                          << ", prime=" << mpz_probab_prime_p(mod.value, 1) << '\n';
//                return ecc::ModularInt{value.value, mod.value};
//            });
//        }
//    };





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
//                        std::clog << "value=" << value.to_string() << ", mod=" << mod.to_string() << '\n';
//                        return ecc::ModularInt{value, mod};
//                    });
//            return gen::map(gen::arbitrary<std::tuple<ecc::BigInt, ecc::BigInt>>(),
//                    [](const auto &vals) {
//                        const auto &[value, mod] = vals;
//                        return ecc::ModularInt{value, mod};
//                    });
}