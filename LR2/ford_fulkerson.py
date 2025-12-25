from collections import deque


class FordFulkerson:

    def __init__(self, graph):
        self.graph = [row[:] for row in graph]
        self.num_vertices = len(graph)
        self.path_history = []

    def bfs(self, source, sink, parent):
        visited = [False] * self.num_vertices
        queue = deque([source])
        visited[source] = True

        while queue:
            u = queue.popleft()

            for v in range(self.num_vertices):
                if not visited[v] and self.graph[u][v] > 0:
                    visited[v] = True
                    parent[v] = u
                    queue.append(v)

                    if v == sink:
                        return True
        return False

    def find_max_flow(self, source, sink):
        # Сохраняем оригинальный граф
        original_graph = [row[:] for row in self.graph]
        
        parent = [-1] * self.num_vertices
        max_flow = 0
        paths = []

        while self.bfs(source, sink, parent):
            path_flow = float('inf')
            s = sink
            path = []

            while s != source:
                path_flow = min(path_flow, self.graph[parent[s]][s])
                path.append((parent[s], s))
                s = parent[s]

            path.reverse()
            paths.append((path, path_flow))
            max_flow += path_flow

            # Обновляем остаточную сеть
            v = sink
            while v != source:
                u = parent[v]
                self.graph[u][v] -= path_flow
                self.graph[v][u] += path_flow
                v = parent[v]

            parent = [-1] * self.num_vertices

        return max_flow, paths, original_graph

    @staticmethod
    def format_results(source, sink, max_flow, paths, original_graph=None):
        result = []
        result.append(f"Maximum flow value: {max_flow}.")
        
        # Переводим к 1-индексации для вывода
        source_1idx = source + 1
        sink_1idx = sink + 1
        result.append(f"Source: {source_1idx}, sink: {sink_1idx}.")
        result.append("Flow:")
        
        # Создаем матрицу потоков
        flow_matrix = [[0] * len(original_graph) for _ in range(len(original_graph))]
        
        for path, flow in paths:
            for u, v in path:
                flow_matrix[u][v] += flow
        
        # Выводим только те рёбра, где есть поток
        for i in range(len(original_graph)):
            for j in range(len(original_graph)):
                if flow_matrix[i][j] > 0:
                    # Переводим к 1-индексации
                    u = i + 1
                    v = j + 1
                    result.append(f"{u}-{v} : {flow_matrix[i][j]}")
        
        return "\n".join(result)
