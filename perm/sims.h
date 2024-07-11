#ifndef SIMS_H
#define SIMS_H
#include <array>
#include <stack>
#include "permutation.h"
#include "basicpermutation.h"
#include <optional>
#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <iterator>
#include "../util/permutation_vector_utils.h"

template <size_t n>
class SimsGenerator
{
    std::vector<Permutation<n>> stack;
    Permutation<n> current;
    Table<Permutation<n>,n,n> table;
public:
    std::stack<Permutation<n>> added;
    int delta = 0;
    SimsGenerator();
    SimsGenerator(const Table<Permutation<n>,n,n> &t) : table(t) {}
    SimsGenerator(const Table<Permutation<n>,n,n> &t, const std::vector<Permutation<n>> &s) : table(t), stack(s) {}

    bool hasNext()
    {
        return !added.empty();
    }

    bool isIsomorphicWithShadow()
    {
        for (auto p : stack)
        {
            if (p.getPerm().order() != p.getShadow().order())
                return false;
        }
        return true;
    }

    std::vector<Permutation<n>> getStack() { return stack; }
    Table<Permutation<n>,n,n>& getTable() { return table; }

    unsigned long long int group_size()
    {
        unsigned long long int size = 1;

        for (int i = 0; i < n; i++)
        {
            unsigned long long int ki = 0;
            for (int j = i; j < n; j++)
            {
                if (i!=j && table[i][j] != Permutation<n>())
                {
                    ki++;
                }
            }
            size *= (ki + 1);
        }
        return size;
    }

    void feed(const std::vector<Permutation<n>>&);
    SimsGenerator<n>& next(bool verbose=true);
    Permutation<n> cascade(Permutation<n> a, bool mutate = true, int upto = -1);
};


template <size_t n>
SimsGenerator<n>::SimsGenerator()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            table[i][j] = Permutation<n>(BasicPermutation<n>(), BasicPermutation<n>());
    }
}

template <size_t n>
void SimsGenerator<n>::feed(const std::vector<Permutation<n>> &generator)
{
    for (auto p : generator)
        cascade(p);
}

template <size_t n>
SimsGenerator<n>& SimsGenerator<n>::next(bool verbose)
{
    auto addedinit = added.size();

    auto stackcpy = stack;
    current = added.top();
    added.pop();
    cascade(current*current);

    for (auto elem = stackcpy.begin(); elem != stackcpy.end(); elem++)
    {
        feed({*elem*current, current* (*elem)});
    }
    if (added.size() < addedinit)
        delta--;
    else if (delta < 0)
        delta++;
    if (verbose)
        std::cout << "delta: " << delta << "\n";

    return *this;
}

template <size_t n>
Permutation<n> SimsGenerator<n>::cascade(Permutation<n> a, bool mutate, int upto)
{
    auto addedfirst = added.size();

    Permutation<n> b = a;
    std::array<int, n> decomposition;

    int i = 1;

    while (i <= n && i != upto)
    {
        if (b(i) == i)
        {
            decomposition[n-i] = i-1;
            i++;
            continue;
        }

        if (table[i-1][b(i)-1].getPerm() == BasicPermutation<n>())
        {
            if (mutate)
            {
                table[i-1][b(i)-1] = b;

                for (int j = 0; j < n; j++)
                    decomposition[j]=j;
                decomposition[i-1] = b(i)-1;
                stack.push_back(b);
                added.push(b);
            }
            break;
        }
        else
        {
            b = table[i-1][b(i)-1].inverse() * b;
            decomposition[n-i] = b(i)-1;
            i++;
        }
    }
    // std::cout << "\ncascade, added diff: " << added.size() - addedfirst;
    return b;
}

#endif // SIMS_H
