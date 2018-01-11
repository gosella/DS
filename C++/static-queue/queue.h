#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include <algorithm>   // Para std::copy_n
#include <cstddef>     // Para std::size_t
#include <utility>     // Para std::swap
#include <iostream>    // Para std::cout y std::endl

template <typename T, size_t CAPACITY>
class queue {
public:
    using size_t = std::size_t;

    queue() {
        m_front = m_length = 0;
    }

    queue(const queue & x) {
        auto f = m_front = x.m_front;
        auto n = m_length = x.m_length;
        auto r = CAPACITY - f;
        if (n > r) {
            std::copy_n(x.m_data + f, r, m_data + f);
            n -= r;
            f = 0;
        }
        std::copy_n(x.m_data + f, n, m_data + f);
    }

    friend
    void swap(queue & x, queue & y) {
        using std::swap;
        swap(x.m_data, y.m_data);
        swap(x.m_front, y.m_front);
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

    T & front() {
        // Precondición: La cola no está vacía.
        return m_data[m_front];
    }

    T & back() {
        // Precondición: La cola no está vacía.
        auto i = m_front + m_length - 1;
        if (i >= CAPACITY) {
            i -= CAPACITY;
        }
        return m_data[i];
    }

    void enqueue(const T & x) {
        auto i = m_front + m_length;
        if (i >= CAPACITY) {
            i -= CAPACITY;
        }
        m_data[i] = x;
        ++m_length;
    }

    void dequeue() {
        // Precondición: La cola no está vacía.
        --m_length;
        ++m_front;
        if (m_front == CAPACITY) {
            m_front = 0;
        }
    }

    void clear() {
        m_front = m_length = 0;
    }

    friend
    bool operator==(const queue & x, const queue & y) {
        if (x.m_length != y.m_length) {
            return false;
        }
        auto px = x.m_front;
        auto py = y.m_front;
        auto n = x.m_length;  // da lo mismo si es y.m_length
        while (n > 0) {
            if (x.m_data[px] != y.m_data[py]) {
                return false;
            }
            ++px;
            if (px == CAPACITY) {
                px = 0;
            }
            ++py;
            if (py == CAPACITY) {
                py = 0;
            }
            --n;
        }
        return true;
    }

    friend
    bool operator!=(const queue & x, const queue & y) {
        return !(x == y);
    }

    friend
    void debug(queue & x) {
        using std::cout;
        using std::endl;
        cout << "DEBUG: { ";
        for (auto & e : x.m_data) {
            cout << e << " ";
        }
        cout << "}" << std::boolalpha
             << " - m_front: " << x.m_front
             << " - size(): " << x.size()
             << " - empty(): " << x.empty();
        if (!x.empty()) {
            cout << " - front(): " << x.front()
                 << " - back(): " << x.back();
        }
        cout << endl;
    }

private:
    size_t m_front;
    size_t m_length;

    T m_data[CAPACITY];
};

#endif // STATIC_QUEUE_H

