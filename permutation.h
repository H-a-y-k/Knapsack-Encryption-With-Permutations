#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <iostream>
#include <array>
#include <random>
#include <optional>

template<size_t n>
class Permutation
{
    std::array<int, n> values;

public:
    Permutation();
    Permutation(const Permutation<n> &other);
    Permutation(const std::array<int, n> &values);

    Permutation<n> inverse() const;
    template<size_t m>
    std::optional<std::array<Permutation<n>,n-m>> decompose();
    template<size_t m>
    std::optional<std::array<int, n-m>> decompose_i();

    Permutation set(int i, int j);
    int at(int i) const;

    static Permutation<n> identity()
    {
        return Permutation<n>();
    }

    static Permutation<n> transposition(int i,int j)
    {
        Permutation<n> p;
        p.set(i, j);
        p.set(j, i);
        return p;
    }

    static Permutation<n> generate_random(std::mt19937& gen)
    {
        Permutation<n> p;

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

    int operator()(int i) const;
    Permutation<n> operator*(Permutation<n> other);
    bool operator==(const Permutation<n> &other);
    bool operator!=(const Permutation<n> &other);

    friend std::ostream& operator<<(std::ostream& os, const Permutation<n> &p)
    {
        for (int i = 1; i <= p.values.size(); i++)
            os << i << " ";
        os << "\n";
        for (int i = 1; i <= p.values.size(); i++)
            os << p(i) << " ";
        os << "\n";
        return os;
    }
};

template<size_t n>
Permutation<n>::Permutation()
{
    static_assert(n > 0, "the size must be greater than zero");

    for (int i = 0; i < n; i++)
        values[i]=i+1;
}

template<size_t n>
Permutation<n>::Permutation(const Permutation<n> &other)
{
    static_assert(n > 0, "the size must be greater than zero");

    values = other.values;
}

template<size_t n>
Permutation<n>::Permutation(const std::array<int, n> &_values)
{
    static_assert(n > 0, "the size must be greater than zero");

    values = _values;
}

template<size_t n>
Permutation<n> Permutation<n>::inverse() const
{
    Permutation<n> inv;
    for(int i = 1; i <= n; ++i){
        inv.set(values[i-1], i);
    }
    return inv;
}

template<size_t n>
template<size_t m>
std::optional<std::array<Permutation<n>, n-m>> Permutation<n>::decompose()
{
    std::array<Permutation<n>, n-m> decomposition;
    auto a = *this;
    for (int i = n-m; i >= 1; i--)
    {
        auto t = transposition(i, a(i));

        if (a(i) <= n-m)
            return {};
        a = a*t;
        decomposition[i-1] = t;
    }
    return decomposition;
}

template<size_t n>
template<size_t m>
std::optional<std::array<int, n-m>> Permutation<n>::decompose_i()
{
    std::array<int, n-m> decomposition_i;
    auto a = *this;
    for (int i = n-m; i >= 1; i--)
    {
        auto j = a(i);

        if (a(i) <= n-m)
            return {};

        a = a*transposition(i, a(i));
        decomposition_i[i-1] = j-(n-m);
    }
    return decomposition_i;
}

template<size_t n>
int Permutation<n>::operator()(int i) const
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");
    return values[i-1];
}

template<size_t n>
Permutation<n> Permutation<n>::set(int i, int j)
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");

    if (j < 1)
        throw std::invalid_argument("Permutation value can not be 0 or negative");
    if (j > n)
        throw std::invalid_argument("Permutation value out of range");

    values[i-1]=j;
    return *this;
}

template<size_t n>
int Permutation<n>::at(int i) const
{
    if (i < 1)
        throw std::invalid_argument("Permutation index can not be 0 or negative");
    if (i > n)
        throw std::invalid_argument("Permutation index out of range");
    return values[i-1];
}

template<size_t n>
Permutation<n> Permutation<n>::operator*(Permutation<n> other)
{
    Permutation<n> a;
    for (int i = 1; i <= n; i++)
    {
        a.values[i-1]=other(values[i-1]);
    }
    return a;
}

template<size_t n>
bool Permutation<n>::operator==(const Permutation<n> &other)
{
    return values == other.values;
}

template<size_t n>
bool Permutation<n>::operator!=(const Permutation<n> &other)
{
    return values != other.values;
}


#endif // PERMUTATION_H
