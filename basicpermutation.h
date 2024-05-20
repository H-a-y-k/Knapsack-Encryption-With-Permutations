#ifndef BASICPERMUTATION_H
#define BASICPERMUTATION_H

#include <iostream>
#include <array>
#include <random>
#include <optional>
#include <iomanip>

template<size_t n>
class BasicPermutation
{
protected:
    std::array<int, n> values;

public:
    BasicPermutation();
    BasicPermutation(const BasicPermutation<n> &other);
    BasicPermutation(const std::array<int, n> &values);

    std::array<int, n> getValues() const
    {
        return values;
    }

    BasicPermutation<n> inverse() const;
    BasicPermutation<n> set(int i, int j);
    int at(int i) const;

    int operator()(int i) const;
    BasicPermutation<n> operator*(const BasicPermutation<n> &other);
    bool operator==(const BasicPermutation<n> &other);
    bool operator!=(const BasicPermutation<n> &other);
    BasicPermutation<n> operator=(BasicPermutation<n> other)
    {
        values = other.getValues();
        return *this;
    }

    static BasicPermutation<n> identity()
    {
        return BasicPermutation<n>();
    }

    static BasicPermutation<n> transposition(int i,int j)
    {
        BasicPermutation<n> p;
        p.set(i, j);
        p.set(j, i);
        return p;
    }

    template<size_t m>
    std::optional<std::array<BasicPermutation<n>, n-m>> decompose();
    template<size_t m>
    std::optional<std::array<int, n-m>> decompose_i();

    static BasicPermutation<n> generate_random(std::mt19937& gen)
    {
        BasicPermutation<n> p;

        for (int i = n-1; i >= 1; i--)
        {
            std::uniform_int_distribution<int> dist(0, i);

            int j = dist(gen);

            int temp = p(i+1);
            p.set(i+1,p(j+1));
            p.set(j+1,temp);
        }
        return p;
    }

    friend std::ostream& operator<<(std::ostream& os, const BasicPermutation<n> &p)
    {
        for (int i = 1; i <= p.values.size(); i++)
            os << std::setw(2) << i << " ";
        os << "\n";
        for (int i = 1; i <= p.values.size(); i++)
            os << std::setw(2) << p(i) << " ";
        os << "\n";
        return os;
    }
};

template<size_t n>
BasicPermutation<n>::BasicPermutation()
{
    static_assert(n > 0, "the size must be greater than zero");

    for (int i = 0; i < n; i++)
        values[i]=i+1;
}

template<size_t n>
BasicPermutation<n>::BasicPermutation(const BasicPermutation<n> &other)
{
    static_assert(n > 0, "the size must be greater than zero");

    values = other.values;
}

template<size_t n>
BasicPermutation<n>::BasicPermutation(const std::array<int, n> &_values)
{
    static_assert(n > 0, "the size must be greater than zero");

    values = _values;
}

template<size_t n>
BasicPermutation<n> BasicPermutation<n>::inverse() const
{
    BasicPermutation<n> inv;
    for(int i = 1; i <= n; ++i){
        inv.set(values[i-1], i);
    }
    return inv;
}

template<size_t n>
int BasicPermutation<n>::operator()(int i) const
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");
    return values[i-1];
}

template<size_t n>
BasicPermutation<n> BasicPermutation<n>::set(int i, int j)
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");

    if (j < 1)
        throw std::invalid_argument("Permutation value can not be 0 or negative");
    if (j > n)
        throw std::invalid_argument("Permutation value out of range: a(" + std::to_string(i) + ") = " + std::to_string(j));

    values[i-1]=j;
    return *this;
}

template<size_t n>
int BasicPermutation<n>::at(int i) const
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");
    return values[i-1];
}

template<size_t n>
template<size_t m>
std::optional<std::array<BasicPermutation<n>, n-m>> BasicPermutation<n>::decompose()
{
    std::array<BasicPermutation<n>, n-m> decomposition;
    auto a = *this;
    for (int i = 1; i <= n-m; i++)
    {
        auto t = transposition(i, a(i));

        if (a(i) <= n-m)
            return {};
        a = t*a;
        decomposition[i-1] = t;
    }
    return decomposition;
}

template<size_t n>
template<size_t m>
std::optional<std::array<int, n-m>> BasicPermutation<n>::decompose_i()
{
    std::array<int, n-m> decomposition_i;
    auto a = *this;
    for (int i = 1; i <= n-m; i++)
    {
        auto j = a(i);

        if (a(i) <= n-m)
            return {};

        a = transposition(i, a(i)) * a;
        decomposition_i[i-1] = j-(n-m);
    }
    return decomposition_i;
}
template<size_t n>
BasicPermutation<n> BasicPermutation<n>::operator*(const BasicPermutation<n> &other)
{
    BasicPermutation<n> a;
    for (int i = 1; i <= n; i++)
    {
        a.values[i-1]=at(other(i));
    }
    return a;
}

template<size_t n>
bool BasicPermutation<n>::operator==(const BasicPermutation<n> &other)
{
    return values == other.values;
}

template<size_t n>
bool BasicPermutation<n>::operator!=(const BasicPermutation<n> &other)
{
    return values != other.values;
}
#endif // BASICPERMUTATION_H
