#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <cstddef>    // Para std::size_t
#include <functional> // Para std::function
#include <iterator>   // Para std::forward_iterator_tag
#include <stack>      // Para std::stack
#include <utility>    // Para std::pair y std::swap

/************************************************************************************/
/*** Funcionalidad adicional no estrictamente necesaria para implementar el árbol ***/
/************************************************************************************/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

/************************************************************************************/
/** Árbol Binario de Búsqueda implementado en forma recursiva con iterador liviano **/
/************************************************************************************/

template <typename T>
class tree {
private:
    struct node {
        T value;
        node * left;
        node * right;
        node * parent;

        node * find_minimum() {
            if (left != nullptr)
                return left->find_minimum();
            return this;
        }

        node * find_maximum() {
            if (right != nullptr)
                return right->find_maximum();
            return this;
        }
    };

    node * m_root;

public:

    /************************************************************************/
    /************* CONSTRUCTORES, DESTRUCTOR, ASIGNACIÓN Y SWAP *************/
    /************************************************************************/

    tree() {
        m_root = nullptr;
    }

    tree(tree & x) {
        m_root = nullptr;
        if (x.m_root != nullptr) {
            copy_nodes(x.m_root, m_root, nullptr);
        }
    }

    ~tree() {
        clear();
    }

    tree & operator=(tree x) {
        swap(*this, x);
        return *this;
    }

    friend
    void swap(tree & x, tree & y) {
        using namespace std;
        swap(x.m_root, y.m_root);
    }

    /************************************************************************/
    /************ ITERADOR LIVIANO QUE RECORRE AL ÁRBOL EN ORDEN ************/
    /************************************************************************/

    class iterator {
    private:
        node * m_current;

    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using difference_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(node * current = nullptr) {
            m_current = current;
        }

        reference operator*() {
            // Precondición: m_current != nullptr
            return m_current->value;
        }

        pointer operator->() {
            return &operator*();
        }

        friend
        bool operator==(const iterator & x, const iterator & y) {
            return x.m_current == y.m_current;
        }

        friend
        bool operator!=(const iterator & x, const iterator & y) {
            return !(x == y);
        }

        iterator & operator++() {
            // Precondición: m_current != nullptr
            if (m_current->right != nullptr) {
                m_current = m_current->right->find_minimum();
            } else {
                node * prev;
                do {
                    prev = m_current;
                    m_current = m_current->parent;
                } while (m_current != nullptr and m_current->right == prev);
            }
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }
    };

    iterator begin() {
        if (empty()) {
            return end();
        }
        return iterator(m_root->find_minimum());
    }

    iterator end() {
        return iterator();
    }

    /************************************************************************/
    /******* MÉTODOS QUE PERMITEN CONSULTAR AL ÁRBOL SIN MODIFICARLO ********/
    /************************************************************************/

    bool empty() {
        return m_root == nullptr;
    }

    friend
    bool operator==(tree & x, tree & y) {
        auto i = x.begin();
        auto j = y.begin();
        while (i != x.end() && j != y.end()) {
            if (*i != *j)
                return false;
            ++i;
            ++j;
        }
        return i == x.end() && j == y.end();
    }

    friend
    bool operator!=(tree & x, tree & y) {
        return !(x == y);
    }

    iterator find(const T & value) {
        return do_find(m_root, value);
    }

    bool contains(const T & value) {
        return find(value) != end();
    }

    iterator minimum() {
        if (m_root == nullptr)
            return end();
        return m_root->find_minimum();
    }

    iterator maximum() {
        if (m_root == nullptr)
            return end();
        return m_root->find_maximum();
    }

    void each(std::function<void(const T &)> func) {
        do_each(m_root, func);
    }

    /************************************************************************/
    /******************* MÉTODOS QUE MODIFICAN AL ÁRBOL *********************/
    /************************************************************************/

    iterator insert(const T & value) {
        return do_insert(m_root, nullptr, value);
    }

    std::pair<bool, iterator> erase(const T & value) {
        return do_erase(m_root, value);
    }

    void clear() {
        do_clear(m_root);
        m_root = nullptr;
    }

private:

    /************************************************************************/
    /************** MÉTODOS AUXILIARES PARA LA IMPLEMENTACIÓN ***************/
    /************************************************************************/

