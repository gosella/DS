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
    _Node = namedlist('_Node', 'key value height parent left right', default=None)
    _Root = namedlist('_Root', 'left right parent', default=None)

    __slots__ = ['_root', '_len']


    def __init__(self, iterable=None):
        self._root = self._Root()
        self._len = 0
        if iterable is not None:
            for key, value in iterable:
                self.insert(key, value)


    def is_empty(self):
        return self._root.left is None


    def __len__(self):
        return self._len


    def begin(self):
        return TreeDict._Coordinate(_minimum_node(self._root))


    def end(self):
        return TreeDict._Coordinate(self._root)


    def find(self, key):
        node = self._root.left
        while node is not None:
            if key < node.key:
                node = node.left
            elif key > node.key:
                node = node.right
            else:  # key == node.key
                return TreeDict._Coordinate(node)
        return self.end()


    def minimum(self):
        return TreeDict._Coordinate(_minimum_node(self._root))


    def maximum(self):
        return TreeDict._Coordinate(_maximum_node(self._root))


    def insert(self, key, value=None):
        def do_insert(node, parent):
            if node is None:
                node = self._Node(key, value, 1, parent)
                coord = TreeDict._Coordinate(node)
                self._len += 1
            elif key == node.key:
                node.value = value
                coord = TreeDict._Coordinate(node)
            else:
                if key < node.key:
                    node.left, coord = do_insert(node.left, node)
                else:  # key > node.key:
                    node.right, coord = do_insert(node.right, node)
                node = self._balance_tree(node)
            return node, coord

        
        self._root.left, coord = do_insert(self._root.left, self._root)
        return coord


    def _balance_tree(self, root):
        def balance_factor(node):
            factor = 0
            if node is not None:
                if node.left is not None:
                    factor += node.left.height
                if node.right is not None:
                    factor -= node.right.height
            return factor


        def rotate_left(root):
            right_tree = root.right
            root.right = right_tree.left
            self._assign_parent(root.right, root)
            right_tree.left = root
            right_tree.parent = root.parent
            root.parent = right_tree
            root = right_tree
            self._update_height(root.left)
            self._update_height(root)
            return root


        def rotate_right(root):
            left_tree = root.left
            root.left = left_tree.right
            self._assign_parent(root.left, root)
            left_tree.right = root
            left_tree.parent = root.parent
            root.parent = left_tree
            root = left_tree
            self._update_height(root.right)
            self._update_height(root)
            return root


        bf = balance_factor(root)
        if bf == 2:
            if balance_factor(root.left) == -1:
                root.left = rotate_left(root.left)
            root = rotate_right(root)
        elif bf == -2:
            if balance_factor(root.right) == 1:
                root.right = rotate_right(root.right)
            root = rotate_left(root)
        else:
            self._update_height(root)
        return root


    def _assign_parent(self, node, parent):
        if node is not None:
            node.parent = parent


    def _update_height(self, node):
        left_height = 0
        if node.left is not None:
            left_height = node.left.height
        right_height = 0
        if node.right is not None:
            right_height = node.right.height
        node.height = 1 + max(left_height, right_height)


    def erase(self, key):
        def do_erase(node):
            if node is None:
                erased = False
                coord = self.end()
            elif key == node.key:
                erased = True
                coord = TreeDict._Coordinate(node).advance()
                node = erase_node(node)
            else:
                if key < node.key:
                    erased, node.left, coord = do_erase(node.left)
                else:  # key > node.key:
                    erased, node.right, coord = do_erase(node.right)
                if erased:
                    node = self._balance_tree(node)
            return erased, node, coord


        def erase_node(node):
            parent = node.parent
            if node.left is None:
                node = node.right
            elif node.right is None:
                node = node.left
            else:
                node = extract_maximum_from(node)
                node = self._balance_tree(node)
            self._assign_parent(node, parent)
            self._len -= 1
            return node


        def extract_maximum_from(root):
            def do_extract(node):
                if node.right is None:
                    maximum = node
                    self._assign_parent(maximum.left, maximum.parent)
                    node = maximum.left
                    return maximum, node
                else:
                    maximum, node.right = do_extract(node.right)
                    self._update_height(node)
                    return maximum, node

            maximum, root.left = do_extract(root.left)
            maximum.left = root.left
            self._assign_parent(maximum.left, maximum)
            maximum.right = root.right
            self._assign_parent(maximum.right, maximum)
            maximum.parent = root.parent
            return maximum


        result, self._root.left, coord = do_erase(self._root.left)
        return result, coord
  

    def clear(self):
        self._root.left = None
        self._len = 0


    def copy(self):
        def do_copy(node, parent):
            if node is None:
                new_node = None
            else:
                new_node = TreeDict._Node(node.key, node.value, node.height, parent)
                new_node.left = do_copy(node.left, new_node)
                new_node.right = do_copy(node.right, new_node)
            return new_node

        new_tree = TreeDict()
        new_tree._root.left = do_copy(self._root.left, new_tree._root)
        new_tree._len = self._len
        return new_tree


    class _Coordinate():
        __slots__ = ['_node']

        def __init__(self, node=None):
            self._node = node

        @property
        def key(self):
            return self._node.key

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
                while node.parent is not None:
                    prev = node
                    node = node.parent
                    if node.right is not prev:
                        break
            self._node = node
            return self

        def next(self):
            return TreeDict._Coordinate(self._node).advance()

        def retreat(self):
            node = self._node
            if node.left is not None:
                node = _maximum_node(node.left)
            else:
                while node.parent is not None:
                    prev = node
                    node = node.parent
                    if node.left is not prev:
                        break
            self._node = node
            return self
           
        def prev(self):
            return TreeDict._Coordinate(self._node).retreat()

        def __eq__(self, other):
            return self._node is other._node

        def __repr__(self):
            if hasattr(self._node, 'key'):
                return 'Coordinate({{key: {}, value: {}}})'.format(
                    self._node.key, self._node.value)
            else:
                return 'end()'


    # key in dict  -->  dict.__contains__(key)
    def __contains__(self, key):
        return self.find(key) != self.end()


    # dict[key]  -->  dict.__getitem__(key)
    def __getitem__(self, key):
        p = self.find(key)
        if p == self.end():
            raise KeyError(key)
        return p.value


    # dict[key] = value -->  dict.__setitem__(key, value)
    def __setitem__(self, key, value): 
        self.insert(key, value)


    # del dict[key] -->  dict.__delitem__(key)
    def __delitem__(self, key):
        success, _ = self.erase(key)
        if not success:
            raise KeyError(key)


    def __eq__(self, other):
        p, p_end = self.begin(), self.end()
        q, q_end = other.begin(), other.end()
        while p != p_end and q != q_end:
            if p.key != q.key or p.value != q.value:
                return False
            p.advance()
            q.advance()
        return p == p_end and q == q_end


    def items(self):
        pos = self.begin()
        end = self.end()
        while pos != end:
            yield pos.key, pos.value
            pos.advance()


    def keys(self):
        for key, _ in self.items():
            yield key


    def values(self):
        for _, value in self.items():
            yield value


    def __iter__(self):
        return self.keys()


    def __repr__(self):
        return 'TreeDict([' + ', '.join(repr(x) for x in self.items()) + '])'


    def __str__(self):
        def calculate_placement(node, level):
            if node is None:
                return 0
                
            nonlocal count
            m1 = calculate_placement(node.left, level + 1)
            placements.append((level, count, node))
            count += 1
            m3 = calculate_placement(node.right, level + 1)
            return max(m1, len(str(node.key)), m3)

        count = 0
        placements = []
        key_len = calculate_placement(self._root.left, 0) + 2

        lines = []
        prev_level = -1
        for level, pos, node in placements:
            i = 2 * level
            while len(lines) <= i:
                lines.append('')

            skip = ' ' * (pos * key_len - len(lines[i]))
            lines[i] += skip + '[{:^{}}]'.format(node.key, key_len - 2)

            if prev_level != -1:
                if prev_level < level:
                    i = 2 * prev_level + 1
                    skip = ' ' * (pos * key_len - len(lines[i]))
                    c = '\\'
                else:
                    i = 2 * level + 1
                    skip = ' ' * (pos * key_len - len(lines[i]) - 1)
                    c = '/'

                lines[i] += skip + '{:>{}}'.format(c,  key_len // 2)

            prev_level = level

        return '\n'.join(lines)

###############################################################################

def verify(tree):
    def do_verify(node):
        if node is None:
            return True, 0
        if node.left is not None:
            if node.left.key > node.key or node.left.parent is not node:
                return False, 0
        if node.right is not None:
            if node.right.key < node.key or node.right.parent is not node:
                return False, 0
        result_left, height_left = do_verify(node.left)
        result_right, height_right = do_verify(node.right)
        if not (result_left and result_right):
            return False, 0
        height = 1 + max(height_left, height_right)
        if node.height != height:
            return False, 0
        factor = height_left - height_right
        if factor < -1 or factor > 1:
            return False, 0
        return True, height
    return do_verify(tree._root.left)


def fill(tree, count=7):
    import random
    tree.clear()
    for x in random.sample(range(count), count):
        tree.insert(chr(65 + x))
    return tree

###############################################################################
