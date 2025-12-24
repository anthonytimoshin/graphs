class Graph:

    def __init__(self, adjacency_matrix=None, adjacency_list=None):
        self.adjacency_matrix = adjacency_matrix
        self.adjacency_list = adjacency_list
        self.num_vertices = len(adjacency_matrix) if adjacency_matrix else 0

    @classmethod
    def from_adjacency_matrix(cls, matrix):
        n = len(matrix)
        adjacency_list = {i: [] for i in range(n)}

        for i in range(n):
            for j in range(n):
                if matrix[i][j] > 0:
                    adjacency_list[i].append((j, matrix[i][j]))

        return cls(adjacency_matrix=matrix, adjacency_list=adjacency_list)

    def get_neighbors(self, vertex):
        if self.adjacency_list:
            return self.adjacency_list.get(vertex, [])
        return []

    def get_edge_weight(self, from_vertex, to_vertex):
        if self.adjacency_matrix:
            return self.adjacency_matrix[from_vertex][to_vertex]
        return float('inf')


class Edge:

    def __init__(self, from_vertex, to_vertex, weight, capacity=None):
        self.from_vertex = from_vertex
        self.to_vertex = to_vertex
        self.weight = weight
        self.capacity = capacity

    def __repr__(self):
        return f"Edge({self.from_vertex} -> {self.to_vertex}, weight={self.weight})"
