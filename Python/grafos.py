#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from namedlist import namedlist

###############################################################################

class AdjacencyMatrixGraph:
    __slots__ = ['_vertices', '_edges']

    def __init__(self):
        self._vertices = []
        self._edges = []


    def copy(self):
        result = AdjacencyMatrixGraph()
        result._vertices = self._vertices.copy()
        result._edges = [edge.copy() for edge in self._edges]
        return result


    def is_empty(self):
        return len(self._vertices) == 0


    def __len__(self):
        return self._vertices


    def vertices(self):
        return list(range(len(self._vertices)))


    def get_vertex(self, vertex):
        return self._vertices[vertex]


    def set_vertex(self, vertex, value):
        self._vertices[vertex] = value


    def add_vertex(self, value):
        vertex = len(self._vertices)
        self._vertices.append(value)
        for edges in self._edges:
            edges.append(None)
        self._edges.append([None] * (vertex + 1))
        return vertex


    def erase_vertex(self, vertex):
        assert 0 <= vertex < len(self._vertices)
        del self._vertices[vertex]
        del self._edges[vertex]
        for edges in self._edges:
            del edges[vertex]


    def __getitem__(self, key):
        if isinstance(key, tuple):
            return self.get_edge(key[0], key[1])
        return self.get_vertex(key)


    def __setitem__(self, key, value): 
        if isinstance(key, tuple):
            return self.set_edge(key[0], key[1], value)
        return self.set_vertex(key, value)


    def __delitem__(self, key):
        if isinstance(key, tuple):
            self.erase_edge(key[0], key[1])
        else:
            self.erase_vertex(key)


    def get_edge(self, from_, to):
        return self._edges[from_][to]


    def set_edge(self, from_, to, weight):
        self._edges[from_][to] = weight


    def add_edge(self, from_, to, weight):
        self._edges[from_][to] = weight


    def erase_edge(self, from_, to):
        if self._edges[from_][to] is not None:
            self._edges[from_][to] = None
            return True
        return False


    def is_adjacent(self, from_, to):
        return self._edges[from_][to] is not None


    def adjacents(self, from_):
        result = []
        for i in range(len(self._vertices)):
            if self._edges[from_][i] is not None:
                result.append(i)
        return result


    def __eq__(self, other):
        return self._vertices == other._vertices and self._edges == other.edges


    def __repr__(self):
        result = '( '
        for v in self.vertices():
            for w in self.adjacents(v):
                result += '{0}-({2})->{1}, '.format(v, w, self.get_edge(v, w))
        return result + ') : AdjacencyMatrixGraph'


###############################################################################


