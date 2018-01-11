#ifndef DYNAMIC_STACK_H
#define DYNAMIC_STACK_H

#include <cstddef>     // Para std::size_t
#include <utility>     // Para std::swap
#include <iostream>    // Para std::cout y std::endl

template <typename T>
class stack {
public:
    using size_t = std::size_t;

    stack() {
        m_top = nullptr;
        m_length = 0;
    }

    stack(const stack<T> & x) {
        node * n = m_top = nullptr;
        node * px = x.m_top;
        node ** prev_ptr = &m_top;
        while (px != nullptr) {
            n = new node { px->value, nullptr };
            *prev_ptr = n;
            prev_ptr = &(*prev_ptr)->prev;
            px = px->prev;
        }
        m_length = x.m_length;
    }

    ~stack() {
        clear();
    }

    friend
    void swap(stack<T> & x, stack<T> & y) {
        using std::swap;
        swap(x.m_length, y.m_length);
        swap(x.m_top, y.m_top);
    }

    stack<T> & operator=(stack<T> x) {
        swap(*this, x);
        return *this;
    }

    size_t size() const {
        return m_length;
    }

    bool empty() const {
        return size() == 0;
    }

    T & top() {
        // Precondición: La pila no está vacía.
        return m_top->value;
    }

    void push(const T & x) {
        m_top = new node { x, m_top };
        ++m_length;
    }

    void pop() {
        // Precondición: La pila no está vacía.
        --m_length;
        auto node = m_top;
        m_top = m_top->prev;
        delete node;
    }

    void clear() {
        node * ptr = m_top;
        while (ptr != nullptr) {
            node * node = ptr;
            ptr = node->prev;
            delete node;
        }
        m_top = nullptr;
        m_length = 0;
    }

    friend
    bool operator==(const stack<T> & x, const stack<T> & y) {
        return !(x != y);
    }

    friend
    bool operator!=(const stack<T> & x, const stack<T> & y) {
        if (x.m_length != y.m_length) {
            return true;
        }

        node * px = x.m_top;
        node * py = y.m_top;
        while (px != nullptr && py != nullptr) {
            if (px->value != py->value) {
                return true;
            }
            px = px->prev;
            py = py->prev;
        }

        return px != nullptr || py != nullptr;
    }

    friend
    void debug(stack & x) {
        using std::cout;
        using std::endl;

        cout << "DEBUG: { ";
        auto px = x.m_top;
        while (px != nullptr) {
            cout << px->value << " ";
            px = px->prev;
        }
        cout << "}" << std::boolalpha
             << " - size(): " << x.size()
             << " - empty(): " << x.empty();
        if (!x.empty()) {
            cout << " - top(): " << x.top();
        }
        cout << endl;
    }

private:
    struct node {
        T value;
        node * prev;
    };

    node * m_top;
    size_t m_length;
};

#endif // DYNAMIC_STACK_H

