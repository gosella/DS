##############################################################################

class RangoIter():
    def __init__(self, rango):
        self.pos = rango.start
        self.end = rango.stop

    def __iter__(self):
        return self

    def __next__(self):
        if self.pos == self.end:
            raise StopIteration
        n = self.pos
        self.pos += 1
        return n


class Rango():
    def __init__(self, start, stop=None):
        if stop is None:
            stop = start
            start = 0
        self.start = start
        self.stop = stop

    def __repr__(self):
        return 'Rango(start={}, stop={})'.format(self.start, self.stop)

    def __iter__(self):
        return RangoIter(self)


##############################################################################

class Rango():
    def __init__(self, start, stop=None):
        if stop is None:
            stop = start
            start = 0
        self.start = start
        self.stop = stop

    def __repr__(self):
        return 'Rango(start={}, stop={})'.format(self.start, self.stop)

    def __iter__(self):
        n = self.start
        while n < self.stop:
            yield n
            n += 1

##############################################################################
