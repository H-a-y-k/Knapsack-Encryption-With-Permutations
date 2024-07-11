#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "keypair.h"
#include "../util/permutation_vector_utils.h"
#include "../perm/sims.h"
#include "../perm/permutation.h"
#include "../perm/basicpermutation.h"

template <size_t n, size_t k>
class Protocol
{
    KeyPair<n,k> *kp;
    Table<BasicPermutation<n+(1<<k)>,n,(1<<k)> private_table;
    constexpr static size_t m = (1<<k);
    SimsGenerator<n+ (1<<k)> sims;

public:
    Protocol();
    inline
    Table<BasicPermutation<n+(1<<k)>,n,(1<<k)> getPublicKey()
    {
        return kp->getPublicKey();
    }

    BasicPermutation<n + (1<<k)> encrypt_block(std::bitset<k*n>) const;
};

template <size_t n, size_t k>
Protocol<n,k>::Protocol()
{
    std::random_device rd;
    std::mt19937 genn( rd());
    Table<BasicPermutation<n+m>,n,m> P;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            P[i][j] = BasicPermutation<n+m>::transposition(i+1,n+j+1);
    }

    kp = new KeyPair<n,k>(P);
}

template <size_t n, size_t k>
BasicPermutation<n + (1<<k)> Protocol<n,k>::encrypt_block(std::bitset<k*n> block) const
{
    auto en = kp->encrypt(block);
    return en;
}
#endif // PROTOCOL_H
