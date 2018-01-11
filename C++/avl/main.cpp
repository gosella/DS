#include "avl.h"

#include <iostream>
#include <string>

using namespace std;

template <typename T>
ostream & operator<<(ostream & out, tree<T> & t) {
    out << "tree { ";
    for (const auto & x : t) {
        out << x << " ";
    }
    out << "}" << endl << t.str();
    return out;
}

template <typename T>
void iter_tree(tree<T> & t) {
    cout << "tree (usando un iterador): { ";
    for (auto p = t.begin(); p != t.end(); ++p) {
        cout << *p << " ";
    }
    cout << "}" << endl;
}

void show_int(int v) {
    cout << v << ' ';
}

int main() {
    cout << ":: Creando t1 vacío." << endl;
    tree<int> t1;

    cout << "  t1 = " << t1 << endl;

    for (const auto & x: { 5, 3, 7, 1, 4, 2, 6, 0, 8 }) {
        cout << ":: Insertando " << x << " en t1 " << endl;
        t1.insert(x);
        cout << "  t1 = " << t1 << endl;
    }

    cout << "  t1 = ";
    iter_tree(t1);

    cout << ":: Creando t2 como copia de t1:" << endl;
    auto t2 = t1;
    cout << "  t2 = " << t2 << endl;

    cout << ":: ¿t1 == t2? " << boolalpha << (t1 == t2) << endl;

    cout << ":: Haciendo más inserciones en t1: 5, 9, 5" << endl;
    t1.insert(5);
    t1.insert(9);
    t1.insert(5);

    cout << "  t1 = " << t1 << endl;
    cout << "  t2 = " << t2 << endl;

    cout << ":: ¿t1 == t2? " << boolalpha << (t1 == t2) << endl;

    cout << ":: ¿t1 contiene a 5? " << boolalpha << t1.contains(5) << endl;
    cout << ":: ¿t1 contiene a 42? " << boolalpha << t1.contains(42) << endl;

    auto p = t1.find(7);
    cout << ":: ¿t1 contiene a 7? " << boolalpha << (p != end(t1)) << endl;
    if (p != end(t1)) {
        cout << "  Lo encontré! El valor es " << *p;
        ++p;
        cout << " y el siguiente es " << *p << endl;
    } else {
        cout << "  No encontré el valor buscado." << endl;
    }

    cout << ":: Mínimo de t1 => " << *t1.minimum() << endl;
    cout << ":: Máximo de t1 => " << *t1.maximum() << endl;

    cout << "  t1 = " << t1 << endl;

    for (int x : {5, 42}) {
        cout << ":: Borrando un " << x << " en t1 => " << t1.erase(x).first << endl;
        cout << "  t1 = " << t1 << endl;
    }

    cout << ":: Vaciando a t2:" << endl;
    t2.clear();
    cout << "  t2 = " << t2 << " - ¿t2 está vacio? " << t2.empty() << endl;

    cout << ":: Asignando t2 = t1:" << endl;
    t2 = t1;
    cout << "  t1 = " << t1 << " - ¿t1 está vacio? " << t1.empty() << endl;
    cout << "  t2 = " << t2 << " - ¿t2 está vacio? " << t2.empty() << endl;

    cout << ":: Recorriendo t2 con each():" << endl;
    cout << "  t2 = tree { ";
    t2.each(show_int);
    cout << "}" << endl;

    cout << "  t2 = " << t2 << endl;
    for (auto p = begin(t2); p != end(t2); ) {
        cout << ":: Borrando un " << *p << " en t2 => ";
        auto result = t2.erase(*p);
        cout << result.first << endl;
        p = result.second;
        cout << "  t2 = " << t2 << endl;
    }

    cout << endl << ":: Insertando en t3 valores del 1 al 16 y un 9 extra:" << endl;
    tree<int> t3;
    for (int x = 1 ; x < 17; ++x)
        t3.insert(x);
    t3.insert(9);

    cout << "  t3 = " << t3 << endl << t3.str() << endl;

    cout << ":: Mínimo de t3 = " << *t3.minimum() << endl;
    cout << ":: Máximo de t3 = " << *t3.maximum() << endl;

    cout << ":: Borrando un 8 => " << t3.erase(8).first << endl;
    cout << "  t3 = " << t3 << endl << t3.str() << endl;
    cout << ":: Borrando un 8 => " << t3.erase(8).first << endl << endl;

    for (int x = 1 ; x < 17; ++x) {
        cout << ":: Borrando un " << x << " => " << t3.erase(x).first << endl;
        cout << "  t3 = " << t3 << endl;
    }

    cout << ":: Usando un árbol de char como un conjunto => " << endl;
    tree<char> letras;
    string texto = "ESTRUCTURAS DE DATOS";
    for (char c : texto) {
        letras.insert(c);
    }
    cout << "  Letras en '" << texto << "': " << letras << endl;
}
