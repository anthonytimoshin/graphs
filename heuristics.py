import math

class Heuristics:

    @staticmethod
    def euclidean(coord1, coord2):
        x1, y1 = coord1
        x2, y2 = coord2
        return math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)

    @staticmethod
    def manhattan(coord1, coord2):
        x1, y1 = coord1
        x2, y2 = coord2
        return abs(x2 - x1) + abs(y2 - y1)