class CompactAdjacencyMatrixGraph:
    __slots__ = ['_vertices', '_edges']

    def __init__(self):
        self._vertices = []
        self._edges = []


    def copy(self):
        result = CompactAdjacencyMatrixGraph()
        result._vertices = self._vertices.copy()
        result._edges = self._edges.copy()
        return result


    def is_empty(self):
        return len(self._vertices) == 0


    def __len__(self):
        return self._vertices


    def vertices(self):
        return list(range(len(self._vertices)))


    def get_vertex(self, vertex):
        return self._vertices[vertex]


    def set_vertex(self, vertex, value):
        self._vertices[vertex] = value


    def add_vertex(self, value):
        # Ejemplo de 0x0 a 1x1:
        #
        #        Inicial: p = 0 y q = 1-0-1 = 0, nada para hacer.
        #
        # Ejemplo de 1x1 a 2x2:
        #
        #   0  =>  0 X  =>
        #          X X
        #
        #        Inicial:  p = 1 y q = 4 - 2 - 1 = 1, nada que hacer.
        #
        # Ejemplo de 3x3 a 4x4:
        #
        #   0 1 2  =>  0 1 2 3  =>  0 1 2 F
        #   3 4 5      4 5 6 7      3 4 5 F
        #   6 7 8      8 X X X      6 7 8 X
        #              X X X X      X X X X
        #
        #        Inicial:  p = 9 y q = 16 - 4 - 1 = 11
        #   v[10] = v[8],  p = 8 y q = 10  % 4 != 0
        #    v[9] = v[7],  p = 7 y q = 9   % 4 != 0
        #    v[8] = v[6],  p = 6 y q = 8   % 4 == 0 ==> q = 7 y v[7] = F
        #    v[6] = v[5],  p = 5 y q = 6   % 4 != 0
        #    v[5] = v[4],  p = 4 y q = 5   % 4 != 0
        #    v[4] = v[3],  p = 3 y q = 4   % 4 == 0 ==> q = 3 y v[3] = F
        #            FIN,  p = 2 y q = 2
        #
        vertex = len(self._vertices)
        self._vertices.append(value)

        p = len(self._edges)
        self._edges.extend([None] * (len(self._vertices) ** 2 - p))
        q = len(self._edges) - len(self._vertices) - 1

        while q > p:
            p -= 1
            q -= 1
            self._edges[q] = self._edges[p]
            if q % len(self._vertices) == 0:
                q -= 1
                self._edges[q] = None
        
        return vertex


    def erase_vertex(self, vertex):
        # Ejemplo de 2x2 a 1x1:
        #
        #    0 1  => [0][1] =>  3 X  =>  3
        #    2 3     [2] 3      X X
        #
        #    0 1  =>  0 [1] =>  0 X  =>  0
        #    2 3     [2][3]     X X
        #
        # Ejemplo de 3x3 a 2x2:
        #
        #    0 1 2  => [0][1][2] =>  4 5 7  =>  4 5
        #    3 4 5     [3] 4  5      8 X X      7 8
        #    6 7 8     [6] 7  8      X X X
        #
        #    0 1 2  =>  0 [1] 2  =>  0 2 6  =>  0 2
        #    3 4 5     [3][4][5]     8 X X      6 8
        #    6 7 8      6 [7] 8      X X X
        #
        #    0 1 2  =>  0  1 [2] =>  0 1 3  =>  0 1
        #    3 4 5      3  4 [5]     4 X X      3 4
        #    6 7 8     [6][7][8]     X X X
        #
        assert 0 <= vertex < len(self._vertices)
        q = vertex
        p = vertex + 1
        n = len(self._vertices)
        m = (n - 1) ** 2

        while q < m:
            if p // q ==vertex:
                p += n

            self._edges[q] = self._edges[p]
            p += 1
            q += 1
            if p % n == vertex:
                p += 1

        del self._vertices[vertex]
        del self._edges[m:]


    def __getitem__(self, key):
        if isinstance(key, tuple):
            return self.get_edge(key[0], key[1])
        return self.get_vertex(key)


    def __setitem__(self, key, value): 
        if isinstance(key, tuple):
            return self.set_edge(key[0], key[1], value)
        return self.set_vertex(key, value)


    def __delitem__(self, key):
        if isinstance(key, tuple):
            self.erase_edge(key[0], key[1])
        else:
            self.erase_vertex(key)


    def _get_index(self, from_, to):
        return from_ * len(self._vertices) + to


    def get_edge(self, from_, to):
        return self._edges[self._get_index(from_, to)]


    def set_edge(self, from_, to, weight):
        self._edges[self._get_index(from_, to)] = weight


    def add_edge(self, from_, to, weight):
        self._edges[self._get_index(from_, to)] = weight


    def erase_edge(self, from_, to):
        if self._edges[self._get_index(from_, to)] is not None:
            self._edges[self._get_index(from_, to)] = None
            return True
        return False


    def is_adjacent(self, from_, to):
        return self._edges[self._get_index(from_, to)] is not None


    def adjacents(self, from_):
        result = []
        for i in range(len(self._vertices)):
            if self._edges[self._get_index(from_, i)] is not None:
                result.append(i)
        return result


    def __eq__(self, other):
        return self._vertices == other._vertices and self._edges == other.edges


    def __repr__(self):
        result = '( '
        for v in self.vertices():
            for w in self.adjacents(v):
                result += '{0}-({2})->{1}, '.format(v, w, self.get_edge(v, w))
        return result + ') : CompactAdjacencyMatrixGraph'


###############################################################################


from listas import SinglyLinkedList


