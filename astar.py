import heapq
from heuristics import Heuristics


class AStar:

    def __init__(self, adjacency_matrix, coordinates, heuristic_type='euclidean'):
        self.adjacency_matrix = adjacency_matrix
        self.coordinates = coordinates
        self.num_vertices = len(adjacency_matrix)
        self.heuristic_type = heuristic_type

        if heuristic_type == 'euclidean':
            self.heuristic = Heuristics.euclidean
        elif heuristic_type == 'manhattan':
            self.heuristic = Heuristics.manhattan
        else:
            raise ValueError("Unknown heuristic type")

    def find_path(self, start, goal):
        import time
        start_time = time.perf_counter()
        open_set = []
        heapq.heappush(open_set, (0, start))

        came_from = {}
        g_score = {i: float('inf') for i in range(self.num_vertices)}
        g_score[start] = 0

        f_score = {i: float('inf') for i in range(self.num_vertices)}
        f_score[start] = self.heuristic(self.coordinates[start], self.coordinates[goal])

        visited_count = 0
        closed_set = set()

        while open_set:
            current_f, current = heapq.heappop(open_set)

            if current in closed_set:
                continue

            closed_set.add(current)
            visited_count += 1

            if current == goal:
                path = self.reconstruct_path(came_from, current)
                execution_time = time.time() - start_time
                return path, g_score[goal], visited_count, execution_time

            for neighbor in range(self.num_vertices):
                if self.adjacency_matrix[current][neighbor] > 0:
                    if neighbor in closed_set:
                        continue

                    tentative_g_score = g_score[current] + self.adjacency_matrix[current][neighbor]

                    if tentative_g_score < g_score[neighbor]:
                        came_from[neighbor] = current
                        g_score[neighbor] = tentative_g_score
                        f_score[neighbor] = g_score[neighbor] + self.heuristic(
                            self.coordinates[neighbor],
                            self.coordinates[goal]
                        )
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))

        execution_time = time.perf_counter() - start_time
        return None, float('inf'), visited_count, execution_time

    def reconstruct_path(self, came_from, current):
        path = [current]
        while current in came_from:
            current = came_from[current]
            path.append(current)
        path.reverse()
        return path

    @staticmethod
    def format_results(start, goal, path, length, visited_count, execution_time, heuristic_type):
        result = []
        result.append(f"Алгоритм: A* с {heuristic_type.lower()} эвристикой")
        result.append(f"Начальная точка: {start}")
        result.append(f"Конечная точка: {goal}")
        result.append("")

        if path:
            result.append(f"Длина найденного пути: {length:.2f}")
            result.append(f"Количество вершин в пути: {len(path)}")
            
            # Для больших путей показываем только начало и конец
            if len(path) > 10:
                result.append(f"Путь (сокращенно): {path[0]} -> ... -> {path[-1]}")
                result.append(f"Полный путь: {' -> '.join(map(str, path))}")
            else:
                result.append(f"Путь: {' -> '.join(map(str, path))}")
            
            result.append(f"Процент просмотренных вершин: {(visited_count / len(path) * 100):.1f}%")
            result.append(f"Время выполнения: {execution_time * 1_000_000:.2f} микросекунд")
        else:
            result.append("Путь не найден")

        return "\n".join(result)

