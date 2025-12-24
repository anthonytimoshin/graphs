from collections import deque


class BipartiteGraph:

    def __init__(self, adjacency_matrix):
        self.adjacency_matrix = adjacency_matrix
        self.num_vertices = len(adjacency_matrix)

    def is_bipartite(self):
        color = [-1] * self.num_vertices
        partitions = [[], []]

        for start in range(self.num_vertices):
            if color[start] == -1:
                queue = deque([start])
                color[start] = 0

                while queue:
                    u = queue.popleft()

                    for v in range(self.num_vertices):
                        if self.adjacency_matrix[u][v] > 0 or self.adjacency_matrix[v][u] > 0:
                            if color[v] == -1:
                                color[v] = 1 - color[u]
                                queue.append(v)
                            elif color[v] == color[u]:
                                return False, None

        for i in range(self.num_vertices):
            partitions[color[i]].append(i)

        return True, partitions

    def kuhn_algorithm(self):
        is_bip, partitions = self.is_bipartite()
        if not is_bip:
            return 0, []

        left_part = partitions[0]
        right_part = partitions[1]

        match = {v: -1 for v in right_part}

        def try_kuhn(v, used):
            for u in right_part:
                if (self.adjacency_matrix[v][u] > 0) and u not in used:
                    used.add(u)
                    if match[u] == -1 or try_kuhn(match[u], used):
                        match[u] = v
                        return True
            return False

        matching_size = 0
        for v in left_part:
            used = set()
            if try_kuhn(v, used):
                matching_size += 1

        matching_edges = [(match[u], u) for u in right_part if match[u] != -1]

        return matching_size, matching_edges

    @staticmethod
    def format_results(is_bipartite, partitions, matching_size, matching_edges):
        result = []

        if is_bipartite:
            result.append("Граф является двудольным: ДА")
            result.append("")
            result.append(f"Число паросочетаний (для двудольного графа): {matching_size}")
            result.append(f"Размер максимального паросочетания (для двудольного графа): {matching_size}")
            result.append("")
            result.append("Рёбра, входящие в паросочетание:")
            for u, v in matching_edges:
                result.append(f"  ({u}, {v})")
        else:
            result.append("Граф является двудольным: НЕТ")

        return "\n".join(result)
