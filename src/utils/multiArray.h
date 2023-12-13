#pragma once

#include <array>
#include <cstddef>

// https://stackoverflow.com/questions/76860140/convenient-way-to-declare-2d-or-even-higher-dimension-arrays-with-stdarray
// https://www.learncpp.com/cpp-tutorial/multidimensional-stdarray/
template <class T, std::size_t size, std::size_t... sizes> struct MDArray {
    using type = std::array<typename MDArray<T, sizes...>::type, size>;
};

template <class T, std::size_t size> struct MDArray<T, size> {
    using type = std::array<T, size>;
};

template <class T, std::size_t... sizes> using Array = typename MDArray<T, sizes...>::type;