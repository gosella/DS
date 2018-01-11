#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <utility>
#include <vector>

template <std::size_t N, typename T, typename W>
class static_graph {
public:
    using vertex_id_type = std::size_t;
    using vertex_value_type = T;
    using edge_value_type = W;

    std::size_t size() {
        return N;
    }

    T & operator[](vertex_id_type id) {
        return m_vertices[id];
    }

    std::vector<vertex_id_type> vertices() {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < N; ++i)
            result.push_back(i);
        return result;
    }

    W & edge(vertex_id_type from, vertex_id_type to) {
        return m_edges[from][to].second;
    }

    void add_edge(vertex_id_type from, vertex_id_type to, const W & weight = W{}) {
        m_edges[from][to] = { true, weight };
    }

    bool erase_edge(vertex_id_type from, vertex_id_type to) {
        bool b = m_edges[from][to].first;
        m_edges[from][to].first = false;
        return b;
    }

    bool is_adjacent(vertex_id_type from, vertex_id_type to) {
        return m_edges[from][to].first;
    }

    std::vector<vertex_id_type> adjacents(vertex_id_type from) {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < N; ++i)
            if (m_edges[from][i].first)
                result.push_back(i);
        return result;
    }

private:
    T m_vertices[N];
    std::pair<bool, W> m_edges[N][N];
};



template <typename T, typename W>
class dynamic_graph {
public:
    using vertex_id_type = std::size_t;
    using vertex_value_type = T;
    using edge_value_type = W;

    std::size_t size() {
        return m_vertices.size();
    }

    T & operator[](vertex_id_type id) {
        return m_vertices[id];
    }

    std::vector<vertex_id_type> vertices() {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < m_vertices.size(); ++i)
            result.push_back(i);
        return result;
    }

    vertex_id_type add_vertex(const T & x) {
        vertex_id_type id = m_vertices.size();
        m_vertices.push_back(x);
        for (auto & e : m_edges) {
            e.push_back({false, W{}});
        }
        std::vector<std::pair<bool, W>> v;
        v.resize(id + 1);
        m_edges.push_back(v);
        return id;
    }

    void erase_vertex(vertex_id_type i) {
        m_vertices.erase(m_vertices.begin() + i);
        m_edges.erase(m_edges.begin() + i);
        for (auto & edges : m_edges) {
            edges.erase(edges.begin() + i);
        }
    }

    W & edge(vertex_id_type from, vertex_id_type to) {
        return m_edges[from][to].second;
    }

    void add_edge(vertex_id_type from, vertex_id_type to, const W & weight = W{}) {
        m_edges[from][to] = { true, weight };
    }

    bool erase_edge(vertex_id_type from, vertex_id_type to) {
        bool b = m_edges[from][to].first;
        m_edges[from][to].first = false;
        return b;
    }

    bool is_adjacent(vertex_id_type from, vertex_id_type to) {
        return m_edges[from][to].first;
    }

    std::vector<vertex_id_type> adjacents(vertex_id_type from) {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < size(); ++i)
            if (m_edges[from][i].first)
                result.push_back(i);
        return result;
    }

private:
    std::vector<T> m_vertices;
    std::vector<std::vector<std::pair<bool, W>>> m_edges;
};



template <typename T, typename W>
class compact_graph {
public:
    using vertex_id_type = std::size_t;
    using vertex_value_type = T;
    using edge_value_type = W;

    std::size_t size() {
        return m_vertices.size();
    }

    T & operator[](vertex_id_type id) {
        return m_vertices[id];
    }

