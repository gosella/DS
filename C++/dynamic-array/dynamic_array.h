#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <algorithm>  // Para std::copy_n, std::equal
#include <cstddef>    // Para std::size_t
#include <iterator>   // Para std::begin, std::end, std::random_access_iterator_tag

template <typename T>
class dynamic_array {
public:
    using size_t = std::size_t;

    dynamic_array(size_t count = 0) {
        if (count > 0) {
            m_data = new T[count];
        } else {
            m_data = nullptr;
        }
        m_size = m_capacity = count;
    }

    dynamic_array(dynamic_array & x) {
        m_capacity = x.m_capacity;
        m_size = x.m_size;
        if (m_capacity > 0) {
            m_data = new T[m_capacity];
            std::copy_n(x.begin(), x.size(), begin());
        } else {
            m_data = nullptr;
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > m_capacity) {
            T * new_data = new T[new_capacity];
            std::copy_n(begin(), size(), &new_data[0]);
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }
    }

    void resize(size_t new_size) {
        if (new_size > capacity()) {
            reserve(new_size);
            auto n = m_size;
            while(n < new_size) {
                m_data[n] = T {};
                ++n;
            }
        }
        m_size = new_size;
    }

    ~dynamic_array() {
        delete[] m_data;
    }

    friend
    void swap(dynamic_array & x, dynamic_array & y) {
        using std::swap;
        swap(x.m_data, y.m_data);
        swap(x.m_size, y.m_size);
        swap(x.m_capacity, y.m_capacity);
    }

    dynamic_array & operator =(dynamic_array x) {
        swap(*this, x);
        return *this;
    }

    friend
    bool operator ==(const dynamic_array & x, const dynamic_array & y) {
        if (x.size() != y.size()) {
            return false;
        }
        auto px = std::begin(x);
        auto py = std::begin(y);
        while (px != std::end(x)) { // Son del mismo tamaño, no necesito ver si py != std::end(y)
            if (*px != *py) {
                return false;
            }
            ++px;
            ++py;
        }
        return true;

        // Equivalente a lo anterior:
//        return std::equal(std::begin(x), std::end(x),
//                          std::begin(y), std::end(y));
    }

    friend
    bool operator !=(const dynamic_array & x, const dynamic_array & y) {
        return !(x == y);
    }

    /************************************************************************/

    size_t capacity() {
        return m_capacity;
    }

    size_t size() {
        return m_size;
    }

    bool empty() {
        return size() == 0;
    }

    T & operator [](size_t index) {
        // Precondición: !empty()
        return m_data[index];
    }

    T & front() {
        // Precondición: !empty()
        return m_data[0];
    }

    T & back() {
        // Precondición: !empty()
        return m_data[size() - 1];
    }

    /************************************************************************/

    class iterator {
        friend class dynamic_array;
    public:
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        iterator(T * current = nullptr) {
            m_current = current;
        }

        reference operator *() {
            return *m_current;
        }

        pointer operator ->() {
            return &operator *();
        }

        friend
        bool operator ==(const iterator & x, const iterator & y) {
            return x.m_current == y.m_current;
        }

        friend
        bool operator !=(const iterator & x, const iterator & y) {
            return !(x == y);
        }

        iterator & operator ++() {
            ++m_current;
            return *this;
        }

        iterator operator ++(int) {
            auto tmp = *this;
            operator ++();
            return tmp;
        }

        iterator & operator --() {
            --m_current;
            return *this;
        }

        iterator operator --(int) {
            auto tmp = *this;
            operator --();
            return tmp;
        }

        iterator & operator +=(size_t n) {
            m_current += n;
            return *this;
        }

        friend
        iterator operator +(iterator x, size_t n) {
            return x += n;
        }

        friend
        iterator operator +(size_t n, iterator x) {
            return x += n;
        }

        iterator & operator -=(size_t n) {
            m_current -= n;
            return *this;
        }

        friend
        iterator operator -(iterator x, size_t n) {
            return x -= n;
        }

        friend
        iterator operator -(size_t n, iterator x) {
            return x -= n;
        }

        friend
        difference_type operator -(iterator x, iterator y) {
            return x.m_current - y.m_current;
        }

        reference operator [](size_t n) {
            return *(*this + n);
        }

        friend
        bool operator <(iterator x, iterator y) {
            return x.m_current < y.m_current;
        }

        friend
        bool operator >(iterator x, iterator y) {
            return y < x;
        }

        friend
        bool operator <=(iterator x, iterator y) {
            return !(y < x);
        }

        friend
        bool operator >=(iterator x, iterator y) {
            return !(x < y);
        }

    private:
        T * m_current;
    };

    iterator begin() {
        return iterator(m_data);
    }

    iterator end() {
        return iterator(m_data + m_size);
    }

    /************************************************************************/

    void clear() {
        m_size = 0;
    }

    void push_back(const T & value) {
        if (m_size == capacity()) {
            enlarge();
        }
        m_data[m_size] = value;
        ++m_size;
    }

    void pop_back() {
        // Precondición: !empty()
        --m_size;
    }

    iterator insert(iterator pos, const T& value) {
        size_t n = pos - begin();
        if (size() == capacity()) {
            enlarge();
            pos = begin() + n;
        }
        auto p = end();
        while(p != pos) {
            *p-- = *p;
        }
        *p = value;
        ++m_size;
        return pos;
    }

    iterator erase(iterator pos) {
        // Precondición: !empty() && pos != end()
        auto p = pos;
        auto q = pos + 1;
        while(q != end()) {
            *p = *q;
            ++p;
            ++q;
        }
        --m_size;
        return pos;
    }

private:
    T * m_data;
    size_t m_size;
    size_t m_capacity;

    void enlarge() {
        if (capacity() == 0) {
            reserve(8);
        } else {
            reserve(2 * capacity());
        }
    }
};


#endif // DYNAMIC_ARRAY_H
