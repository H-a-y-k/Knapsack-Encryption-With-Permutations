#ifndef PRIVATEKEYGENERATOR_H
#define PRIVATEKEYGENERATOR_H

#include "permutation.h"
#include <array>

template <size_t n>
class PrivateKeyGenerator
{
    std::random_device rd;
    std::mt19937 gen;
public:
    PrivateKeyGenerator();
    PrivateKeyGenerator(const PrivateKeyGenerator<n>& other)
    {
        rd = other.rd;
        gen = other.gen;
    }

    template <size_t count>
    std::array<Permutation<n>, count> generate();
};

template <size_t n>
PrivateKeyGenerator<n>::PrivateKeyGenerator()
    : gen(rd())
{
}

template <size_t n>
template <size_t count>
std::array<Permutation<n>, count> PrivateKeyGenerator<n>::generate()
{
    static_assert(count > 0, "Count must be greater than 0");
    std::array<Permutation<n>, count> vec;
    for (int i = 0; i < count; i++)
        vec[i] = Permutation<n>::generate_random(gen);
    return vec;
}
#endif // PRIVATEKEYGENERATOR_H
