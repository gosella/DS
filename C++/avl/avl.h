#ifndef AVL_H
#define AVL_H

#include <algorithm>  // Para std::max
#include <cstddef>    // Para std::size_t
#include <functional> // Para std::function
#include <iterator>   // Para std::forward_iterator_tag
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
/************** Árbol AVL balanceado implementado con iterador liviano **************/
/************************************************************************************/

template <typename T>
class tree {
    struct node {
        T value;
        node * left;
        node * right;
        node * parent;
        int height;

        node * find_minimum() {
            node * minimum = this;
            while (minimum->left != nullptr) {
                minimum = minimum->left;
            }
            return minimum;
        }

        node * find_maximum() {
            node * maximum = this;
            while (maximum->right != nullptr) {
                maximum = maximum->right;
            }
            return maximum;
        }

        void update_height() {
            int left_height = 0;
            if (left != nullptr) {
                left_height = left->height;
            }
            int right_height = 0;
            if (right != nullptr) {
                right_height = right->height;
            }
            height = 1 + std::max(left_height, right_height);
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

    tree(const tree & x) {
        m_root = nullptr;
        copy_nodes(x.m_root, m_root, nullptr);
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
        using pointer = value_type *;
        using reference = value_type &;
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
        if (empty())
            return end();
        return iterator(m_root->find_minimum());
    }

    iterator end() {
        return iterator(nullptr);
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
        return iterator(m_root->find_minimum());
    }

    iterator maximum() {
        if (m_root == nullptr)
            return end();
        return iterator(m_root->find_maximum());
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
        auto result = do_erase(m_root, value);
        if (result.first && !empty()) {
            m_root->update_height();
        }
        return result;
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
            this_node = new node { other_node->value, nullptr, nullptr, parent, other_node->height };
            copy_nodes(other_node->left, this_node->left, this_node);
            copy_nodes(other_node->right, this_node->right, this_node);
        }
    }

    void do_each(node * current, std::function<void(const T &)> func) {
        if (current != nullptr) {
            do_each(current->left, func);
            func(current->value);
            do_each(current->right, func);
        }
    }

    iterator do_insert(node * & current, node * parent, const T & value) {
        if (current == nullptr) {
            current = new node { value, nullptr, nullptr, parent, 1 };
            return iterator(current);
        }

        if (value == current->value) {
            return iterator(current);
        }

        iterator result;
        if (value < current->value) {
            result = do_insert(current->left, current, value);
        } else {
            result = do_insert(current->right, current, value);
        }
        balance_tree(current);
        return result;
    }

    void balance_tree(node * & root) {
        int bf = balance_factor(root);
        if (bf == 2) {
            if (balance_factor(root->left) == -1) {
                rotate_left(root->left);
            }
            rotate_right(root);
        } else if (bf == -2) {
            if (balance_factor(root->right) == 1) {
                rotate_right(root->right);
            }
            rotate_left(root);
        } else {
            root->update_height();
        }
    }

    int balance_factor(const node * a_node) {
        int result = 0;
        if (a_node != nullptr) {
            if (a_node->left != nullptr) {
                result = a_node->left->height;
            }
            if (a_node->right != nullptr) {
                result -= a_node->right->height;
            }
        }
        return result;
    }

    void assign_parent(node * & n, node * p) {
        if (n != nullptr) {
            n->parent = p;
        }
    }

    void rotate_left(node * & root) {
        node * right_tree = root->right;
        root->right = right_tree->left;
        assign_parent(root->right, root);
        right_tree->left = root;
        right_tree->parent = root->parent;
        root->parent = right_tree;
        root = right_tree;
        root->left->update_height();
        root->update_height();
    }

    void rotate_right(node * & root) {
        node * left_tree = root->left;
        root->left = left_tree->right;
        assign_parent(root->left, root);
        left_tree->right = root;
        left_tree->parent = root->parent;
        root->parent = left_tree;
        root = left_tree;
        root->right->update_height();
        root->update_height();
    }

    std::pair<bool, iterator> do_erase(node * & current, const T & value) {
        if (current == nullptr) {
            return { false, end() };
        }

        if (value == current->value) {
            iterator next = ++iterator(current);
            erase_node(current);
            return { true, next };
        }

        std::pair<bool, iterator> result;
        if (value < current->value) {
            result = do_erase(current->left, value);
        } else {
            result = do_erase(current->right, value);
        }
        if (result.first) {
            balance_tree(current);
        }
        return result;
    }

    void erase_node(node * & n) {
        node * removed = n;
        if (n->left == nullptr) {
            n = n->right;
        } else if (n->right == nullptr) {
            n = n->left;
        } else {
            move_maximum_to(n, n->left);
            balance_tree(n);
        }
        assign_parent(n, removed->parent);
        delete removed;
    }

    void move_maximum_to(node * & root, node * & current) {
        if (current->right == nullptr) {
            replace_node(root, current);
        } else {
            move_maximum_to(root, current->right);
            current->update_height();
        }
    }

    void replace_node(node * & original, node * & replacement) {
        // Engancha al nodo "y" para que esté en el lugar del nodo "x",
        // al que borrará luego de haber hecho el cambio (x <- y).
        node * r = replacement;
        assign_parent(replacement->left, replacement->parent);
        replacement = replacement->left;
        r->left = original->left;
        r->right = original->right;
        assign_parent(r->left, r);
        assign_parent(r->right, r);
        r->parent = original->parent;
        original = r;
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

#endif // AVL_H
