#include <iostream>
#include <fstream>
#include "adjacency_matrix.h"
#include "adjacency_list.h"
#include "graph_algorithms.h"

using namespace std;

void imprimir(string & x) {
    cout << x << ' ';
}

template <typename Graph>
void mostrar(Graph & g) {
    cout << "grafo { ";
    for (auto & v: g.vertices()) {
        for (auto & w : g.adjacents(v)) {
            cout << v << "->" << w
                 << " (" << g.edge(v, w) << ") ";
        }
    }
    cout << "}" << endl;
}

template <typename Graph>
void crear_conexiones(Graph & g) {
    g.add_edge(0, 2, 8);
    g.add_edge(0, 3, 2);
    g.add_edge(1, 5, 8);
    g.add_edge(1, 6, 1);
    g.add_edge(3, 1, 1);
    g.add_edge(4, 2, 7);
    g.add_edge(4, 6, 9);
    g.add_edge(4, 7, 6);
    g.add_edge(5, 1, 1);
    g.add_edge(6, 2, 6);
    g.add_edge(6, 4, 7);
    g.add_edge(7, 5, 9);
    g.add_edge(7, 6, 6);
}

template <typename Graph>
void hacer_recorridos(Graph & g) {
    cout << "Con ciclos: ";
    recorridos(g);

    g.erase_edge(5, 1);
    g.erase_edge(5, 1);
    g.erase_edge(6, 4);
    g.erase_edge(4, 7);

    cout << "Sin ciclos: ";
    recorridos(g);
}

template <typename Graph>
void recorridos(Graph & g) {
    cout << "DFS (iter): { ";
    dfs(g, imprimir);
    cout << "}  ";

    cout << "DFS (rec): { ";
    recursive_dfs(g, imprimir);
    cout << "}  ";

    cout << "BFS: { ";
    bfs(g, imprimir);
    cout << "}  ";

    cout << "Topo: ";
    auto r = topological_sort(g);
    if (r.first)
        cout << "(sin ciclos)";
    else
        cout << "(con ciclos)";
    cout << " { ";
    for (auto & v : r.second)
        cout << v << " ";
    cout << "}" << endl;
}


template <typename Graph>
void caminos_mas_cortos(Graph & g, typename Graph::vertex_id_type start) {
    auto caminos = shortest_path(g, start);

    cout << "Caminos más cortos:" << endl;
    for (auto & v : g.vertices()) {
        cout << "De " << start << " a " << v << ": D = "
             << caminos[v].first << " - Recorrido: { ";
        for (auto & w : caminos[v].second) {
            cout << w << " ";
        }
        cout << "}" << endl;
    }
}

int main() {
    static_graph<8, string, int> g1;
    g1[0] = "A";
    g1[1] = "B";
    g1[2] = "C";
    g1[3] = "D";
    g1[4] = "E";
    g1[5] = "F";
    g1[6] = "G";
    g1[7] = "H";

    crear_conexiones(g1);
    cout << "g1 = "; mostrar(g1);
    caminos_mas_cortos(g1, 0);

    dynamic_graph<string, int> g2;
    g2.add_vertex("A");
    g2.add_vertex("B");
    g2.add_vertex("C");
    g2.add_vertex("D");
    g2.add_vertex("E");
    g2.add_vertex("F");
    g2.add_vertex("G");
    g2.add_vertex("H");

    crear_conexiones(g2);
    cout << "\ng2 = "; mostrar(g2);
    caminos_mas_cortos(g2, 0);

    graph<string, int> g3;
    g3.add_vertex("A");
    g3.add_vertex("B");
    g3.add_vertex("C");
    g3.add_vertex("D");
    g3.add_vertex("E");
    g3.add_vertex("F");
    g3.add_vertex("G");
    g3.add_vertex("H");

    crear_conexiones(g3);
    cout << "\ng3 = "; mostrar(g3);
    caminos_mas_cortos(g3, 0);

    compact_graph<string, int> g4;
    g4.add_vertex("A");
    g4.add_vertex("B");
    g4.add_vertex("C");
    g4.add_vertex("D");
    g4.add_edge(0, 2, 8);
    g4.add_edge(0, 3, 2);
    g4.add_edge(3, 1, 1);
    g4.add_vertex("E");
    g4.add_edge(4, 2, 7);
    g4.add_vertex("F");
    g4.add_edge(1, 5, 8);
    g4.add_edge(5, 1, 1);
    g4.add_vertex("G");
    g4.add_edge(1, 6, 1);
    g4.add_edge(4, 6, 9);
    g4.add_edge(6, 2, 6);
    g4.add_edge(6, 4, 7);
    g4.add_vertex("H");
    g4.add_edge(4, 7, 6);
    g4.add_edge(7, 5, 9);
    g4.add_edge(7, 6, 6);

    cout << "\ng4 = "; mostrar(g4);
    caminos_mas_cortos(g4, 0);

    cout << "\n:: Recorridos:" << endl;
    cout << "grafo g1:" << endl; hacer_recorridos(g1);
    cout << "grafo g2:" << endl; hacer_recorridos(g2);
    cout << "grafo g3:" << endl; hacer_recorridos(g3);
    cout << "grafo g4:" << endl; hacer_recorridos(g4);

    cout << "\n:: Creando g5 como copia de g3:" << endl;
    auto g5 = g3;
    cout << "g3 = "; mostrar(g3);
    cout << "g5 = "; mostrar(g5);
    cout << "¿g3 == g5? " << boolalpha << (g3 == g5) << endl;

    cout << "\n:: Borrando un vértice de g2:" << endl;
    cout << "Antes:   "; mostrar(g2);
    g2.erase_vertex(3);
    cout << "Después: "; mostrar(g2);

    cout << "\n:: Borrando un vértice de g3:" << endl;
    cout << "Antes:   "; mostrar(g3);
    g3.erase_vertex(3);
    cout << "Después: "; mostrar(g3);

    cout << "\n:: Borrando un vértice de g4:" << endl;
    cout << "Antes:   "; mostrar(g4);
    g4.erase_vertex(3);
    cout << "Después: "; mostrar(g4);

    cout << "\n:: Asignando g5 a g3:" << endl;
    cout << "Antes:   g3 = "; mostrar(g3);
    g3 = g5;
    cout << "Después: g3 = "; mostrar(g3);
}
