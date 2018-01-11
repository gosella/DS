#include <iostream>
#include "stack.h"

int main() {
    using namespace std;

    stack<int> s1;

    for(auto x : {1, 2, 3, 4, 5}) {
        cout << "Apilando " << x << " en s1" << endl;
        s1.push(x);
    }

    debug(s1);

    auto s2 = s1;
    cout << "s1 == s2? " << boolalpha << (s1 == s2) << endl;
    debug(s2);

    cout << "Apilando 42 en s1" << endl;
    s1.push(42);
    cout << "s1 == s2? " << boolalpha << (s1 == s2) << endl;

    while (!s1.empty()) {
        auto x = s1.top();
        s1.pop();
        cout << "Desapilé " << x << " desde s1" << endl;
        debug(s1);
    }

    for (int n = 0; n < 3; ++n) {
        auto x = s2.top();
        s2.pop();
        cout << "Desapilé " << x << " desde s2" << endl;
        debug(s2);
    }

    for (int n = 10; n < 14; ++n) {
        cout << "Apilando " << n << " en s2" << endl;
        s2.push(n);
        debug(s2);
    }

    while (!s2.empty()) {
        auto x = s2.top();
        s2.pop();
        cout << "Desapilé " << x << " desde s2" << endl;
        debug(s2);
    }

    stack<string> s3;
    debug(s3);

    s3.push("Una cadena!");
    s3.push("Otra cadena!");
    debug(s3);

    while (!s3.empty()) {
        auto x = s3.top();
        s3.pop();
        cout << "Desapilé \"" << x << "\" desde s3" << endl;
        debug(s3);
    }
}
