#pragma once
#include <cstddef>

template <typename T>
class Vector {
private:
    T* data;
    size_t num_items;
    size_t _capacity;
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < num_items; i++) 
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
    }
public:
    Vector() : data(nullptr), num_items(0), _capacity(0) {}

    Vector(size_t _size) : data(nullptr), num_items(_size), _capacity(_size) {
        data = new T[_capacity];
        for (size_t i = 0; i < num_items; i++)
            data[i] = T();
    }

    ~Vector() { delete[] data; }

    void push_back(const T& val) {
        if (num_items == _capacity) 
            resize(_capacity == 0 ? 1 : _capacity * 2);
        data[num_items++] = val;
    }

    void pop_back() {
        if(num_items == 0) 
            throw std::out_of_range("pop_back() called on an empty vector\n");
        --num_items;
    }

    T& operator[](size_t index) {
        if (index >= num_items)
            throw std::out_of_range("index out of range\n");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= num_items)
            throw std::out_of_range("index out of range\n");
        return data[index];
    }

    size_t size() const { return num_items; }
    size_t capacity() const { return _capacity; }
    void clear() { num_items = 0; }
    bool isEmpty() { return num_items == 0; }
};
