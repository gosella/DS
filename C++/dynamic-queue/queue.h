#ifndef DYNAMIC_QUEUE_H
#define DYNAMIC_QUEUE_H

#include <cstddef>     // Para std::size_t
#include <utility>     // Para std::swap
#include <iostream>    // Para std::cout y std::endl

template <typename T>
class queue {
public:
    using size_t = std::size_t;

    queue() {
        m_front = m_back = nullptr;
        m_length = 0;
    }

    queue(const queue<T> & x) {
        node * n = m_front = nullptr;
        node * px = x.m_front;
        node ** prev_ptr = &m_front;
        while (px != nullptr) {
            n = new node { px->value, nullptr };
            *prev_ptr = n;
            prev_ptr = &(*prev_ptr)->prev;
            px = px->prev;
        }
        m_back = n;
        m_length = x.m_length;
    }

    ~queue() {
        clear();
    }

    friend
    void swap(queue<T> & x, queue<T> & y) {
        using std::swap;
        swap(x.m_length, y.m_length);
        swap(x.m_front, y.m_front);
        swap(x.m_back, y.m_back);
    }

    queue<T> & operator=(queue<T> x) {
        swap(*this, x);
        return *this;
    }

    size_t size() const {
        return m_length;
    }

    bool empty() const {
        return size() == 0;
    }

    T & front() {
        // Precondición: La cola no está vacía.
        return m_front->value;
    }

    T & back() {
        // Precondición: La cola no está vacía.
        return m_back->value;
    }

    void enqueue(const T & x) {
        auto new_node = new node { x, nullptr };
        if (m_back != nullptr) {
            m_back->prev = new_node;
        }
        m_back = new_node;
        if (m_front == nullptr) {
            m_front = m_back;
        }
        ++m_length;
    }

    void dequeue() {
        // Precondición: La cola no está vacía.
        --m_length;
        auto node = m_front;
        m_front = m_front->prev;
        if (m_front == nullptr) {
            m_back = nullptr;
        }
        delete node;
    }

    void clear() {
        node * ptr = m_front;
        while (ptr != nullptr) {
            node * node = ptr;
            ptr = node->prev;
            delete node;
        }
        m_front = m_back = nullptr;
        m_length = 0;
    }

    friend
    bool operator==(const queue<T> & x, const queue<T> & y) {
        return !(x != y);
    }

    friend
    bool operator!=(const queue<T> & x, const queue<T> & y) {
        if (x.m_length != y.m_length) {
            return true;
        }

        node * px = x.m_front;
        node * py = y.m_front;
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
    void debug(queue & x) {
        using std::cout;
        using std::endl;

        cout << "DEBUG: { ";
        auto px = x.m_front;
        while (px != nullptr) {
            cout << px->value << " ";
            px = px->prev;
        }
        cout << "}" << std::boolalpha
             << " - size(): " << x.size()
             << " - empty(): " << x.empty();
        if (!x.empty()) {
            cout << " - front(): " << x.front()
                 << " - back(): " << x.back();
        }
        cout << endl;
    }

private:
    struct node {
        T value;
        node * prev;
    };

    node * m_front;
    node * m_back;
    size_t m_length;
};

#endif // DYNAMIC_QUEUE_H

