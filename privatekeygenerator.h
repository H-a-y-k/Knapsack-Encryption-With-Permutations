#ifndef PRIVATEKEYGENERATOR_H
#define PRIVATEKEYGENERATOR_H

#include "basicpermutation.h"
#include <array>

template <size_t n>
class PrivateKeyGenerator
{
    std::random_device rd;
    std::mt19937 gen;
public:
    PrivateKeyGenerator();
    PrivateKeyGenerator(const PrivateKeyGenerator<n> &other);

    template <size_t count>
    std::array<BasicPermutation<n>, count> generate();
};

template <size_t n>
PrivateKeyGenerator<n>::PrivateKeyGenerator(const PrivateKeyGenerator<n> &other)
    : rd(other.rd)
    , gen(other.gen)
    , gen(rd())
{
}

template <size_t n>
PrivateKeyGenerator<n>::PrivateKeyGenerator()
    : gen(rd())
{
}

template <size_t n>
template <size_t count>
std::array<BasicPermutation<n>, count> PrivateKeyGenerator<n>::generate()
{
    static_assert(count > 0, "Count must be greater than 0");
    std::array<BasicPermutation<n>, count> vec;
    for (int i = 0; i < count; i++)
        vec[i] = BasicPermutation<n>::generate_random(gen);
    return vec;
}
#endif // PRIVATEKEYGENERATOR_H
