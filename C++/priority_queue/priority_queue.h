#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include "heap.h"

template <typename P, typename T, typename Comparator = std::less<P>>
class priority_queue {
public:
    using value_type = T;
    using priority_type = P;

    priority_queue(Comparator cmp = Comparator()) {
        m_cmp = priority_comparator { cmp };
    }

    void push(const priority_type & priority, const value_type & value) {
        size_t i = m_data.size();
        m_data.push_back({priority, value});
        up_heap(m_data, i, m_cmp);
    }

    void pop() {
        m_data.front() = m_data.back();
        m_data.pop_back();
        down_heap(m_data, 0, m_cmp);

    }

    std::pair<const priority_type, value_type &> top() {
        return { m_data.front().first, m_data.front().second };
    }

    bool empty() {
        return m_data.empty();
    }

    size_t size() {
        return m_data.size();
    }

    friend
    std::ostream & operator<<(std::ostream & out, priority_queue & p) {
        out << "priority_queue { ";
        for (auto d : p.m_data) {
            out << d.first << ':' << d.second << ' ';
        }
        return out << "}";
    }

private:
    using data_type = std::pair<priority_type, value_type>;

    struct priority_comparator {
        Comparator cmp;

        bool operator()(const data_type & x, const data_type & y) {
            return cmp(x.first, y.first);
        }
    };

    std::vector<data_type> m_data;
    priority_comparator m_cmp;
};

#endif // PRIORITY_QUEUE_H
