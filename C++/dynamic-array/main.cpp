#include <iostream>
#include "dynamic_array.h"

using namespace std;

void debug(dynamic_array<int> & a) {
    cout << "{ ";
    for (auto & x : a) {
        cout << x << ' ';
    }
    cout << "}"
         << " - size(): " << a.size()
         << " - capacity(): " << a.capacity()
         << endl;
}

int main() {
    dynamic_array<int> a1;

    cout << "Haciendo unos push_back()...\n";
    for (auto & x : {1, 2, 3, 4, 5}) {
        a1.push_back(x);
        debug(a1);
    }

    cout << "\nHaciendo varios push_back()...\n";
    for (int x = 10; x < 35; ++x) {
        a1.push_back(x);
        debug(a1);
    }

    cout << "\nHaciendo algunos insert()...\n";
    for (int i = 0; i < 10; ++i) {
        a1.insert(begin(a1) + 3 * i, 50 + i);
        debug(a1);
    }

    cout << "\nHaciendo unos pop_back()...\n";
    for (int i = 0; i < 15; ++i) {
        cout << "back(): " << a1.back() << " - ";
        a1.pop_back();
        debug(a1);
    }

    cout << "\nHaciendo algunos erase()...\n";
    for (int i = 0; i < 6; ++i) {
        a1.erase(begin(a1) + 2 * i + 1);
        debug(a1);
    }

    cout << "\nRecorriendo usando índices()...\n";
    cout << "{ ";
    for (size_t i = 0; i < a1.size(); ++i) {
        cout << a1[i] << ' ';
    }
    cout << "}" << endl;

    cout << "\nCopiando un arreglo...\n";
    auto a2 = a1;
    cout << "a1 = "; debug(a1);
    cout << "a2 = "; debug(a2);

    cout << "\nVaciando un arreglo...\n";
    a1.clear();
    cout << "a1 = "; debug(a1);
    cout << "a2 = "; debug(a2);

    cout << "\nAsignando un arreglo...\n";
    a1 = a2;
    cout << "a1 = "; debug(a1);
    cout << "a2 = "; debug(a2);

    cout << "\nVaciando el otro arreglo...\n";
    a2.clear();
    cout << "a1 = "; debug(a1);
    cout << "a2 = "; debug(a2);
}

