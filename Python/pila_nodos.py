#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from collections import namedtuple

class Stack():
    _Node = namedtuple('Node', 'value prev')

    __slots__ = ['_top']

    def __init__(self, iterable=None):
        self._top = None
        if iterable is not None:
            for value in iterable:
                self.push(value)


    def is_empty(self):
        return self._top is None


    @property
    def top(self):
        assert not self.is_empty(), 'No se puede operar con una pila vacía'
        return self._top.value


    def clear(self):
        self._top = None


    def push(self, value):
        self._top = Stack._Node(value, self._top)


    def pop(self):
        assert not self.is_empty(), 'No se puede operar con una pila vacía'
        value = self._top.value
        self._top = self._top.prev
        return value


    def copy(self):
        new_stack = Stack()
        new_stack._top = self._top
        return new_stack


    def __len__(self):
        n = 0
        node = self._top
        while node is not None:
            node = node.prev
            n += 1
        return n


    def __eq__(self, other):
        x = self._top
        y = other._top
        while x is not None and y is not None:
            if x.value != y.value:
                return False
            x = x.prev
            y = y.prev
        return x is None and y is None
        # O simplemente hacer:
        # return self._top == other._top


    def __repr__(self):
        values = []
        node = self._top
        while node is not None:
            values.insert(0, node.value)
            node = node.prev
        return 'Stack([' + ', '.join(repr(x) for x in values) + '])'
