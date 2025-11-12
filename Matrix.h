#pragma once
#include "Vector.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <variant>
#include <algorithm>


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
    Matrix(const Matrix& other) : row(other.row), col(other.col), mat_data(other.mat_data) {}

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            row = other.row;
            col = other.col;
            mat_data = other.mat_data;
        }
        return *this;
    }

    Matrix(Matrix&& other) noexcept : row(other.row), col(other.col), mat_data(std::move(other.mat_data)) {
        other.row = 0;
        other.col = 0;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            row = other.row;
            col = other.col;
            mat_data = std::move(other.mat_data);
            other.row = 0;
            other.col = 0;
        }
        return *this;
    }
    void setRow(size_t r, const Vector<T>& rowVec) {
        if (r >= row)
            throw std::out_of_range("Row index out of range in Matrix::setRow()");
        if (rowVec.size() != col)
            throw std::runtime_error("Row size mismatch in Matrix::setRow()");
        mat_data[r] = rowVec;
    }

    void printMatrix() const {
        std::cout << "\nMatrix (" << row << "x" << col << "):\n";
        for (size_t i = 0; i < row; i++) {
            for (size_t j = 0; j < col; j++) {
                std::cout << mat_data[i][j] << "\t";
            }
            std::cout << "\n";
        }
    }
    T& operator()(size_t i, size_t j) { return mat_data[i][j]; }
    const T& operator()(size_t i, size_t j) const { return mat_data[i][j]; }
    Vector<T>& operator[](size_t i) { return mat_data[i]; }
    const Vector<T>& operator[](size_t i) const { return mat_data[i]; }
    size_t numRow() { return row; }
    size_t numCol() { return col; }
    void setRow(size_t r) { row = r; } 
    void setCol(size_t c) { col = c; }
    void partialPivot(size_t k) {
        size_t maxIndex = k;
        T maxVal = std::fabs(mat_data[k][k]);
        for (size_t i = k + 1; i < row; i++) {
            if(std::fabs(mat_data[i][k]) > maxVal) {
                maxVal = std::fabs(mat_data[i][k]);
                maxIndex = i;
            }
        }
        if (maxIndex != k) {
            std::swap(mat_data[k], mat_data[maxIndex]);
        }
    }
    void gaussianElimination() {
        for (size_t k = 0; k < row; k++) {
            partialPivot(k);
            T pivot = mat_data[k][k];
            if (pivot == 0) 
                continue;
            for (size_t j = k; j < col; j++) {
                mat_data[k][j] /= pivot;
            }
            for (size_t i = k + 1; i < row; i++) {
                T factor = mat_data[i][k];
                for (size_t j = k; j < col; j++) {
                    mat_data[i][j] -= factor * mat_data[k][j];
                }
            }
        }
    }
    Vector<std::variant<double, std::string>> backSubstitution() {
        Vector<std::variant<double, std::string>> x(col - 1, 0.0);

        Vector<bool> isPivotCol(col - 1, false);
        for (size_t j = 0; j < col - 1; j++) {
            for (size_t i = 0; i < row; i++) {
                if (std::fabs(mat_data[i][j]) > 1e-9) {
                    isPivotCol[j] = true;
                    break;
                }
            }
        }
        for (size_t j = 0; j < col - 1; j++) {
            if(!isPivotCol[j]) {
                x[j] = "x" + std::to_string(j + 1);
            }
        }
        for (int i = static_cast<int>(row) - 1; i >= 0; i--) {
            int pivotCol = -1;
            for (size_t j = 0; j < col - 1; j++) {
                if (std::fabs(mat_data[i][j]) > 1e-9) {
                    pivotCol = static_cast<int>(j);
                    break;
                }
            }
            if (pivotCol == -1) {
                continue;
            }
            double rhs = mat_data[i][col - 1];
            bool dependFree = false;
            std::ostringstream depend;
            depend << "(" << rhs;
            if (pivotCol >= static_cast<int>(col - 1))
                continue;
            for (size_t j = pivotCol + 1; j < col - 1; j++) {
                if (std::holds_alternative<double>(x[j])) {
                    rhs -= mat_data[i][j] * std::get<double>(x[j]);
                } else {
                    dependFree = true;
                    depend << " - " << mat_data[i][j] << "*" << std::get<std::string>(x[j]);  
                }
            }
            if (pivotCol < 0 || pivotCol >= static_cast<int>(col)) {
                std::cout << "[WARN] Skipping invalid pivotCol " << pivotCol
                            << " at row " << i << " (col=" << col << ")\n";
            continue;
            }
            if (pivotCol >= static_cast<int>(mat_data[i].size())) {
                std::cout << "[ERROR] pivotCol out of range for this row!\n";
                continue;
            }   
            if (dependFree) {
                depend << ") /" << mat_data[i][pivotCol];
                x[pivotCol] = depend.str();
            } else {
                x[pivotCol] = rhs / mat_data[i][pivotCol];
            }
        }
        return x;
    }

    Vector<std::variant<double, std::string>> solve() {
        gaussianElimination();
        return backSubstitution();
    }


};