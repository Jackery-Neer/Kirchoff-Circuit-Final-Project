#pragma once
#include <cstddef>   
#include <stdexcept> 
#include <utility>   
#include <iostream>


template <typename T>
class Vector {
private:
    T* data;
    size_t num_items;
    size_t _capacity;
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        size_t elem_to_copy = (new_capacity < num_items) ? new_capacity : num_items;
        for (size_t i = 0; i < elem_to_copy; i++) 
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
        if (num_items > new_capacity)
            num_items = new_capacity;
    }
public:
    //Default Constructor
    Vector() : data(nullptr), num_items(0), _capacity(0) {}

    //Constructor
    Vector(size_t _size) : data(nullptr), num_items(_size), _capacity(_size) {
        data = new T[_capacity];
        for (size_t i = 0; i < num_items; i++)
            data[i] = T();
    }

    Vector(size_t _size, const T& val) : data(new T[_size]), num_items(_size), _capacity(_size) {
        for (size_t i = 0; i < _size; i++)
            data[i] = val;
    }

    //Copy Constructor
    Vector(const Vector& other) : data(new T[other._capacity]), num_items(other.num_items), _capacity(other._capacity)  {
        for (size_t i = 0; i < num_items; i++) 
            data[i] = other.data[i];
    }

    //Copy Assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            _capacity = other._capacity;
            num_items = other.num_items;
            data = new T[_capacity];
            for (size_t i = 0; i < num_items; i++)
                data[i] = other.data[i];
        }
        return *this;
    }

    //Move Constructor
    Vector(Vector&& other) noexcept : data(other.data), num_items(other.num_items), _capacity(other._capacity) {
        other.data = nullptr;
        other.num_items = 0;
        other._capacity = 0;
    }

    //Move Assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            num_items = other.num_items;
            _capacity = other._capacity;
            other.data = nullptr;
            other.num_items = 0;
            other._capacity = 0;
        }
        return *this;
    }

    //Destructor
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

    void erase(size_t index) {
        if (index >= num_items)
            throw std::out_of_range("erase() index out of range\n");

        for (size_t i = index; i < num_items - 1; i++) {
            data[i] = data[i + 1];  // Shift elements left
        }

        --num_items;
    }

    void erase(T* it) {
        size_t index = static_cast<size_t>(it - data);  // compute index from pointer
        erase(index);
    }

    T& back() {
        if (num_items == 0)
            throw std::out_of_range("back() called on empty Vector\n");
        return data[num_items - 1];
    }

    const T& back() const {
        if (num_items == 0)
            throw std::out_of_range("back() called on empty Vector\n");
        return data[num_items - 1];
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

    T& at(size_t index) {
        if (index >= num_items)
            throw std::out_of_range("index out of range\n");
        return data[index];
    }

    const T& at(size_t index) const{
        if (index >= num_items)
            throw std::out_of_range("index out of range\n");
        return data[index];
    }

    size_t size() const { return num_items; }
    size_t capacity() const { return _capacity; }
    void clear() { num_items = 0; }
    bool isEmpty() { return num_items == 0; }
    T* begin() { return data; }
    T* end() { return data + num_items; }
    const T* begin() const { return data; }
    const T* end() const { return data + num_items; }
};
