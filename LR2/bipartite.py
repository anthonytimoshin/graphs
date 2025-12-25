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
                        if self.adjacency_matrix[u][v] > 0:
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
        
        # Сопоставляем исходные номера вершин с новыми индексами для алгоритма Куна
        left_idx = {v: i for i, v in enumerate(left_part)}
        right_idx = {v: i for i, v in enumerate(right_part)}
        
        # Создаем матрицу смежности между долями
        n_left = len(left_part)
        n_right = len(right_part)
        bipartite_matrix = [[0] * n_right for _ in range(n_left)]
        
        for i, u in enumerate(left_part):
            for j, v in enumerate(right_part):
                if self.adjacency_matrix[u][v] > 0:
                    bipartite_matrix[i][j] = 1
        
        # Алгоритм Куна
        match_right = [-1] * n_right
        match_left = [-1] * n_left
        
        def dfs(v, visited):
            for u in range(n_right):
                if bipartite_matrix[v][u] and not visited[u]:
                    visited[u] = True
                    if match_right[u] == -1 or dfs(match_right[u], visited):
                        match_right[u] = v
                        match_left[v] = u
                        return True
            return False
        
        matching_size = 0
        for v in range(n_left):
            visited = [False] * n_right
            if dfs(v, visited):
                matching_size += 1
        
        # Восстанавливаем исходные номера вершин
        matching_edges = []
        for u_idx, v_idx in enumerate(match_left):
            if v_idx != -1:
                u = left_part[u_idx]
                v = right_part[v_idx]
                matching_edges.append((u, v))

        return matching_size, matching_edges

    @staticmethod
    def format_results(is_bipartite, partitions, matching_size, matching_edges):
        result = []

        if is_bipartite:
            result.append("Size of maximum matching: {}.".format(matching_size))
            result.append("Maximum matching:")
            
            # Сортируем рёбра и конвертируем индексы в 1-индексацию
            formatted_edges = []
            for u, v in matching_edges:
                formatted_edges.append("({}, {})".format(u + 1, v + 1))
            
            if formatted_edges:
                result.append("{{" + ", ".join(formatted_edges) + ", }")
            else:
                result.append("{{}}")
        else:
            result.append("Graph is not bipartite")

        return "\n".join(result)
