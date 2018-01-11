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
/** Árbol Binario de Búsqueda implementado en forma iterativa con iterador liviano **/
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
            // Pre-condición: this != nullptr
            node * current = this;
            while (current->left != nullptr) {
                current = current->left;
            }
            return current;
        }

        node * find_maximum() {
            // Pre-condición: this != nullptr
            node * current = this;
            while (current->right != nullptr) {
                current = current->right;
            }
            return current;
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
            copy_nodes(x.m_root);
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
        node * current = m_root;
        while (current != nullptr) {
            if (value < current->value) {
                current = current->left;
            } else if (value > current->value) {
                current = current->right;
            } else {
                return iterator(current);
            }
        }
        return end();
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
        for (const T & x : *this)
            func(x);
    }

    /************************************************************************/
    /******************* MÉTODOS QUE MODIFICAN AL ÁRBOL *********************/
    /************************************************************************/

    iterator insert(const T & value) {
        node ** ptr = &m_root;
        node * parent = nullptr;
        while (*ptr != nullptr) {
            parent = *ptr;
            if (value < (*ptr)->value) {
                ptr = &(*ptr)->left;
            } else if (value > (*ptr)->value) {
                ptr = &(*ptr)->right;
            } else {
                return { *ptr };
            }
        }
        *ptr = new node { value, nullptr, nullptr, parent };
        return { *ptr };
    }

    std::pair<bool, iterator> erase(const T & value) {
        node ** ptr = &m_root;
        while (*ptr != nullptr) {
            if (value < (*ptr)->value) {
                ptr = &(*ptr)->left;
            } else if (value > (*ptr)->value) {
                ptr = &(*ptr)->right;
            } else {
                iterator next = ++iterator(*ptr);
                erase_node(*ptr);
                return { true, next };
            }
        }
        return { false, end() };
    }

    void clear() {
        if (m_root == nullptr) {
            return;
        }
        std::stack<node *> nodes;
        nodes.push(m_root);
        m_root = nullptr;
        while (!nodes.empty()) {
            node * current = nodes.top();
            nodes.pop();
            if (current->left != nullptr) {
                nodes.push(current->left);
            }
            if (current->right != nullptr) {
                nodes.push(current->right);
            }
            delete current;
        }
    }

private:

    /************************************************************************/
    /************** MÉTODOS AUXILIARES PARA LA IMPLEMENTACIÓN ***************/
    /************************************************************************/

    void copy_nodes(node * root) {
        struct info {
            node * from;
            node * & to;
            node * parent;
        };
        std::stack<info> nodes;
        nodes.push({ root, m_root, nullptr });
        while (!nodes.empty()) {
            auto data = nodes.top();
            nodes.pop();
            data.to = new node { data.from->value, nullptr, nullptr, data.parent };
            if (data.from->left != nullptr) {
                nodes.push({ data.from->left, data.to->left, data.to });
            }
            if (data.from->right != nullptr) {
                nodes.push({ data.from->right, data.to->right, data.to });
            }
        }
    }

    void erase_node(node * & n) {
        node * removed = n;
        if (n->left == nullptr) {
            n = n->right;
        } else if (n->right == nullptr) {
            n = n->left;
        } else {
            move_maximum_to(n);
        }
        assign_parent(n, removed->parent);
        delete removed;
    }

    void move_maximum_to(node * & root) {
        node ** ptr = &root->left;
        while ((*ptr)->right != nullptr) {
            ptr = &(*ptr)->right;
        }
        node * max = *ptr;
        assign_parent(max->left, max->parent);
        *ptr = max->left;
        max->left = root->left;
        assign_parent(max->left, max);
        max->right = root->right;
        assign_parent(max->right, max);
        root = max;
    }

    void assign_parent(node * & n, node * parent) {
        if (n != nullptr) {
            n->parent = parent;
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
