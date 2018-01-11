#include "avl_map.h"

#include <iostream>
#include <string>

using namespace std;

template <typename K, typename V>
ostream & operator<<(ostream & out, tree<K, V> & t) {
    out << "tree { ";
    for (const auto & x :t) {
        out << x.first << ":" << x.second << " ";
    }
    out << "}";
    return out;
}

template <typename K, typename V>
void iter_tree(tree<K, V> & t) {
    cout << "tree (usando un iterador): { ";
    for (auto p = t.begin(); p != t.end(); ++p) {
        cout << p->first << ":" << p->second << " ";
    }
    cout << "}" << endl;
}

void show_key_value(const int & k, string & v) {
    cout << k << ":" << v << ' ';
}

int main() {
    tree<int, string> t1;

    for (const auto & x: { 5, 3, 7, 1, 4, 2, 6, 0, 8 })
        t1.insert(x, "[" + to_string(x) + "]");

    cout << "t1 = " << t1 << endl;
    cout << "Graficando t1:" << endl;
    cout << t1.str();
    cout << endl << "t1 = ";
    iter_tree(t1);

    cout << endl << "Creando t2 como copia de t1:" << endl;
    auto t2 = t1;
    cout << "t2 = " << t2 << endl;

    cout << "Graficando t2:" << endl;
    cout << t2.str();

    cout << endl << "Haciendo más inserciones en t1:" << endl;
    t1.insert(5, "a");
    t1.insert(9, "b");
    t1.insert(5, "c");

    cout << "t1 = " << t1 << endl << t1.str() << endl;

    cout << "¿t1 contiene a 5? " << boolalpha << t1.contains(5) << endl;
    cout << "¿t1 contiene a 42? " << boolalpha << t1.contains(42) << endl;

    auto p = t1.find(7);
    if (p != end(t1)) {
        cout << "El valor asociado a " << p->first << " es: " << p->second << endl;
        p->second = "z";
    }

    cout << "Borrando un 5 => " << t1.erase(5).first << endl;
    cout << "Borrando un 42 => " << t1.erase(42).first << endl;
    cout << "t1 = " << t1 << endl << t1.str() << endl;

    cout << "Mínimo de t1 => " << t1.minimum()->first << endl;
    cout << "Máximo de t1 => " << t1.maximum()->first << endl << endl;

    cout << "Vaciando a t1:" << endl;
    t1.clear();
    cout << "t1 = " << t1 << " - ¿t1 está vacio? " << t1.empty() << endl << t1.str() << endl;

    cout << "Haciendo t1 = t2:" << endl;
    t1 = t2;
    cout << "t1 = " << t1 << " - ¿t1 está vacio? " << t1.empty() << endl << t1.str() << endl;
    cout << "t2 = " << t2 << " - ¿t2 está vacio? " << t2.empty() << endl << t2.str() << endl;

    cout << "Recorreindo t2 con each():" << endl;
    cout << "t2 = tree { ";
    t2.each(show_key_value);
    cout << "}" << endl;

    cout << endl << "Insertando en t3 valores del 1 al 16 y un 9 extra:" << endl;
    tree<int, string> t3;
    for (int x = 1 ; x < 17; ++x)
        t3.insert(x, "(n." + to_string(x) + ")");
    t3.insert(9, "y");

    cout << "t3 = " << t3 << endl << t3.str() << endl;

    cout << "mínimo de t3 = " << t3.minimum()->first << endl;
    cout << "máximo de t3 = " << t3.maximum()->first << endl;

    cout << "Borrando un 8 => " << t3.erase(8).first << endl;
    cout << "t3 = " << t3 << endl << t3.str() << endl;
    cout << "Borrando un 8 => " << t3.erase(8).first << endl << endl;

    for (int x = 1 ; x < 17; ++x) {
        cout << "Borrando un " << x << " => " << t3.erase(x).first << endl;
        cout << "t3 = " << t3 << endl << t3.str() << endl;
    }
}
