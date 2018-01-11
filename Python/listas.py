#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from namedlist import namedlist

###############################################################################

class DoublyLinkedList():
    _Node = namedlist('Node', 'value prev next', default=None)
    _Head = namedlist('Head', 'prev next', default=None)

    __slots__ = ['_head']

    def __init__(self, iterable=None):
        self._head = DoublyLinkedList._Head()
        self._head.prev = self._head.next = self._head
        if iterable is not None:
            for value in iterable:
                self.append_back(value)


    def __repr__(self):
        return 'DoublyLinkedList([' + ', '.join(repr(v) for v in self) + '])'


    def is_empty(self):
        return self._head.next is self._head


    def clear(self):
        self._head.prev = self._head.next = self._head


    def __len__(self):
        n = 0
        for x in self:  # Usando abstracciones
            n += 1

        # Equivalente al for anterior:
        # nodo = self._head.next
        # while nodo is not self._head:
        #     n += 1
        #     nodo = nodo.next
        return n


    def __iter__(self):
        # Usando abstracciones
        p = self.begin()
        while p != self.end():
            yield p.value
            p.advance()

        # Otra opción más "a mano"...
        # nodo = self._head.next
        # while nodo != self._head:
        #     yield nodo.value
        #     nodo = nodo.next


    def __eq__(self, other):
        p = self.begin()
        q = other.begin()
        while p != self.end() and q != other.end():
            if p.value != q.value:
                return False
            p.advance()
            q.advance()
        return p == self.end() and q == other.end()


    def begin(self):
        return DoublyLinkedList._Coordinate(self._head.next)


    def end(self):
        return DoublyLinkedList._Coordinate(self._head)


    def insert(self, coord, value):
        current = coord._node
        new_node = DoublyLinkedList._Node(value)
        new_node.prev = current.prev
        new_node.next = current
        new_node.prev.next = new_node
        new_node.next.prev = new_node
        return DoublyLinkedList._Coordinate(current)


    def append_front(self, value):
        self.insert(self.begin(), value)


    def append_back(self, value):
        self.insert(self.end(), value)


    def erase(self, coord):
        current = coord._node
        current.prev.next = current.next
        current.next.prev = current.prev
        return coord.next()


    def copy(self):
        new_list = DoublyLinkedList()
        prev = new_list._head
        for value in self:
            node = DoublyLinkedList._Node(value)
            node.prev = prev
            prev.next = node
            prev = node
        new_list._head.prev = prev
        prev.next = new_list._head
        return new_list


    class _Coordinate():
        __slots__ = ['_node']

        def __init__(self, node=None):
            self._node = node

        @property
        def value(self):
            return self._node.value

        @value.setter
        def value(self, value):
            self._node.value = value

        def advance(self):
            self._node = self._node.next
            return self

        def next(self):
            return DoublyLinkedList._Coordinate(self._node).advance()

        def retreat(self):
            self._node = self._node.prev
            return self
           
        def prev(self):
            return DoublyLinkedList._Coordinate(self._node).retreat()

        def __eq__(self, other):
            return self._node is other._node

###############################################################################

class SinglyLinkedList():
    _Node = namedlist('Node', 'value next', default=None)
    _Head = namedlist('Head', 'next', default=None)

    __slots__ = ['_head']

    def __init__(self, iterable=None):
        self._head = SinglyLinkedList._Head(None)
        if iterable is not None:
            prev = self._head
            for value in iterable:
                node = SinglyLinkedList._Node(value, None)
                prev.next = node
                prev = node


    def __repr__(self):
        return 'SinglyLinkedList([' + ', '.join(repr(v) for v in self) + '])'


    def is_empty(self):
        return self._head.next is None


    def clear(self):
        self._head.next = None
 

    def __len__(self):
        n = 0
        node = self._head
        while node.next is not None:
            n += 1
            node = node.next
        return n


    def __iter__(self):
        # Usando abstracciones
        p = self.begin()
        while p != self.end():
            yield p.value
            p.advance()

        # Otra opción más "a mano"...
        # nodo = self._head.next
        # while nodo is not None:
        #     yield nodo.value
        #     nodo = nodo.next


    def __eq__(self, other):
        p = self.begin()
        q = other.begin()
        while p != self.end() and q != other.end():
            if p.value != q.value:
                return False
            p.advance()
            q.advance()
        return p == self.end() and q == other.end()


    def begin(self):
        return SinglyLinkedList._Coordinate(self._head.next)


    def end(self):
        return SinglyLinkedList._Coordinate(None)


    def before_begin(self):
        return SinglyLinkedList._Coordinate(self._head)


    def insert_after(self, coord, value):
        current = coord._node
        new_node = SinglyLinkedList._Node(value=value, next=current.next)
        current.next = new_node
        return SinglyLinkedList._Coordinate(new_node)


    def append_front(self, value):
        self.insert_after(self.before_begin(), value)


    def erase_after(self, coord):
        current = coord._node
        current.next = current.next.next
        return coord.next()


    def copy(self):
        new_list = SinglyLinkedList()
        prev = new_list._head
        for value in self:
            node = SinglyLinkedList._Node(value)
            prev.next = node
            prev = node
        return new_list


    class _Coordinate():
        __slots__ = ['_node']

        def __init__(self, node=None):
            self._node = node

        @property
        def value(self):
            return self._node.value

        @value.setter
        def value(self, value):
            self._node.value = value

        def advance(self):
            self._node = self._node.next
            return self
            
        def next(self):
            c = SinglyLinkedList._Coordinate(self._node)
            return c.advance()

        def __eq__(self, other):
            return self._node is other._node

###############################################################################
