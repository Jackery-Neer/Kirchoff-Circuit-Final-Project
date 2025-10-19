#pragma once
#include "Functions.h"

template <typename T>
class Stack{
private: 
    Vector<T> data;
public:
    void push(const T& val) { data.push_back(val); }
    void pop();
    T& top() { return data[data.size() - 1]; }
    bool isEmpty() { return data.size() == 0; }
    size_t size() { return data.size(); }
    T& peek(size_t depth = 0) { return data[data.size() - 1 - depth]; }
    void clear();
    
    //For print for test cases, can prolly delete when done or keep to show how to test
    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& s);
};