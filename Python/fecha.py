def días_en_mes(año, mes):
    meses = [-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    if mes == 2 and es_año_bisiesto(año):
        return 29
    return meses[mes]


def es_año_bisiesto(año):
    return (año % 4 == 0) and (año % 100 != 0 or año % 400 == 0)


class Fecha():
    __slots__ = ['_día', '_mes', '_año']

    def __init__(self, año, mes, día):
        if año <= 0:
            raise ValueError('Año no válido')

        if mes < 1 or mes > 12:
            raise ValueError('Mes no válido')

        if día < 1 or día > días_en_mes(año, mes):
            raise ValueError('Día no válido')

        self._año = año
        self._mes = mes
        self._día = día


    def siguiente(self):
        año = self._año
        mes = self._mes
        día = self._día + 1
        if día > días_en_mes(año, mes):
            día = 1
            mes += 1
            if mes > 12:
                mes = 1
                año += 1
        return Fecha(año, mes, día)


    def anterior(self):
        año = self._año
        mes = self._mes
        día = self._día - 1
        if día < 1:
            mes -= 1
            if mes < 1:
                mes = 12
                año -= 1
            día = días_en_mes(año, mes)
        return Fecha(año, mes, día)


    def __repr__(self):
        return 'Fecha({}, {}, {})'.format(self.año, self.mes, self.día)


    @property
    def día(self):
        return self._día
        

    @property
    def mes(self):
        return self._mes

        
    @property
    def año(self):
        return self._año


    def __eq__(self, other):  # f1 == f2
        return (self.año == other.año and
                self.mes == other.mes and
                self.día == other.día)


    def __lt__(self, other):  # f1 < f2
        if self.año < other.año:
            return True
        elif self.año == other.año:
            if self.mes < other.mes:
                return True
            elif self.mes == other.mes:
                if self.día < other.día:
                    return True
        return False


    def __le__(self, other):  # f1 <= f2
        return (self < other) or (self == other)

