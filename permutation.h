#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <iostream>
#include <array>
#include <random>
#include <optional>
#include <iomanip>
#include "basicpermutation.h"

template <size_t n>
class Permutation: public BasicPermutation<n>
{
protected:
    BasicPermutation<n> perm;
    BasicPermutation<n> shadow;
public:
    Permutation();
    Permutation(const BasicPermutation<n> &other, const BasicPermutation<n> &other_shadow = BasicPermutation<n>());
    Permutation(const Permutation<n> &other);
    Permutation(const std::array<int, n> &_values, const std::array<int, n> &_shadow_values = BasicPermutation<n>().getValues());

    BasicPermutation<n> getPerm() const
    {
        return perm;
    }

    BasicPermutation<n> getShadow() const
    {
        return shadow;
    }

    Permutation<n> inverse() const;
    Permutation<n> mirror() const
    {
        return Permutation<n>(shadow,perm);
    }
    int at(int i) const;
    Permutation<n> set(int i, int j);
    int operator()(int i) const;


    Permutation<n> operator*(const Permutation<n> &other);
    Permutation<n> operator=(Permutation<n> other);
    bool operator==(const Permutation<n> &other);
    bool operator==(const BasicPermutation<n> &other);
    bool operator!=(const Permutation<n> &other);
    bool operator!=(const BasicPermutation<n> &other);

    friend std::ostream& operator<<(std::ostream& os, const Permutation<n> &p)
    {
        for (int i = 1; i <= p.perm.getValues().size(); i++)
            os << std::setw(2) << i << " ";
        os << "\n";
        for (int i = 1; i <= p.perm.getValues().size(); i++)
            os << std::setw(2) << p(i) << " ";
        os << "\n";
        return os;
    }
};

template <size_t n>
Permutation<n>::Permutation()
{
    perm = BasicPermutation<n>();
    shadow = BasicPermutation<n>();
}

template <size_t n>
Permutation<n>::Permutation(const BasicPermutation<n> &other, const BasicPermutation<n> &other_shadow)
{
    perm = other;
    shadow = other_shadow;
}

template <size_t n>
Permutation<n>::Permutation(const Permutation<n> &other)
{
    perm = other.perm;
    shadow = other.getShadow();
}

template <size_t n>
Permutation<n>::Permutation(const std::array<int, n> &_values, const std::array<int, n> &_shadow_values)
{
    perm = BasicPermutation<n>(_values);
    shadow = BasicPermutation<n>(_shadow_values);
}

template<size_t n>
Permutation<n> Permutation<n>::inverse() const
{
    BasicPermutation<n> p = perm.inverse();
    BasicPermutation<n> pshadow = shadow.inverse();
    return Permutation<n>(p, pshadow);
}

template<size_t n>
int Permutation<n>::at(int i) const
{
    return perm.at(i);
}

template<size_t n>
Permutation<n> Permutation<n>::set(int i, int j)
{
    perm.set(i,j);
    return *this;
}

template<size_t n>
int Permutation<n>::operator()(int i) const
{
    return perm(i);
}


template<size_t n>
Permutation<n> Permutation<n>::operator*(const Permutation<n> &other)
{
    auto p = perm * other.perm;
    auto pshadow = shadow * other.getShadow();

    return Permutation<n>(p,pshadow);
}

template<size_t n>
Permutation<n> Permutation<n>::operator=(Permutation<n> other)
{
    perm = other.perm;
    shadow = other.getShadow();
    return *this;
}


template<size_t n>
bool Permutation<n>::operator==(const Permutation<n> &other)
{
    return perm == other.perm;
}

template<size_t n>
bool Permutation<n>::operator==(const BasicPermutation<n> &other)
{
    return perm == other;
}

template<size_t n>
bool Permutation<n>::operator!=(const Permutation<n> &other)
{
    return perm != other.perm;
}

template<size_t n>
bool Permutation<n>::operator!=(const BasicPermutation<n> &other)
{
    return perm != other;
}
#endif // PERMUTATION_H