class AdjacencyListGraph:
    _Vertex = namedlist('_Vertex', 'value edges')
    _Edge = namedlist('_Edge', 'to weight')

    __slots__ = ['_vertices']


    def __init__(self):
        self._vertices = []


    def copy(self):
        result = AdjacencyListGraph()
        for vertex in self._vertices:
            new_list = SinglyLinkedList()
            p = new_list.before_begin()
            for edge in vertex.edges:
                new_edge = self._Edge(edge.to, edge.weight)
                p = new_list.insert_after(p, new_edge)
            new_vertex = self._Vertex(vertex.value, new_list)
            result._vertices.append(new_vertex)
        return result


    def is_empty(self):
        return len(self._vertices) == 0


    def __len__(self):
        return self._vertices


    def vertices(self):
        return list(range(len(self._vertices)))


    def get_vertex(self, vertex):
        return self._vertices[vertex].value


    def set_vertex(self, vertex, value):
        self._vertices[vertex].value = value


    def add_vertex(self, value):
        vertex = len(self._vertices)
        self._vertices.append(self._Vertex(value, SinglyLinkedList()))
        return vertex


    def erase_vertex(self, vertex):
        assert 0 <= vertex < len(self._vertices)
        del self._vertices[vertex]
        for _, edges in self._vertices:
            prev = edges.before_begin()
            p = prev.next()
            while p != edges.end():
                if p.value.to == vertex:
                    p = edges.erase_after(prev)
                else:
                    if p.value.to > vertex:
                        p.value.to -= 1
                    prev.advance()
                    p.advance()


    def __getitem__(self, key):
        if isinstance(key, tuple):
            return self.get_edge(key[0], key[1])
        return self.get_vertex(key)


    def __setitem__(self, key, value): 
        if isinstance(key, tuple):
            return self.set_edge(key[0], key[1], value)
        return self.set_vertex(key, value)


    def __delitem__(self, key):
        if isinstance(key, tuple):
            self.erase_edge(key[0], key[1])
        else:
            self.erase_vertex(key)


    def get_edge(self, from_, to):
        for edge in self._vertices[from_].edges:
            if edge.to == to:
                return edge.weight
        raise IndexError('No existe esa arista')


    def set_edge(self, from_, to, weight):
        for edge in self._vertices[from_].edges:
            if edge.to == to:
                edge.weight = weight
                return
        raise IndexError('No existe esa arista')


    def add_edge(self, from_, to, weight):
        edges = self._vertices[from_].edges
        prev = edges.before_begin()
        p = prev.next()
        while p != edges.end():
            if p.value.to == to:
                p.value.weight = weight
                return
            elif p.value.to > to:
                break
            prev.advance()
            p.advance()
        edges.insert_after(prev, self._Edge(to, weight))


    def erase_edge(self, from_, to):
        edges = self._vertices[from_].edges
        prev = edges.before_begin()
        p = prev.next()
        while p != edges.end():
            if p.value.to == to:
                edges.erase_after(prev)
                return True
            elif p.value.to > to:
                break;
            prev.advance()
            p.advance()
        return False


    def is_adjacent(self, from_, to):
        for edge in self._vertices[from_].edges:
            if edge.to == to:
                return True
            elif edge.to > to:
                break
        return False


    def adjacents(self, from_):
        result = []
        for to, _ in self._vertices[from_].edges:
            result.append(to)
        return result


    def __eq__(self, other):
        if len(self._vertices) != len(other._vertices):
            return False

        for vx, vy in zip(self._vertices, other._vertices):
            if vx.value != vy.value or vx.edges != vy.edges:
                return False

        return True


    def __repr__(self):
        result = '( '
        for v in self.vertices():
            for w in self.adjacents(v):
                result += '{0}-({2})->{1}, '.format(v, w, self.get_edge(v, w))
        return result + ') : AdjacencyListGraph'


###############################################################################


def recursive_dfs(graph, start):
    visited = set()

    def dfs_from(current):
        visited.add(current)
        yield current, graph.get_vertex(current)
        for vertex in graph.adjacents(current):
            if vertex not in visited:
                yield from dfs_from(vertex)

    yield from dfs_from(start)
    for vertex in graph.vertices():
        if vertex not in visited:
            yield from dfs_from(vertex)



def dfs(graph, start):
    visited = set()
    pending = list()
    vertices = [start] + graph.vertices()

    for vertex in vertices:
        if vertex in visited:
            continue
    
        pending.append(vertex)
        visited.add(vertex)
        while pending:
            current = pending.pop()
            yield current, graph.get_vertex(current)
    
            for neighbor in graph.adjacents(current):
                if neighbor not in visited:
                    pending.append(neighbor)
                    visited.add(neighbor)


def bfs(graph, start):
    from collections import deque

    visited = set()
    pending = deque()
    vertices = [start] + graph.vertices()

    for vertex in vertices:
        if vertex in visited:
            continue
    
        pending.append(vertex)
        visited.add(vertex)
        while pending:
            current = pending.popleft()
            yield current, graph.get_vertex(current)
    
            for adjacent in graph.adjacents(current):
                if adjacent not in visited:
                    pending.append(adjacent)
                    visited.add(adjacent)


def topological_sort(graph):
    from collections import deque, defaultdict

    inputs = defaultdict(int)
    for vertex in graph.vertices():
        for adjacent in graph.adjacents(vertex):
            inputs[adjacent] += 1

    pending = deque()
    for vertex in graph.vertices():
        if inputs[vertex] == 0:
            pending.append(vertex)

    result = []
    while pending:
        vertex = pending.popleft()
        result.append(vertex)

        for adjacent in graph.adjacents(vertex):
            inputs[adjacent] -= 1
            if inputs[adjacent] == 0:
                pending.append(adjacent)

    for i in inputs.values():
        if i != 0:
            raise ValueError('El grafo dado tiene ciclo(s).')

    return result


