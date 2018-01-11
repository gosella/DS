#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>

template <typename Graph>
void dfs_from(Graph & g,
                std::function<void(typename Graph::vertex_value_type &)> func,
                typename Graph::vertex_id_type v,
                std::unordered_set<typename Graph::vertex_id_type> & visited)
{
    visited.insert(v);
    func(g[v]);
    for(auto & w : g.adjacents(v)) {
        if (visited.find(w) == visited.end()) {
            dfs_from(g, func, w, visited);
        }
    }
}

template <typename Graph>
void recursive_dfs(Graph & g, std::function<void(typename Graph::vertex_value_type &)> func)
{
    std::unordered_set<typename Graph::vertex_id_type> visited;

    for (auto & v : g.vertices()) {
        if (visited.find(v) == visited.end()) {
            dfs_from(g, func, v, visited);
        }
    }
}

template <typename Graph>
void dfs(Graph & g, std::function<void(typename Graph::vertex_value_type &)> func)
{
    std::unordered_set<typename Graph::vertex_id_type> visited;
    std::stack<typename Graph::vertex_id_type> pending;

    for (auto & v : g.vertices()) {
        if (visited.find(v) != visited.end())
            continue;

        pending.push(v);
        visited.insert(v);

        while(!pending.empty()) {
            auto w = pending.top();
            pending.pop();
            func(g[w]);

            for (auto & x : g.adjacents(w)) {
                if (visited.find(x) == visited.end()) {
                   pending.push(x);
                   visited.insert(x);
                }
            }
        }
    }
}

template <typename Graph>
void bfs(Graph & g, std::function<void(typename Graph::vertex_value_type &)> func)
{
    std::unordered_set<typename Graph::vertex_id_type> visited;
    std::queue<typename Graph::vertex_id_type> pending;

    for (auto & v : g.vertices()) {
        if (visited.find(v) != visited.end())
            continue;

        pending.push(v);
        visited.insert(v);

        while(!pending.empty()) {
            auto w = pending.front();
            pending.pop();
            func(g[w]);

            for (auto & x : g.adjacents(w)) {
                if (visited.find(x) == visited.end()) {
                   pending.push(x);
                   visited.insert(x);
                }
            }
        }
    }
}

template <typename Graph>
std::pair<bool, std::vector<typename Graph::vertex_id_type>> topological_sort(Graph & g)
{
    std::unordered_map<typename Graph::vertex_id_type, std::size_t> in;
    for (auto & v : g.vertices())
        for(auto & w : g.adjacents(v))
            ++in[w];

    std::queue<typename Graph::vertex_id_type> pending;
    for (auto & v : g.vertices())
        if (in[v] == 0)
            pending.push(v);

    std::vector<typename Graph::vertex_id_type> result;
    while (!pending.empty()) {
        auto v = pending.front();
        pending.pop();
        result.push_back(v);

        for(auto & w : g.adjacents(v))
            if (--in[w] == 0)
                pending.push(w);
    }

    for (auto & x : in)
        if (x.second != 0) {
            result.clear();
            return { false, result };
        }

    return { true, result };
}


template <typename Graph>
auto shortest_path(Graph & g, typename Graph::vertex_id_type start)
{
    using id_t = typename Graph::vertex_id_type;
    using dist_t = typename Graph::edge_value_type;

    struct data_t {
        dist_t dist;
        bool visited;
        id_t prev;
    };
    std::vector<data_t> data;

    dist_t max_dist;
    if (std::numeric_limits<dist_t>::has_infinity) {
        max_dist = std::numeric_limits<dist_t>::infinity();
    } else {
        max_dist = std::numeric_limits<dist_t>::max();
    }
    data.resize(g.size(), { max_dist, false, start });

    data[start].dist = 0;
    for (auto & v : g.adjacents(start))
        data[v].dist = g.edge(start, v);

    while (true) {
        dist_t min_dist = max_dist;
        id_t min_v;
        for (auto & v : g.vertices()) {
            if (!data[v].visited && data[v].dist < min_dist) {
                min_dist = data[v].dist;
                min_v = v;
            }
        }

        if (min_dist == max_dist)
            break;

        data[min_v].visited = true;
        for (auto & v : g.adjacents(min_v)) {
            auto dt = data[min_v].dist + g.edge(min_v, v);
            if (dt < data[v].dist) {
                data[v].dist = dt;
                data[v].prev = min_v;
            }
        }
    }

    std::vector<std::pair<dist_t, std::vector<id_t>>> result;
    for (auto & v : g.vertices()) {
        std::vector<id_t> path;
        id_t n = v;
        while (data[n].visited) {
            path.push_back(n);
            if (n == start)
                break;
            n = data[n].prev;
        }
        std::reverse(path.begin(), path.end());
        result.push_back({data[v].dist, path});
    }

    return result;
}

#endif // GRAPH_ALGORITHMS_H
