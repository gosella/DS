#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <cstddef>   // para std::size_t
#include <iterator>  // para std::bidirectional_iterator_tag
#include <utility>   // para std::swap

template <typename T>
class forward_list {
private:
    struct node {
        node * next;
        T value;
    };

    node m_front;
    std::size_t m_length;

public:
    forward_list() {
        m_front.next = nullptr;
        m_length = 0;
    }

    forward_list(forward_list & x) : forward_list() {
        assign(x.begin(), x.end());
    }

    template <typename Iter>
    forward_list(Iter first, Iter last) : forward_list() {
        assign(first, last);
    }

    ~forward_list() {
        clear();
    }

    friend
    void swap(forward_list & x, forward_list & y) {
        using std::swap;
        swap(x.m_front, y.m_front);
        swap(x.m_length, y.m_length);
    }

    forward_list & operator=(forward_list x) {
        swap(*this, x);
        return *this;
    }

    void clear() {
        node * ptr = m_front.next;
        m_front.next = nullptr;
        m_length = 0;
        while (ptr != nullptr) {
            node * current = ptr;
            ptr = current->next;
            delete current;
        }
    }

    template <typename Iter>
    void assign(Iter first, Iter last) {
        clear();
        node ** ptr = &m_front.next;
        while (first != last) {
            *ptr = new node { nullptr, *first };
            ptr = &(*ptr)->next;
            ++m_length;
            ++first;
        }
    }

    std::size_t length() {
        return m_length;
    }

    bool empty() {
        return m_length == 0;
    }

    T & front() {
        // Precondición: !empty()
        return m_front.next->value;
    }

    class iterator {
    public:
        friend class forward_list;

        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using difference_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(node * ptr = nullptr) {
            m_ptr = ptr;
        }

        T & operator*() {
            return m_ptr->value; // retorna el valor del nodo actualmente iterado
        }

        T * operator->() {
            return &operator*(); // retorna la dirección del valor en el nodo al que apunta el iterador
        }

        iterator & operator++() {
            m_ptr = m_ptr->next; // avanza al siguiente nodo de la lista
            return *this;        // retorna al iterador avanzado
        }

        iterator operator++(int) {
            iterator it { *this }; // copia el iterador actual
            operator++();          // avanza el iterador actual
            return it;             // retorna el iterador copiado
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

    iterator before_begin() {
        return iterator(&m_front);
    }

    iterator begin() {
        return iterator(m_front.next);
    }

    iterator end() {
        return iterator(nullptr);
    }

    void push_front(const T & value) {
        m_front.next = new node { m_front.next, value };
        ++m_length;
    }

    T pop_front() {
        // Precondición: !empty()
        node * node = m_front.next;
        m_front.next = node->next;
        delete node;
        --m_length;
    }

    iterator insert_after(iterator pos, const T & value ) {
        node * ptr = pos.m_ptr;
        ptr->next = new node { ptr->next, value };
        ++m_length;
        return { ptr->next };
    }

    iterator erase_after(iterator pos) {
        node * ptr = pos.m_ptr;
        node * node = ptr->next;
        ptr->next = node->next;
        delete node;
        --m_length;
        return { ptr->next };
    }

    void splice_after(iterator pos, forward_list& other) {
        if (other.empty()) {
            return;
        }
        node * last = &other.m_front;
        while (last->next != nullptr) {
            last = last->next;
            ++m_length;
        }
        node * node = pos.m_ptr;
        last->next = node->next;
        node->next = other.m_front.next;
        other.m_front.next = nullptr;
        other.m_length = 0;
    }

    void merge(forward_list & other) {
        node ** this_ptr = &m_front.next;
        node ** other_ptr = &other.m_front.next;

        while (*this_ptr != nullptr and *other_ptr != nullptr) {
            node * this_node = *this_ptr;
            node * other_node = *other_ptr;

            if (this_node->value <= other_node->value) {
                this_ptr = &this_node->next;
            } else {
                *other_ptr = other_node->next;
                *this_ptr = other_node;
                other_node->next = this_node;
                ++m_length;
            }
        }

        if (*other_ptr != nullptr) {
            *this_ptr = *other_ptr;
            node * other_node = *other_ptr;
            while (other_node != nullptr) {
                other_node = other_node->next;
                ++m_length;
            }
        }

        other.m_front.next = nullptr;
        other.m_length = 0;
    }
};

#endif // SINGLY_LINKED_LIST_H
