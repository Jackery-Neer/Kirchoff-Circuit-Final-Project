#include "Stack.h"

template <typename T>
void Stack<T>::pop() {
    if (isEmpty())
        throw std::out_of_range("pop() called on empty Stack");
    data.pop_back();
}

template <typename T>
void Stack<T>::clear() {
    while (!isEmpty()) 
        pop();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& s) {
    for (size_t i = 0; i < s.data.size(); i++) {
        os << s.data[i] << " "
    }
    return os;
}
