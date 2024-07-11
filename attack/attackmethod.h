#ifndef ATTACKMETHOD_H
#define ATTACKMETHOD_H

#include <array>
#include <utility>
#include "../perm/permutation.h"
#include "../perm/basicpermutation.h"
#include "../perm/sims.h"
#include "../util/permutation_vector_utils.h"

template <size_t n, size_t m>
class AttackMethod
{
    SimsGenerator<n+m> *simsC;

public:
    AttackMethod();
    ~AttackMethod();

    void fit(std::vector<BasicPermutation<n+m>> M, std::vector<BasicPermutation<n+m>> C);
    std::pair<bool, BasicPermutation<n+m>> decrypt(BasicPermutation<n+m> a);
};

template <size_t n, size_t m>
AttackMethod<n,m>::AttackMethod()
{
    simsC = new SimsGenerator<n+m>;
}

template <size_t n, size_t m>
AttackMethod<n,m>::~AttackMethod()
{
    delete simsC;
}

template <size_t n, size_t m>
void AttackMethod<n,m>::fit(std::vector<BasicPermutation<n+m>> M, std::vector<BasicPermutation<n+m>> C)
{
    simsC->feed(zip<n+m>(C, M));
    while (simsC->hasNext() && simsC->delta > -50)
        simsC->next();
    // std::cout << "Group size: " << simsC->group_size();
}

template <size_t n, size_t m>
std::pair<bool, BasicPermutation<n+m>> AttackMethod<n,m>::decrypt(BasicPermutation<n+m> a)
{
    auto decomp = simsC->cascade(Permutation<n+m>(a));

    if (decomp.getPerm() != BasicPermutation<n+m>::identity())
    {
        std::array<int, n> default_vec;
        return std::make_pair(false, BasicPermutation<n+m>());
    }

    return std::make_pair(true, decomp.getShadow().inverse());
}

#endif // ATTACKMETHOD_H
