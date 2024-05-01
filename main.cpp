#include <iostream>
#include "permutation.h"
#include "privatekeygenerator.h"
#include "protocol.h"

template< size_t size>
typename std::bitset<size> random_bitset()
{
    typename std::bitset<size> bits;
    std::random_device rd;
    std::mt19937 gen( rd());
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

int main(int argc, char *argv[])
{
    constexpr int n = 4;
    constexpr int m = 2;
    constexpr int k = 2;

    KeyPair<n,k> kp;
    std::bitset<k*n> bits = random_bitset<k*n>();

    std::cout << "message: " << bits << "\n\n";
    auto en = kp.encrypt(bits);
    std::cout << "encrypted:\n" << en << "\n";
    auto de = kp.decrypt(en);
    std::cout << "decrypted: " << de << "\n";

    return 0;
}