    std::vector<vertex_id_type> vertices() {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < m_vertices.size(); ++i)
            result.push_back(i);
        return result;
    }

    vertex_id_type add_vertex(const T & x) {
        //   Ejemplo de 0x0 a 1x1:
        //
        //        Inicial: p = 0 y q = 1-0-1 = 0, nada para hacer.
        //
        //
        //   Ejemplo de 1x1 a 2x2:
        //
        //   0  =>  0 X  =>
        //          X X
        //
        //        Inicial:  p = 1 y q = 4 - 2 - 1 = 1, nada que hacer.
        //
        //
        //   Ejemplo de 3x3 a 4x4:
        //
        //   0 1 2  =>  0 1 2 3  =>  0 1 2 F
        //   3 4 5      4 5 6 7      3 4 5 F
        //   6 7 8      8 X X X      6 7 8 X
        //              X X X X      X X X X
        //
        //        Inicial:  p = 9 y q = 16 - 4 - 1 = 11
        //   v[10] = v[8],  p = 8 y q = 10  % 4 != 0
        //    v[9] = v[7],  p = 7 y q = 9   % 4 != 0
        //    v[8] = v[6],  p = 6 y q = 8   % 4 == 0 ==> q = 7 y v[7] = F
        //    v[6] = v[5],  p = 5 y q = 6   % 4 != 0
        //    v[5] = v[4],  p = 4 y q = 5   % 4 != 0
        //    v[4] = v[3],  p = 3 y q = 4   % 4 == 0 ==> q = 3 y v[3] = F
        //            FIN,  p = 2 y q = 2
        //
        vertex_id_type id = m_vertices.size();
        m_vertices.push_back(x);

        size_t p = m_edges.size();
        m_edges.resize(m_vertices.size() * m_vertices.size());
        size_t q = m_edges.size() - m_vertices.size() - 1;

        while (q > p) {
            --p;
            --q;
            m_edges[q] = m_edges[p];
            if (q % m_vertices.size() == 0) {
                --q;
                m_edges[q].first = false;
            }
        }

        return id;
    }

    void erase_vertex(vertex_id_type id) {
        //
        //   Ejemplo de 2x2 a 1x1:
        //
        //   0 1  => [0][1] =>  3 X  =>  3
        //   2 3     [2] 3      X X
        //
        //   0 1  =>  0 [1] =>  0 X  =>  0
        //   2 3     [2][3]     X X
        //
        //
        //   Ejemplo de 3x3 a 2x2:
        //
        //   0 1 2  => [0][1][2] =>  4 5 7  =>  4 5
        //   3 4 5     [3] 4  5      8 X X      7 8
        //   6 7 8     [6] 7  8      X X X
        //
        //   0 1 2  =>  0 [1] 2  =>  0 2 6  =>  0 2
        //   3 4 5     [3][4][5]     8 X X      6 8
        //   6 7 8      6 [7] 8      X X X
        //
        //   0 1 2  =>  0  1 [2] =>  0 1 3  =>  0 1
        //   3 4 5      3  4 [5]     4 X X      3 4
        //   6 7 8     [6][7][8]     X X X
        //

        size_t q = id;
        size_t p = id + 1;
        size_t n = m_vertices.size();
        size_t m = (n-1) * (n-1);

        while (q < m) {
            if (p / n == id) {
                p += n;
            }
            m_edges[q] = m_edges[p];
            ++p;
            ++q;
            if (p % n == id) {
                ++p;
            }
        }

        m_edges.resize(m);
        m_vertices.erase(m_vertices.begin() + id);
    }

    W & edge(vertex_id_type from, vertex_id_type to) {
        return m_edges[get_index(from, to)].second;
    }

    void add_edge(vertex_id_type from, vertex_id_type to, const W & weight = W{}) {
        m_edges[get_index(from, to)] = { true, weight };
    }

    bool erase_edge(vertex_id_type from, vertex_id_type to) {
        bool b = m_edges[get_index(from, to)].first;
        m_edges[get_index(from, to)].first = false;
        return b;
    }

    bool is_adjacent(vertex_id_type from, vertex_id_type to) {
        return m_edges[get_index(from, to)].first;
    }

    std::vector<vertex_id_type> adjacents(vertex_id_type from) {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < size(); ++i)
            if (m_edges[get_index(from, i)].first)
                result.push_back(i);
        return result;
    }

private:
    std::vector<T> m_vertices;
    std::vector<std::pair<bool, W>> m_edges;

    std::size_t get_index(vertex_id_type from, vertex_id_type to) {
        return from * size() + to;
    }
};

#endif // ADJACENCY_MATRIX_H
