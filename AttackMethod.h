#ifndef ATTACKMETHOD_H
#define ATTACKMETHOD_H

#include <array>
#include "permutation.h"
#include "basicpermutation.h"

template <size_t n>
class AttackMethod
{
    std::array<std::array<BasicPermutation<n+m>, m>, n> T;
    std::array<std::array<BasicPermutation<n+m>, m>, n> public_key;
public:
    AttackMethod();
    void init_method();
};

#endif // ATTACKMETHOD_H
