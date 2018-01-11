#ifndef STATIC_STACK_H
#define STATIC_STACK_H

#include <algorithm>   // Para std::copy_n
#include <cstddef>     // Para std::size_t
#include <utility>     // Para std::swap
#include <iostream>    // Para std::cout y std::endl

template <typename T, size_t CAPACITY>
class stack {
public:
    using size_t = std::size_t;

    stack() {
        m_length = 0;
    }

    stack(const stack & x) {
        m_length = x.m_length;
        std::copy_n(x.m_data, m_length, m_data);
    }

    friend
    void swap(stack & x, stack & y) {
        using std::swap;
        swap(x.m_data, y.m_data);
        swap(x.m_length, y.m_length);
    }

    size_t size() const {
        return m_length;
    }

    bool empty() const {
        return size() == 0;
    }

    bool full() const {
        return size() == CAPACITY;
    }

    T & top() {
        // Precondición: La pila no está vacía.
        return m_data[m_length - 1];
    }

    void push(const T & x) {
        m_data[m_length] = x;
        ++m_length;
    }

    void pop() {
        // Precondición: La pila no está vacía.
        --m_length;
    }

    void clear() {
        m_length = 0;
    }

    friend
    bool operator==(const stack & x, const stack & y) {
        if (x.m_length != y.m_length) {
            return false;
        }
        std::size_t px = 0;
        std::size_t py = 0;
        std::size_t n = x.m_length; // da lo mismo si es y.m_length
        while (n > 0) {
            if (x.m_data[px] != y.m_data[py]) {
                return false;
            }
            ++px;
            ++py;
            --n;
        }
        return true;
    }

    friend
    bool operator!=(const stack & x, const stack & y) {
        return !(x == y);
    }

    friend
    void debug(stack & x) {
        using std::cout;
        using std::endl;
        cout << "DEBUG: { ";
        for (auto & e : x.m_data) {
            cout << e << " ";
        }
        cout << "}" << std::boolalpha
             << " - m_length: " << x.m_length
             << " - size(): " << x.size()
             << " - empty(): " << x.empty();
        if (!x.empty()) {
            cout << " - top(): " << x.top();
        }
        cout << endl;
    }

private:
    size_t m_length;

    T m_data[CAPACITY];
};

#endif // STATIC_STACK_H
