# Estructuras de Datos

Este repositorio contiene la implementación de varias estructuras de datos clásicas implementadas en C++11 usadas en un curso universitario:
- [Arreglo de tamaño dinámico](C++/dynamic-array/dynamic_array.h).
- [Pila de tamaño estático](C++/static-stack/stack.h).
- [Cola de tamaño estático](C++/static-queue/queue.h).
- [Pila de tamaño dinámico](C++/dynamic-stack/stack.h).
- [Cola de tamaño dinámico](C++/dynamic-queue/queue.h).
- [Lista enlazada simple](C++/singly-linked-list/forward_list.h).
- [Lista doblemente enlazada](C++/doubly-linked-list/list.h).
- Árboles binarios de búsqueda:
    - [Implementado de forma recursiva](C++/recursive-BST-light-iterator/tree.h).
    - Implementado de forma iterativa:
        - [Usando un iterador liviano con nodos conteniendo enlaces a sus padres](C++/iterative-BST-light-iterator/tree.h).
        - [Usando un iterador pesado conteniendo los enlaces dentro del propio iterador](C++/iterative-BST-fat-iterator/tree.h).
        - [Usando un iterador bidireccional con nodos conteniendo enlaces a sus padres](C++/iterative-BST-bidirectional-light-iterator/tree.h).
- [Árbol AVL](C++/avl/avl.h).
- [Implementación de un mapa asociativo (usando internamiente un árbol AVL)](C++/avl-as-map/avl_map.h).
- [Cola con prioridad](C++/priority_queue/priority_queue.h) usando internamente un [montículo binario](C++/priority_queue/heap.h).
- Grafos:
    - [Usando lista de adyacencia](C++/graphs/adjacency_list.h).
    - [Usando matriz de adyacencia](C++/graphs/adjacency_matrix.h).
    - [Algoritmos sobre grafos](C++/graphs/graph_algorithms.h):
        - Deep First Search (iterativo y recursivo).
        - Breath First Search.
        - Orden topológico.
        - Camino más corto (algoritmo de Dijkstra).

Muchas de las estructuras de datos también están [implementadas en Python](Python/).
