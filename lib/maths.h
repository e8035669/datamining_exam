#ifndef MATHS_H
#define MATHS_H

#include <numeric>
#include <vector>
#include <algorithm>

template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {
    std::vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(),
         [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}

#endif
