#!/usr/bin/env python3
# -*- coding: utf-8 -*-

class Stack():
    __slots__ = ['_values']

    def __init__(self, iterable=None):
        self._values = []
        if iterable is not None:
            for value in iterable:
                self.push(value)


    def is_empty(self):
        return len(self._values) == 0


    @property
    def top(self):
        assert not self.is_empty(), 'No se puede operar con una pila vacía'
        return self._values[-1]


    def clear(self):
        self._values.clear()


    def push(self, value):
        self._values.append(value)


    def pop(self):
        assert not self.is_empty(), 'No se puede operar con una pila vacía'
        return self._values.pop()


    def copy(self):
        new_stack = Stack()
        new_stack._values = self._values.copy()
        return new_stack


    def __len__(self):
        return len(self._values)


    def __eq__(self, other):
        return self._values == other._values


    def __repr__(self):
        return 'Stack([' + ', '.join(repr(x) for x in self._values) + '])'
