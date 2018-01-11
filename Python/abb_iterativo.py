#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from namedlist import namedlist

###############################################################################


def _minimum_node(node):
    while node.left is not None:
        node = node.left
    return node


def _maximum_node(node):
    while node.right is not None:
        node = node.right
    return node


class TreeDict():
    _Node = namedlist('_Node', 'key value parent left right', default=None)
    _Root = namedlist('_Root', 'left right parent', default=None)

    __slots__ = ['_root', '_len']


    def __init__(self, iterable=None):
        self._root = self._Root(None, None)
        self._len = 0
        
        if iterable is not None:
            raise NotImplemented


    def is_empty(self):
        return self._root.left is None


    def __len__(self):
        return self._len


    def begin(self):
        return TreeDict._Coordinate(_minimum_node(self._root))


    def end(self):
        return TreeDict._Coordinate(self._root)


    def find(self, key):
        def find(node):
            if node is None:
                return self.end()
            elif key < node.key:
                return find(node.left)
            elif key > node.key:
                return find(node.right)
            else:  # key == node.key
                return TreeDict._Coordinate(node)

        return find(self._root.left)


    def minimum(self):
        return TreeDict._Coordinate(_minimum_node(self._root))


    def maximum(self):
        return TreeDict._Coordinate(_maximum_node(self._root))


    def insert_iterativo(self, key, value):
        self._len += 1
        node = self._root.left
        if node is None:
            node = self._Node(key, value, self._root)
            self._root.left = node
        else:
            searching = True
            while searching:
                if key < node.key:
                    if node.left is None:
                        node.left = self._Node(key, value, node)
                        searching = False
                    node = node.left
                elif key > node.key:
                    if node.right is None:
                        node.right = self._Node(key, value, node)
                        searching = False
                    node = node.right
                else:  # key == node.key
                    node.value = value
                    self._len -= 1
                    searching = False
        return TreeDict._Coordinate(node)


    def insert(self, key, value):
        def insert(node, parent):
            if node is None:
                node = self._Node(key, value, parent)
                coord = TreeDict._Coordinate(node)
            elif key < node.key:
                node.left, coord = insert(node.left, node)
            elif key > node.key:
                node.right, coord = insert(node.right, node)
            else:  # key == node.key
                node.value = value
            return node, coord
        
        self._root.left, coord = insert(self._root.left, self._root)
        return coord



    # def erase(self, key):
    #     node = self._root.left
    #     while node is not None:
  

    # key in map  -->  map.__contains__(key)
    def __contains__(self, key):
        return self.find(key) != self.end()


    # map[key]  -->  map.__getitem__(key)
    def __getitem__(self, key):
        p = self.find(key)
        if p == self.end():
            raise KeyError(key)
        return p.value


    # map[key] = value -->  map.__setitem__(key, value)
    def __setitem__(self, key, value): 
        self.insert(key, value)

        # def insert(node, parent):
        #     if node is None:
        #         return self._Node(key, value, parent)
        #     if key < node.key:
        #         node.left = insert(node.left, node)
        #     elif key > node.key:
        #         node.right = insert(node.right, node)
        #     else:  # key == node.key
        #         node.value = value
        #     return node
        #
        # self._root.left = insert(self._root.left, self._root)


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
            node = self._node
            if node.right is not None:
                node = _minimum_node(node.right)
            else:
                while node.parent is not None and node.parent.right is node:
                    node = node.parent
            self._node = node
            return self

        def next(self):
            return TreeDict._Coordinate(self._node).advance()

        def retreat(self):
            node = self._node
            if node.left is not None:
                node = _maximum_node(node.left)
            else:
                while node.parent is not None and node.parent.left is node:
                    node = node.parent
            self._node = node
            return self
           
        def prev(self):
            return TreeDict._Coordinate(self._node).retreat()

        def __eq__(self, other):
            return self._node is other._node

###############################################################################
