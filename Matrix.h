#pragma once
#include "Vector.h"
#include <cstddef>

template <typename T>
class Matrix{
private:
    Vector<Vector<T>> mat_data;
    size_t row;
    size_t col;
public:
    Matrix() : row(0), col(0), mat_data() {}
    Matrix(size_t r, size_t c) : row(r), col(c), mat_data(r) {
        for (size_t i = 0; i < r; i++) {
            mat_data[i] = Vector<T>(c);
        }
    }
    T& operator()(size_t i, size_t j) { return mat_data[i][j]; }
    const T& operator()(size_t i, size_t j) const { return mat_data[i][j]; }
    size_t numRow() { return row; }
    size_t numCol() { return col; }
    void setRow(size_t r) { row = r; } 
    void setCol(size_t c) { col = c; }
};