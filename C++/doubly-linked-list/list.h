#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstddef>   // para std::size_t
#include <iterator>  // para std::bidirectional_iterator_tag
#include <utility>   // para std::swap

template <typename T>
class list {
private:
    struct node {
        T value;
        node * prev;
        node * next;
    };

    node m_front_and_back;

public:
    list() {
        m_front_and_back.prev = &m_front_and_back;
        m_front_and_back.next = &m_front_and_back;
    }

    list(list & x) : list() {
        assign(x.begin(), x.end());
    }

    template <typename Iter>
    list(Iter first, Iter last) : list() {
        assign(first, last);
    }

    ~list() {
        clear();
    }

    friend
    void swap(list & x, list & y) {
        using std::swap;
        swap(x.m_front_and_back, y.m_front_and_back);
    }

    list & operator=(list y) {
        swap(*this, y);
        return *this;
    }

    void clear() {
        auto p = begin();
        while (p != end()) {
            p = erase(p);
        }
    }

    template <typename Iter>
    void assign(Iter first, Iter last) {
        clear();
        while (first != last) {
            push_back(*first);
            ++first;
        }
    }

    std::size_t length() {
        std::size_t n = 0;
        auto p = begin();
        while (p != end()) {
            ++p;
            ++n;
        }
        return n;
    }

    bool empty() {
        return m_front_and_back.next == &m_front_and_back;
    }

    T & front() {
        // Precondición: la lista no está vacía.
        return m_front_and_back.next->value;
    }

    T & back() {
        // Precondición: la lista no está vacía.
        return m_front_and_back.prev->value;
    }

    class iterator {
    public:
        friend class list;

        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using difference_type = std::size_t;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator(node * ptr = nullptr) {
            m_ptr = ptr;
        }

        T & operator*() {
            return m_ptr->value;
        }

        T * operator->() {
            return &operator*();
        }

        iterator & operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            operator++();
            return tmp;
        }

        iterator & operator--() {
            m_ptr = m_ptr->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            operator--();
            return tmp;
        }

        friend
        bool operator==(const iterator & x, const iterator & y) {
            return x.m_ptr == y.m_ptr;
        }

        friend
        bool operator!=(const iterator & x, const iterator & y) {
            return !(x == y);
        }

    private:
        node * m_ptr;
    };

    iterator begin() {
        return iterator(m_front_and_back.next);
    }

    iterator end() {
        return iterator(&m_front_and_back);
    }

    void push_front(const T & value) {
        insert(begin(), value);
    }

    void push_back(const T & value) {
        insert(end(), value);
    }

    void pop_front() {
        // Precondición: la lista no está vacía
        erase(begin());
    }

    void pop_back() {
        // Precondición: la lista no está vacía
        erase(--end());
    }

    iterator insert(iterator pos, const T & value) {
        node * current = pos.m_ptr;
        node * new_node =  new node;
        new_node->value = value;
        new_node->prev = current->prev;
        new_node->next = current;
        new_node->prev->next = new_node;
        new_node->next->prev = new_node;
        return iterator(new_node);
    }

    iterator erase(iterator pos) {
        node * current = pos.m_ptr;
        ++pos;
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        return pos;
    }

    void splice(iterator pos, iterator first, iterator last) {
        if (first == last) {
            return;
        }

        node * current = pos.m_ptr;
        node * other_first = first.m_ptr;
        node * other_last = last.m_ptr->prev;

        // Desengancha los nodos de la otra lista
        other_first->prev->next = other_last->next;
        other_last->next->prev = other_first->prev;

        // Engancha esos nodos en la posición indicada en esta lista
        current->prev->next = other_first;
        other_first->prev = current->prev;
        current->prev = other_last;
        other_last->next = current;
    }

    void splice(iterator pos, iterator other_pos) {
        splice(pos, other_pos, std::next(other_pos));
    }

    void splice(iterator pos, list& other) {
        splice(pos, other.begin(), other.end());
    }

    void merge(list & other) {
        auto this_iter = begin();
        auto other_iter = other.begin();

        while (other_iter != other.end()) {
            if (this_iter != end() and *this_iter <= *other_iter) {
                ++this_iter;
            } else {
                splice(this_iter, other_iter++);
            }
        }
    }

};

#endif // DOUBLY_LINKED_LIST_H