def shortest_path(graph, start, max_cost=float('inf')):
    Data = namedlist('Data', 'cost visited prev')

    data = dict()
    for vertex in graph.vertices():
        data[vertex] = Data(max_cost, False, start)

    data[start].cost = 0
    for adjacent in graph.adjacents(start):
        data[adjacent].cost = graph.get_edge(start, adjacent)

    while True:
        min_cost = max_cost
        for vertex in graph.vertices():
            cost, visited, _ = data[vertex]
            if not visited and cost < min_cost:
                min_cost = cost
                min_vertex = vertex
        
        if min_cost == max_cost:
            break

        data[min_vertex].visited = True
        accumulated = data[min_vertex].cost
        for adjacent in graph.adjacents(min_vertex):
            entry = data[adjacent]
            total_cost = accumulated + graph.get_edge(min_vertex, adjacent)
            if total_cost < entry.cost:
                entry.cost = total_cost
                entry.prev = min_vertex


    result = dict()
    for vertex in graph.vertices():
        path = []
        current = vertex
        while data[current].visited:
            path.append(current)
            if current == start:
                break
            current = data[current].prev
        result[vertex] = (data[vertex].cost, path[::-1])

    return result


###############################################################################


def crear_conexiones(g):
    g.add_edge(0, 2, 8)
    g.add_edge(0, 3, 2)
    g.add_edge(1, 5, 8)
    g.add_edge(1, 6, 1)
    g.add_edge(3, 1, 1)
    g.add_edge(4, 2, 7)
    g.add_edge(4, 6, 9)
    g.add_edge(4, 7, 6)
    g.add_edge(5, 1, 1)
    g.add_edge(6, 2, 6)
    g.add_edge(6, 4, 7)
    g.add_edge(7, 5, 9)
    g.add_edge(7, 6, 6)


def hacer_recorridos(g):
    print('Orden Topológico:', end=' ')
    try:
        r = topological_sort(g);
        print('(sin ciclos)', r)
    except ValueError:
        print('(con ciclos)')

    g2 = g.copy()
    g2.erase_edge(5, 1)
    g2.erase_edge(5, 1)
    g2.erase_edge(6, 4)
    g2.erase_edge(4, 7)

    def recorridos(g, start):
        print('DFS (iter):', list(dfs(g, start)))
        print('DFS (recu):', list(recursive_dfs(g, start)))
        print('BFS:', list(bfs(g, start)))


    for v in g.vertices():
        print('-- Grafo con ciclos:')
        recorridos(g, v);
        print('\n-- Grafo sin ciclos:')
        recorridos(g2, v)
        print()


def caminos_mas_cortos(g, start):
    caminos = shortest_path(g, start);
    for v, d in caminos.items():
        print('De', start, 'a', v, '=> Costo =', d[0], '- Camino:', d[1])


g1 = AdjacencyMatrixGraph()
for x in 'ABCDEFGH':
    g1.add_vertex(x)
crear_conexiones(g1)
print('g1 =', g1)


g2 = CompactAdjacencyMatrixGraph()
for x in 'ABCDEFGH':
    g2.add_vertex(x)
crear_conexiones(g2)
print('g2 =', g2)


g3 = AdjacencyListGraph()
for x in 'ABCDEFGH':
    g3.add_vertex(x)
crear_conexiones(g3)
print('g3 =', g3)


g4 = CompactAdjacencyMatrixGraph()
g4.add_vertex('A')
g4.add_vertex('B')
g4.add_vertex('C')
g4.add_vertex('D')
g4.add_edge(0, 2, 8)
g4.add_edge(0, 3, 2)
g4.add_edge(3, 1, 1)
g4.add_vertex('E')
g4.add_edge(4, 2, 7)
g4.add_vertex('F')
g4.add_edge(1, 5, 8)
g4.add_edge(5, 1, 1)
g4.add_vertex('G')
g4.add_edge(1, 6, 1)
g4.add_edge(4, 6, 9)
g4.add_edge(6, 2, 6)
g4.add_edge(6, 4, 7)
g4.add_vertex('H')
g4.add_edge(4, 7, 6)
g4.add_edge(7, 5, 9)
g4.add_edge(7, 6, 6)
print('g4 =', g4)

print('\n:: Caminos más cortos:')
caminos_mas_cortos(g4, 0);

print('\n:: Recorridos:')
print('>> Grafo g1:\n')
hacer_recorridos(g1);
print('>> Grafo g2:\n')
hacer_recorridos(g2);
print('>> Grafo g3:\n')
hacer_recorridos(g3);
print('>> Grafo g4:\n')
hacer_recorridos(g4);


g5 = AdjacencyMatrixGraph()
for x in 'ABCDEFGH':
    g5.add_vertex(x)

g5[0, 2] = 8
g5[0, 3] = 2
g5[3, 1] = 1

print('g5 =', g5)
print('g5[0] =', g5[0])
print('g5[0, 2] =', g5[0, 2])

g5[0] = 'Hola'
g5[0, 2] = -5

print('g5 =', g5)
print('g5[0] =', g5[0])
print('g5[0, 2] =', g5[0, 2])
