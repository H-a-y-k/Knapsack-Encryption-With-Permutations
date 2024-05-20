#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "privatekeygenerator.h"
#include <string>
#include <vector>
#include "basicpermutation.h"
#include <bitset>
#include <array>

template<size_t n, size_t m>
using PublicKey = std::array<std::array<BasicPermutation<n+m>, m>, n>;

template<size_t n, size_t k>
class KeyPair {
    PublicKey<n,(1<<k)> publicKey;
    BasicPermutation<n+(1<<k)> privateKey;
    PrivateKeyGenerator<n+(1<<k)> gen;
public:
    KeyPair();

    PublicKey<n, 1<<k> getPublicKey() const
    {
        return publicKey;
    }

    BasicPermutation<n + (1<<k)> getPrivateKey() const
    {
        return privateKey;
    }

    BasicPermutation<n+ (1<<k)> encrypt(std::bitset<k*n>) const;
    std::bitset<k*n> decrypt(BasicPermutation<n+ (1<<k)> a) const;
};

template<size_t n, size_t k>
KeyPair<n,k>::KeyPair() : gen(PrivateKeyGenerator<n+(1<<k)>())
{
    constexpr static int m = (1 << k);
    std::array<BasicPermutation<n+m>, n> vec = gen.template generate<n>();

    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            publicKey[i][j] = vec[i] * BasicPermutation<n+m>::transposition(i+1, n + j+1) * vec[i+1].inverse();
        }
    }

    for (int j = 0; j < m; j++)
    {
        publicKey[n-1][j] = vec[n-1] * BasicPermutation<n+m>::transposition(n, n + j+1) * vec[0];
    }

    privateKey = vec[0];
}

template<size_t n, size_t k>
BasicPermutation<n+(1<<k)> KeyPair<n,k>::encrypt(std::bitset<k*n> message_block) const
{
    constexpr static int m = (1 << k);
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
std::bitset<k*n> KeyPair<n,k>::decrypt(BasicPermutation<n+(1<<k)> a) const
{
    constexpr static size_t m = (1 << k);

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

#endif // PROTOCOL_H
