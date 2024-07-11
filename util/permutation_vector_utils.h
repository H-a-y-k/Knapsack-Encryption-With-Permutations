#ifndef PERMUTATION_VECTOR_UTILS_H
#define PERMUTATION_VECTOR_UTILS_H

#include "../perm/permutation.h"
#include "../perm/basicpermutation.h"
#include <vector>
#include <array>
#include <algorithm>

template<typename T, size_t rows, size_t cols>
class Table {
public:
    Table() : data(rows, std::vector<T>(cols)) {}

    std::vector<std::vector<T>> to_vector()
    {
        return data;
    }

    std::vector<T>& operator[](size_t index) {
        return data[index];
    }

    const std::vector<T>& operator[](size_t index) const {
        return data[index];
    }

    auto begin() {
        return data.begin();
    }

    auto end() {
        return data.end();
    }

    auto begin() const {
        return data.begin();
    }

    auto end() const {
        return data.end();
    }

private:
    std::vector<std::vector<T>> data;
};

template<size_t n>
std::vector<BasicPermutation<n>> operator*(std::vector<BasicPermutation<n>> vec1, std::vector<BasicPermutation<n>> vec2)
{
    std::vector<BasicPermutation<n>> product;

    for (auto p1 : vec1)
    {
        for (auto p2 : vec2)
        {
            product.push_back(p1*p2);
        }
    }
    return product;
}

template<size_t n>
std::vector<Permutation<n>> operator*(std::vector<Permutation<n>> vec1, std::vector<Permutation<n>> vec2)
{
    std::vector<Permutation<n>> product;

    for (auto p1 : vec1)
    {
        for (auto p2 : vec2)
        {
            product.push_back(p1*p2);
        }
    }
    return product;
}

template<size_t n>
std::vector<Permutation<n>> zip(const std::vector<BasicPermutation<n>> &vec1, const std::vector<BasicPermutation<n>> &vec2)
{
    if (vec1.size() != vec2.size())
        throw std::invalid_argument("Vector sizes must be equal");
    std::vector<Permutation<n>> zipped;
    for (int i = 0; i < vec1.size(); i++)
    {
        zipped.push_back(Permutation<n>(vec1[i], vec2[i]));
    }
    return zipped;
}

template<size_t n>
std::vector<Permutation<n>> mirror(const std::vector<Permutation<n>> &vec)
{
    std::vector<Permutation<n>> mirrored;
    std::for_each(vec.begin(), vec.end(), [&mirrored](Permutation<n> p) {
        mirrored.push_back(p.mirror());
    });
    return mirrored;
}

template <typename T, std::size_t N>
std::vector<T> concatenateSquareMatrix(const Table<T,N,N>& matrix) {
    std::vector<T> concatenated;
    concatenated.reserve(N * N); // Reserve space for efficiency

    // Iterate over each row of the matrix
    for (const auto& row : matrix) {
        // Insert each element of the row into the vector
        concatenated.insert(concatenated.end(), row.begin(), row.end());
    }

    return concatenated;
}

template <std::size_t n, size_t m>
std::vector<std::vector<int>> productIndexVector()
{
    std::vector<std::vector<int>> prod = {{}};
    std::vector<int> row;
    row.resize(m);
    for (int i = 0; i < m; i++)
        row[i] = i;
    for (int i = 0; i < n; i++)
    {
        std::vector<std::vector<int>> newprod;
        for (auto p : prod)
        {
            for (int j = 0; j < m; j++)
            {
                p.push_back(j);
                newprod.push_back(p);
                p.pop_back();
            }
        }
        prod = newprod;
    }
    return prod;
}

template<size_t n, size_t m>
BasicPermutation<n+m> block_from_vec(const std::array<int, n> &vec, const Table<BasicPermutation<n+m>,n,m> &table)
{
    auto block = BasicPermutation<n+m>();
    for (int i = 0; i < n; i++)
        block = block * table[i][vec[i]-1];
    return block;
}

#endif // PERMUTATION_VECTOR_UTILS_H
