#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def find(first, last, value):
	while first != last:
		if first.value == value:
			return first
		first = first.next()
	return last

