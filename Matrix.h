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
    /*Create a function that can check to see if a variable is free (if a variable is not a pivot it is free)
    Then create a back sub fn that will find all the unknown variables
    print fn
    fn that combines fns to make it so i just need to call one fn*/
    //Add var(x1 or t1) in pivot check sec?
    //
    Vector<std::variant<double, std::string>> backSubstitution() {
        Vector<std::variant<double, std::string>> x(col - 1);
        for (size_t j = 0; j < col - 1; j++) 
            x[j] = 0.0;
        int freeVarCount = 1;
        for (size_t j = 0; j < col - 1; j++) {
            bool isPivot = false;
            for (size_t i = 0; i < row; i++) {
                if (std::fabs(mat_data[i][j]) > 1e-9) {
                    isPivot = true;
                    break;
                }
            }
            if (!isPivot) {
                x[j] = "x" + std::to_string(j);
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
            double sum = 0.0;
            bool dependFree = false;
            std::ostringstream depend;
            for (size_t j = pivotCol + 1; j < col - 1; j++) {
                if (std::holds_alternative<double>(x[j])) {
                    sum += mat_data[i][j] * std::get<double>(x[j]);
                } else {
                    dependFree = true;
                    depend << mat_data[i][j] << " * " << std::get<std::string>(x[j]) << " + ";  
                }
            }
            double rhs = static_cast<double>(mat_data[i][col - 1]) - sum;
            if (dependFree) {
                depend.str("");
                depend.clear();
                depend << "(" << rhs;
                for (size_t j = pivotCol + 1; j < col - 1; j++) {
                    if(std::holds_alternative<std::string>(x[j])) {
                        depend << " - " << mat_data[i][j] << " * " << std::get<std::string>(x[j]);
                    }
                }
                depend << ") / " << mat_data[i][pivotCol];
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