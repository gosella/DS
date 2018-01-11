#include <iostream>
#include <functional>
#include <vector>

#include "heap.h"
#include "priority_queue.h"

using namespace std;

template<typename T>
ostream & operator<<(ostream & out, const vector<T> & vec) {
    out << "vector<T> { ";
    for (auto & d : vec) {
        out << d << ' ';
    }
    return out << "}";
}

int main() {
    int data[] = {1, 10, 4, 20, 5, 7, 3, 12, 15, 11, 18, 25, 2, 0, 8, 1};

    // Usando las funciones para operar sobre vectores **********************

    vector<int> v { begin(data), end(data) };
    cerr << "v = " << v << endl;

    auto cmp = std::greater<> {};
    heapify(v, cmp);
    cerr << "v = " << v << endl;

    v.push_back(42);
    up_heap(v, v.size() - 1, cmp);
    cerr << "v = " << v << endl;

    while (!v.empty()) {
        cerr << "v.top() => " << v.front() << endl;
        v.front() = v.back();
        v.pop_back();
        down_heap(v, 0, cmp);
    }

    cerr << "v = " << v << endl;
    cerr << string(70, '=') << endl;

    // Probando la cola con prioridades *************************************

    priority_queue<int, string> q1;
    for (auto & x : data) {
        q1.push(x, to_string(2 * x));
    }

    cerr << "q1 = " << q1 << endl;

    cerr << "q1.top().first = " << q1.top().first << endl;
    cerr << "q1.top().second = " << q1.top().second << endl;

//    q1.top().first = 100;   // No puedo modificar las prioridades!
    q1.top().second = "cambiado";

    cerr << "q1.top().first = " << q1.top().first << endl;
    cerr << "q1.top().second = " << q1.top().second << endl;

    cerr << "q1 = " << q1 << endl;
    cerr << string(70, '=') << endl;

    priority_queue<int, string> q2;
    q2 = q1;
    cerr << "q2 = " << q2 << endl;
    cerr << string(70, '=') << endl;

    auto q3 = q1;
    cerr << "q3 = " << q3 << endl;
    cerr << string(70, '=') << endl;

    while (!q1.empty()) {
        cerr << "q1.top() => " << q1.top().first << ", " << q1.top().second
             << "\t q2.top() => " << q2.top().first << ", " << q2.top().second
             << endl;
        q1.pop();
        q2.pop();
    }

    cerr << string(70, '=') << endl;

    priority_queue<int, string, std::greater<>> q4;
    while (!q3.empty()) {
        if (q3.top().first > 10)
            break;

        q4.push(q3.top().first, q3.top().second);
        q3.pop();
    }
    q4.push(100, "cien");
    q4.push(-20, "menos veinte");

    cerr << "q3 = " << q3 << endl;
    cerr << "q4 = " << q4 << endl;
    cerr << string(70, '=') << endl;

    while (!q4.empty()) {
        cerr << "q4.top() => " << q4.top().first  << ", " << q4.top().second << endl;
        q4.pop();
    }
    cerr << string(70, '=') << endl;
}
