#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <forward_list>
#include <iterator>
#include <vector>

template <typename T, typename W>
class graph {
public:
    using vertex_id_type = std::size_t;
    using vertex_value_type = T;
    using edge_value_type = W;

    friend
    bool operator ==(const graph & x, const graph & y) {
        if (x.m_vertices.size() != y.m_vertices.size())
            return false;

        auto px = x.m_vertices.begin();
        auto py = y.m_vertices.begin();
        while (px != x.m_vertices.end()) {
            if (px->value != py->value)
                return false;

            auto ex = px->edges.begin();
            auto ey = py->edges.begin();
            while (ex != px->edges.end() && ey != py->edges.end()) {
                if (ex->to != ey->to ||  ex->weight != ey->weight)
                    return false;
                ++ex;
                ++ey;
            }
            if (ex != px->edges.end() || ey != py->edges.end())
                return false;

            ++px;
            ++py;
        }

        return true;
    }

    std::size_t size() {
        return m_vertices.size();
    }

    T & operator[](vertex_id_type id) {
        return m_vertices[id].value;
    }

    std::vector<vertex_id_type> vertices() {
        std::vector<vertex_id_type> result;
        for (vertex_id_type i = 0; i < m_vertices.size(); ++i)
            result.push_back(i);
        return result;
    }

    vertex_id_type add_vertex(const T & x) {
        vertex_id_type id = m_vertices.size();
        m_vertices.push_back({x, std::forward_list<edge_t>{} });
        return id;
    }

    void erase_vertex(vertex_id_type id) {
        m_vertices.erase(m_vertices.begin() + id);
        for (auto & vertex : m_vertices) {
            auto & edges = vertex.edges;
            auto p = edges.before_begin();
            auto prev = p++;
            while (p != edges.end()) {
                if (p->to == id) {
                    p = edges.erase_after(prev);
                } else {
                    if (p->to > id) {
                        --(p->to);
                    }
                    prev = p++;
                }
            }
        }
    }

    W & edge(vertex_id_type from, vertex_id_type to) {
        // Precondicion: Los vértices from y to están conectados.
        for (auto & e : m_vertices[from].edges) {
            if (e.to == to) {
                return e.weight;
            }
        }

        static edge_value_type dummy;
        return dummy; // Cuando la arista no existe, se devuelve un valor "de mentira".
    }

    void add_edge(vertex_id_type from, vertex_id_type to, const W & weight = W{}) {
        auto & edges = m_vertices[from].edges;
        auto p = edges.before_begin();
        auto prev = p++;
        while (p != edges.end()) {
            if (p->to == to) {
                p->weight = weight;
                return;
            } else if (p->to > to) {
                break;
            }
            prev = p++;
        }
        m_vertices[from].edges.insert_after(prev, {to, weight});
    }

    bool erase_edge(vertex_id_type from, vertex_id_type to) {
        auto & edges = m_vertices[from].edges;
        auto p = edges.before_begin();
        auto prev = p++;
        while (p != edges.end()) {
            if (p->to == to) {
                edges.erase_after(prev);
                return true;
            } else if (p->to > to) {
                break;
            }
            prev = p++;
        }
        return false;
    }

    bool is_adjacent(vertex_id_type from, vertex_id_type to) {
        for (auto & e : m_vertices[from].edges) {
            if (e.to == to) {
                return true;
            } else if (e.to > to) {
                break;
            }
        }
        return false;
    }

    std::vector<vertex_id_type> adjacents(vertex_id_type from) {
        std::vector<vertex_id_type> result;
        for (auto & e : m_vertices[from].edges) {
            result.push_back(e.to);
        }
        return result;
    }


private:
    struct edge_t {
        vertex_id_type to;
        W weight;
    };

    struct vertex_t {
        T value;
        std::forward_list<edge_t> edges;
    };

    std::vector<vertex_t> m_vertices;

};
#endif // ADJACENCY_LIST_H