    void copy_nodes(const node * other_node, node * & this_node, node * parent) {
        if (other_node != nullptr) {
            this_node = new node { other_node->value, nullptr, nullptr, parent };
            copy_nodes(other_node->left, this_node->left, this_node);
            copy_nodes(other_node->right, this_node->right, this_node);
        }
    }

    iterator do_find(node * current, const T & value) {
        if (current == nullptr) {
            return end();
        } else if (value < current->value ) {
            return do_find(current->left, value);
        } else if (value > current->value) {
            return do_find(current->right, value);
        } else {
            return { current };
        }
    }

    void do_each(const node * current, std::function<void(const T &)> func) {
        if (current != nullptr) {
            do_each(current->left, func);
            func(current->value);
            do_each(current->right, func);
        }
    }

    iterator do_insert(node * & current, node * parent, const T & value) {
        if (current == nullptr) {
            current = new node { value, nullptr, nullptr, parent };
        } else if (value < current->value) {
            return do_insert(current->left, current, value);
        } else if (value > current->value) {
            return do_insert(current->right, current, value);
        }
        return { current };
    }

    std::pair<bool, iterator> do_erase(node * & current, const T & value) {
        if (current == nullptr)
            return { false, end() };

        if (value < current->value) {
            return do_erase(current->left, value);
        } else if (value > current->value) {
            return do_erase(current->right, value);
        }

        iterator next = ++iterator(current);
        erase_node(current);
        return { true, next };
    }

    void erase_node(node * & n) {
        node * removed = n;
        if (n->left == nullptr) {
            n = n->right;
        } else if (n->right == nullptr) {
            n = n->left;
        } else {
            move_maximum_to(n, n->left);
        }
        assign_parent(n, removed->parent);
        delete removed;
    }

    void move_maximum_to(node * & root, node * & current) {
        if (current->right == nullptr) {
            node * max = current;
            assign_parent(max->left, max->parent);
            current = max->left;
            max->left = root->left;
            assign_parent(max->left, max);
            max->right = root->right;
            assign_parent(max->right, max);
            root = max;
        } else {
            move_maximum_to(root, current->right);
        }
    }

    void assign_parent(node * & n, node * parent) {
        if (n != nullptr) {
            n->parent = parent;
        }
    }

    void do_clear(node * & current) {
        if (current != nullptr) {
            do_clear(current->left);
            do_clear(current->right);
            delete current;
        }
    }

    /************************************************************************/
    /************** AQUÍ NO HAY NADA PARA VER... ¡CIRCULE! :-P **************/
    /************************************************************************/

    using nodes_placement = std::vector<std::tuple<int, int, const node *>>;

    // Calcula la posición de los nodos para poder mostrarlos en pantalla
    void calculate_nodes_placement(const node * current, int & x, int h, nodes_placement & placements) const {
        if (current != nullptr) {
            calculate_nodes_placement(current->left, x, h+1, placements);
            placements.emplace_back(h, x++, current);
            calculate_nodes_placement(current->right, x, h+1, placements);
        }
    }

public:

    // Éste método genera una representación "gráfica" del árbol usando caracteres ASCII
    std::string str() const {
        int count = 0;
        nodes_placement placements;
        calculate_nodes_placement(m_root, count, 0, placements);

        const int node_value_size = 3;
        std::vector<std::string> lines;
        int prev_level = -1;
        for (const auto & placement: placements) {
            int level;
            int pos;
            const node * the_node;
            std::tie(level, pos, the_node) = placement;

            while (int(lines.size()) <= 2 * level) {
                lines.emplace_back();
            }

            std::ostringstream s;
            int i = 2 * level;
            s << std::setw(pos * node_value_size - lines[i].size()) << "";
            s << std::setw(node_value_size) << the_node->value;
            lines[i] += s.str();

            if (prev_level != -1) {
                char c;
                if (prev_level < level) {
                    i = 2 * prev_level + 1;
                    c = '\\';
                } else {
                    i = 2 * level + 1;
                    c = '/';
                }

                s.str("");
                s << std::setw(pos * node_value_size - lines[i].size()) << "";
                s << std::setw(node_value_size / 2) << c;
                lines[i] += s.str();
            }
            prev_level = level;
        }

        std::string result;
        for (const auto & line: lines) {
            result += line;
            result += '\n';
        }
        return result;
    }
};

#endif // BINARY_SEARCH_TREE_H
