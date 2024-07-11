#ifndef KEYPAIR_H
#define KEYPAIR_H

#include "../util/privatekeygenerator.h"
#include <string>
#include <vector>
#include "../perm/basicpermutation.h"
#include <bitset>
#include <array>
#include "../util/permutation_vector_utils.h"

template<size_t n, size_t k>
class KeyPair {
public:
    static constexpr size_t m = (1<<k);
private:
    Table<BasicPermutation<n + m>, n, m> publicKey;
    BasicPermutation<n + m> privateKey;
    PrivateKeyGenerator<n + m> gen;
public:
    KeyPair(Table<BasicPermutation<n + (1<<k)>, n, (1<<k)>);
    Table<BasicPermutation<n+m>, n, m> getPublicKey()
    {
        return publicKey;
    }

    BasicPermutation<n + (1<<k)> getPrivateKey() const
    {
        return privateKey;
    }

    BasicPermutation<n + (1<<k)> encrypt(std::bitset<k*n>) const;
    std::bitset<k*n> decrypt(BasicPermutation<n + (1<<k)> a) const;
};

template<size_t n, size_t k>
KeyPair<n,k>::KeyPair(Table<BasicPermutation<n+(1<<k)>,n,(1<<k)> pk) : gen(PrivateKeyGenerator<n+m>())
{
    std::random_device rd;
    std::mt19937 genn( rd());

    std::array<BasicPermutation<n+m>, n> vec = gen.template generate<n>();

    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            publicKey[i][j] = vec[i] * pk[i][j] * vec[i+1].inverse();
        }
    }

    for (int j = 0; j < m; j++)
    {
        publicKey[n-1][j] = vec[n-1] * pk[n-1][j] * vec[0];
    }

    privateKey = vec[0];
}

template<size_t n, size_t k>
BasicPermutation<n + (1<<k)> KeyPair<n,k>::encrypt(std::bitset<k*n> message_block) const
{
    BasicPermutation<n+m> a,b;

    for (int i = 0; i < n; i++)
    {
        int yi = 0;
        for (int j = 0; j < k; j++)
            yi += (1 << j)*message_block[i*k+j];

        a = a*publicKey[i][yi];
    }

    return a;
}

template<size_t n, size_t k>
std::bitset<k*n> KeyPair<n,k>::decrypt(BasicPermutation<n+ (1<<k)> a) const
{
    std::bitset<k*n> block;

    a = (privateKey.inverse()*a*privateKey.inverse());
    auto vec = a.template decompose_i<m>();

    for (int i = 0; i < n; i++)
    {
        auto bits = std::bitset<k>(vec.value()[i]-1);
        for (int j = 0; j < k; j++)
            block[i*k+j] = bits[j];
    }

    return block;
}
#endif // KEYPAIR_H
