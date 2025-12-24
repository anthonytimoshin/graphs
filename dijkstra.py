import heapq


class Dijkstra:

    def __init__(self, adjacency_matrix):
        self.adjacency_matrix = adjacency_matrix
        self.num_vertices = len(adjacency_matrix)

    def find_shortest_path(self, start):
        distances = [float('inf')] * self.num_vertices
        distances[start] = 0
        predecessors = [-1] * self.num_vertices

        priority_queue = [(0, start)]
        visited = set()

        while priority_queue:
            current_dist, current = heapq.heappop(priority_queue)

            if current in visited:
                continue

            visited.add(current)

            for neighbor in range(self.num_vertices):
                weight = self.adjacency_matrix[current][neighbor]
                if weight > 0:
                    distance = current_dist + weight

                    if distance < distances[neighbor]:
                        distances[neighbor] = distance
                        predecessors[neighbor] = current
                        heapq.heappush(priority_queue, (distance, neighbor))

        return distances, predecessors

    def reconstruct_path(self, predecessors, start, end):
        if predecessors[end] == -1 and start != end:
            return None

        path = []
        current = end
        while current != -1:
            path.append(current)
            if current == start:
                break
            current = predecessors[current]

        path.reverse()
        return path

    @staticmethod
    def format_results(start, distances, predecessors, num_vertices):
        result = []
        result.append(f"Алгоритм Дейкстры")
        result.append(f"Начальная вершина: {start}")
        result.append("")

        # Показываем расстояния до нескольких вершин
        result.append("Расстояния до некоторых вершин:")
        for vertex in range(min(10, num_vertices)):  # Показываем первые 10
            if vertex != start:
                if distances[vertex] == float('inf'):
                    result.append(f"  До вершины {vertex}: недостижима")
                else:
                    result.append(f"  До вершины {vertex}: {distances[vertex]:.2f}")

        if num_vertices > 10:
            result.append(f"  ... и ещё {num_vertices - 10} вершин")

        return "\n".join(result)
