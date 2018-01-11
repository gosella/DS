#include <iostream>
#include <vector>

#include "list.h"

using namespace std;

template <typename T>
std::ostream & operator<<(std::ostream & out, list<T> & lst) {
    out << "list { ";
    for(const T & x: lst)
        out << x << " ";
    out << "}";
    return out;
}

template <typename T>
void show_list(const char * name, list<T> & lst)
{
    cout << "Lista " << name << " - Elementos: " << lst.length();
    cout << endl << "Contenido: " << lst << endl << endl;
}

int main()
{
    list<int> list1;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(4);
    list1.push_back(5);
    list1.push_back(6);

    show_list("list1", list1);

    cout << "Creando list2 como copia de list1..." << endl;
    list<int> list2 = list1;
    show_list("list2", list2);

    cout << "Vaciando list1..." << endl;
    list1.clear();
    show_list("list1", list1);
    show_list("list2", list2);

    cout << "Manipulando list2..." << endl;
    auto it = ++list2.begin();
    it = list2.erase(it);
    ++it;
    it = list2.erase(it);
    list2.insert(list2.begin(), 10);
    auto it2 = list2.insert(it, 30);
    it2 = list2.insert(it2, 40);
    list2.insert(it2, 50);
    list2.insert(it, 20);
    list2.insert(list2.begin(), -10);
    show_list("list2", list2);

    cout << "Creando un vector<int> v a partir de los datos de list2..." << endl;
    vector<int> v;
    v.assign(list2.begin(), list2.end());

    cout << "Contenido: vector<int> {";
    for(const int & x: v)
        cout << x << " ";
    cout << "}" << endl << endl;

    cout << "Creando list3 vacía..." << endl;
    list<int> list3;
    show_list("list3", list3);

    cout << "Agregando elementos a list3 en forma ordenada..." << endl;
    for (const int & x : {1, 10, 2, 3, 7, 4, 9, 8, 6, 5, 0}) {
        auto p = list3.begin();
        while (p != list3.end() and *p < x)
            ++p;
        list3.insert(p, x);
    }
    cout << "Después de las inserciones en list3..." << endl;
    show_list("list3", list3);

    cout << "Creando list4 a partir de un rango..." << endl;
    int data[] = {1, 1, 2, 2, 4, 4, 7, 7, 9, 9, 20};
    list<int> list4;
    list4.assign(begin(data), end(data));
    show_list("list4", list4);

    cout << "Uniendo con merge() los elementos de list3 en list4..." << endl;
    list4.merge(list3);
    cout << "Resultado:" << endl;
    show_list("list3", list3);
    show_list("list4", list4);

    cout << "Pegando los elementos de list2 en el lugar 6 de list4..." << endl;
    auto i = list4.begin();
    advance(i, 6);
    auto from = list2.begin();
    advance(from, 5);
    auto to = from;
    advance(to, 3);
    list4.splice(i, from, to);
    cout << "Resultado:" << endl;
    show_list("list4", list4);
    show_list("list2", list2);
}
