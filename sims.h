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

namespace
{
std::set<int> intersect(std::set<int> a, std::set<int> b)
{

    std::set<int> intersection;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(intersection, intersection.begin()));
    return intersection;
}

std::set<int> intersect(std::vector<std::set<int>>::iterator begin, std::vector<std::set<int>>::iterator end)
{
    std::set<int> intersection = *begin;
    for (auto i = begin; i != end; i++)
    {
        intersection = intersect(intersection, *i);
    }

    return intersection;
}

template <size_t n>
std::set<int> fixed(Permutation<n> a)
{
    std::set<int> fixed;
    for (int i = 1; i <= n; i++)
    {
        if (a(i) == i)
        fixed.insert(i);
    }
    return fixed;
}

template <size_t n>
std::set<int> fixed_i(std::vector<std::set<int>> vec, int i)
{
    int k = 0;
    for (int j = 0; j < i; j++)
    {
        k += (n-j);
    }

    return intersect(vec.begin() + k, vec.end());
}

template <size_t n>
bool subset(std::set<int> A, std::set<int> B)
{
    if (A.size() > B.size())
        return false;
    if (A.size() == B.size() && A != B)
        return false;

    for (auto a : A)
    {
        if (B.find(a) == B.end())
            return false;
    }

    return true;
}

}


template <size_t n>
class SimsGenerator
{
    std::vector<Permutation<n>> stack;
    std::stack<Permutation<n>> added;
    Permutation<n> current;
    std::array<std::array<Permutation<n>,n>,n> table;

public:
    using Table = std::array<std::array<Permutation<n>, n>, n>;

    SimsGenerator();
    SimsGenerator(const Table &t) : table(t) {}

    bool hasNext()
    {
        return !added.empty();
    }

    Table getTable() { return table; }

    unsigned long long int group_size()
    {
        unsigned long long int size = 1;

        for (int i = 0; i < n; i++)
        {
            int ki = 0;
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

    void feed(std::vector<Permutation<n>>);
    SimsGenerator<n>& next();
    Permutation<n> cascade(Permutation<n> a, bool mutate = true);
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
void SimsGenerator<n>::feed(std::vector<Permutation<n>> generator)
{
    for (auto p : generator)
        cascade(p);
}

template <size_t n>
SimsGenerator<n>& SimsGenerator<n>::next()
{
    if (!hasNext())
        return *this;

    auto stackcpy = stack;
    current = added.top();
    added.pop();
    cascade(current*current);

    for (auto elem = stackcpy.begin(); elem != stackcpy.end(); elem++)
    {
        feed({*elem*current, current* (*elem)});
    }
    // std::cout << "\nnext, added: " << added.size();

    return *this;
}

template <size_t n>
Permutation<n> SimsGenerator<n>::cascade(Permutation<n> a, bool mutate)
{
    auto addedfirst = added.size();

    Permutation<n> b = a;
    std::array<int, n> decomposition;

    int i = 1;

    while (i <= n)
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
