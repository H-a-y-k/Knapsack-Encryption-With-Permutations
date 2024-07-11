#ifndef BITSET_UTILS_H
#define BITSET_UTILS_H

#include <iostream>
#include <bitset>
#include <random>
#include <array>


template< size_t size>
typename std::bitset<size> random_bitset()
{
    std::random_device rd;
    std::mt19937 gen( rd());

    typename std::bitset<size> bits;
    std::bernoulli_distribution d(0.5);

    for( int n = 0; n < size; ++n)
    {
        bits[n] = d(gen);
    }

    return bits;
}

template<size_t N>
std::ostream& operator<<(std::ostream& os, const std::bitset<N> &bits)
{
    for (int i = N-1; i >= 0; i--)
        os << bits[i];
    return os;
}

template <size_t n, size_t k>
std::array<int, n> bitset_to_vec(const std::bitset<k*n> &message)
{
    std::array<int, n> vec;
    for (int i = 0; i < n; i++)
    {
        std::bitset<k> word;
        for (int j = 0; j < k; j++)
        {
            word[j] = message[i*k+j];
        }
        vec[i] = word.to_ulong()+1;
    }
    return vec;
}

#endif // BITSET_UTILS_H
