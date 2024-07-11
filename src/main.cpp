#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <utility>
#include "../perm/basicpermutation.h"
#include "../perm/permutation.h"
#include "../perm/sims.h"
#include "../util/privatekeygenerator.h"
#include "../util/permutation_vector_utils.h"
#include "../util/bitset_utils.h"
#include "../protocol/keypair.h"
#include "../protocol/protocol.h"
#include "../attack/attackmethod.h"


template<size_t n, size_t m>
std::pair<std::vector<BasicPermutation<n+m>>,
          std::vector<BasicPermutation<n+m>>>
get_column_products(Table<BasicPermutation<n+m>,n,m> T,
                    Table<BasicPermutation<n+m>,n,m> P)
{
    std::vector<BasicPermutation<n+m>> M,C;

    for (int i = 0; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            BasicPermutation<n+m> T_perm;
            BasicPermutation<n+m> P_perm;

            std::for_each(T.begin(), T.begin() + i, [&T_perm](const auto& row) { T_perm = T_perm * row[0]; });
            std::for_each(P.begin(), P.begin() + i, [&P_perm](const auto& row) { P_perm = P_perm * row[0]; });

            T_perm = T_perm * T[i][j];
            P_perm = P_perm * P[i][j];

            std::for_each(T.begin() + i + 1, T.end(), [&T_perm](const auto& row) { T_perm = T_perm * row[0]; });
            std::for_each(P.begin() + i + 1, P.end(), [&P_perm](const auto& row) { P_perm = P_perm * row[0]; });

            M.push_back(T_perm);
            C.push_back(P_perm);
        }
    }

    return {M,C};
}

int main(int argc, char *argv[])
{
    constexpr unsigned int n = 8;
    constexpr unsigned int k = 3;
    constexpr unsigned int m = (1<<k);

    std::random_device rd;
    std::mt19937 gen( rd());

    Table<BasicPermutation<n+m>,n,m> T;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T[i][j] = BasicPermutation<n+m>::transposition(i+1,n+j+1);

    BasicPermutation<n+m> mu;
    for (const auto &row : T)
        mu = mu * row[0];

    auto protocol = new Protocol<n,k>();

    BasicPermutation<n+m> gamma;
    for (const auto &row : protocol->getPublicKey())
        gamma = gamma * row[0];

    auto [M,C] = get_column_products<n,m>(T, protocol->getPublicKey());
    M = M * std::vector<BasicPermutation<n+m>>{mu.inverse()};
    C = C * std::vector<BasicPermutation<n+m>>{gamma.inverse()};

    auto attack = new AttackMethod<n,m>();
    attack->fit(M,C);

    constexpr int n_attempts = 100000;
    int n_success = 0;

    for (int i = 0; i < n_attempts; i++)
    {
        auto message = random_bitset<n*k>();
        auto message_vec = bitset_to_vec<n,k>(message);
        auto block_x_muinv = block_from_vec(message_vec, T)*mu.inverse();
        auto encrypted_block_x_gammainv = protocol->encrypt_block(message)*gamma.inverse();

        auto cracked = attack->decrypt(encrypted_block_x_gammainv);
        if (!cracked.first)
            continue;
        if (block_x_muinv == cracked.second)
            n_success++;
    }

    std::cout << "Success: " << double(n_success)/n_attempts * 100 << "%\n";
    return 0;
}
