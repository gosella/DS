#ifndef HEAP_H
#define HEAP_H

#include <cstddef>
#include <functional>
#include <utility>

template <typename Container, typename Comparator = std::less<>>
void up_heap(Container & data, std::size_t index, Comparator cmp = Comparator{}) {
    using std::swap;

    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (cmp(data[index], data[parent])) {
            swap(data[index], data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

template <typename Container, typename Comparator = std::less<>>
void down_heap(Container & data, std::size_t index, Comparator cmp = Comparator{}) {
    using std::swap;

    while (true) {
        std::size_t left = 2 * index + 1;
        std::size_t right = left + 1;
        std::size_t selected = index;
        if (left < data.size() && cmp(data[left], data[selected])) {
            selected = left;
        }
        if (right < data.size() && cmp(data[right], data[selected])) {
            selected = right;
        }
        if (selected == index) {
            break;
        }
        swap(data[index], data[selected]);
        index = selected;
    }
}

template <typename Container, typename Comparator = std::less<>>
void heapify(Container & data, Comparator cmp = Comparator{}) {
    if (data.size() < 2) {
        return;
    }
    std::size_t parent = (data.size() - 2) / 2;
    do {
        down_heap(data, parent, cmp);
    } while (parent-- > 0);
}

#endif // HEAP_H
