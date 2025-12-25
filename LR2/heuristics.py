import math

class Heuristics:

    @staticmethod
    def euclidean(coord1, coord2):
        # coord1 и coord2 теперь: (x, y, height)
        x1, y1, h1 = coord1
        x2, y2, h2 = coord2
        # Используем только координаты x, y для евклидова расстояния
        return math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)

    @staticmethod
    def manhattan(coord1, coord2):
        # coord1 и coord2 теперь: (x, y, height)
        x1, y1, h1 = coord1
        x2, y2, h2 = coord2
        # Используем только координаты x, y для манхэттенского расстояния
        return abs(x2 - x1) + abs(y2 - y1)

    @staticmethod
    def chebyshev(coord1, coord2):
        # coord1 и coord2 теперь: (x, y, height)
        x1, y1, h1 = coord1
        x2, y2, h2 = coord2
        # Используем только координаты x, y для расстояния Чебышева
        return max(abs(x2 - x1), abs(y2 - y1))
