#pragma once
#include "Functions.h"

template <typename T>
class Stack{
private: 
    //Vector based stack
    Vector<T> data;
public:
    //Push item onto stack
    void push(const T& val) { data.push_back(val); }
    //Pop item from stack
    void pop() {
        if (isEmpty())
            throw std::out_of_range("pop() called on empty Stack");
        data.pop_back();
    }
    //Returns top element
    T& top() { return data[data.size() - 1]; }
    //Checks if stack is empty
    bool isEmpty() { return data.size() == 0; }
    //Returns size of matrix
    size_t size() { return data.size(); }
    //Peeks at next item in stack
    T& peek(size_t depth = 0) { return data[data.size() - 1 - depth]; }
    //Clears stack
    void clear() {
        while (!isEmpty()) 
            pop();
    }

    //Print stack for test cases
    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& s) {
        for (size_t i = 0; i < s.data.size(); i++) {
            os << s.data[i] << " ";
        }
        return os;
    }
};