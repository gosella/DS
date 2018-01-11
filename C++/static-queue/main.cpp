#include <iostream>
#include "queue.h"

int main() {
    using namespace std;

    queue<int, 6> q1;

    for(auto x : {1, 2, 3, 4, 5}) {
        cout << "Encolando " << x << " en q1" << endl;
        q1.enqueue(x);
    }

    debug(q1);

    auto q2 = q1;
    cout << "q1 == q2? " << boolalpha << (q1 == q2) << endl;
    debug(q2);

    cout << "Encolando 42 en q1" << endl;
    q1.enqueue(42);
    cout << "q1 == q2? " << boolalpha << (q1 == q2) << endl;

    while (!q1.empty()) {
        auto x = q1.front();
        q1.dequeue();
        cout << "Desencolé " << x << " desde q1" << endl;
        debug(q1);
    }

    for (int n = 0; n < 3; ++n) {
        auto x = q2.front();
        q2.dequeue();
        cout << "Desencolé " << x << " desde q2" << endl;
        debug(q2);
    }

    for (int n = 10; n < 14; ++n) {
        cout << "Encolando " << n << " en q2" << endl;
        q2.enqueue(n);
        debug(q2);
    }

    while (!q2.empty()) {
        auto x = q2.front();
        q2.dequeue();
        cout << "Desencolé " << x << " desde q2" << endl;
        debug(q2);
    }

    queue<string, 10> q3;
    debug(q3);

    q3.enqueue("Una cadena!");
    q3.enqueue("Otra cadena!");
    debug(q3);

    while (!q3.empty()) {
        auto x = q3.front();
        q3.dequeue();
        cout << "Desencolé \"" << x << "\" desde q3" << endl;
        debug(q3);
    }
}
