import heapq
import time
from heuristics import Heuristics


class AStar:

    def __init__(self, adjacency_matrix, coordinates, heuristic_type='euclidean'):
        self.adjacency_matrix = adjacency_matrix
        self.coordinates = coordinates
        self.num_vertices = len(adjacency_matrix)
        
        # Выбираем эвристику
        if heuristic_type == 'euclidean':
            self.heuristic = Heuristics.euclidean
        elif heuristic_type == 'manhattan':
            self.heuristic = Heuristics.manhattan
        elif heuristic_type == 'chebyshev':
            self.heuristic = Heuristics.chebyshev
        else:
            raise ValueError(f"Unknown heuristic type: {heuristic_type}")

    def find_path(self, start, goal):
        start_time = time.perf_counter()
        open_set = []
        heapq.heappush(open_set, (0, start))

        came_from = {}
        g_score = {i: float('inf') for i in range(self.num_vertices)}
        g_score[start] = 0

        f_score = {i: float('inf') for i in range(self.num_vertices)}
        f_score[start] = self.heuristic(self.coordinates[start], self.coordinates[goal])

        visited_set = set()
        visited_count = 0

        while open_set:
            current_f, current = heapq.heappop(open_set)

            if current in visited_set:
                continue

            visited_set.add(current)
            visited_count += 1

            if current == goal:
                end_time = time.perf_counter()
                # Восстанавливаем путь
                path = self._reconstruct_path(came_from, start, goal)
                
                # Вычисляем длину пути
                path_length = self._calculate_path_length(path)
                
                visited_percent = (visited_count / self.num_vertices) * 100
                time_taken = (end_time - start_time) * 1000  # в миллисекундах
                
                return path, path_length, visited_percent, time_taken

            # Проверяем всех соседей
            for neighbor in range(self.num_vertices):
                if self.adjacency_matrix[current][neighbor] > 0:
                    tentative_g = g_score[current] + self.adjacency_matrix[current][neighbor]
                    
                    if tentative_g < g_score[neighbor]:
                        came_from[neighbor] = current
                        g_score[neighbor] = tentative_g
                        f_score[neighbor] = g_score[neighbor] + self.heuristic(
                            self.coordinates[neighbor], self.coordinates[goal]
                        )
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))

        end_time = time.perf_counter()
        visited_percent = (visited_count / self.num_vertices) * 100
        time_taken = (end_time - start_time) * 1000
        return None, float('inf'), visited_percent, time_taken

    def _reconstruct_path(self, came_from, start, goal):
        """Восстанавливает путь от цели к старту"""
        if goal not in came_from and start != goal:
            return None
        
        path = []
        current = goal
        while current != start:
            path.append(current)
            current = came_from.get(current)
            if current is None:
                return None
        path.append(start)
        path.reverse()
        return path

    def _calculate_path_length(self, path):
        """Вычисляет длину пути"""
        if not path or len(path) < 2:
            return 0
            
        total_length = 0
        for i in range(len(path) - 1):
            u, v = path[i], path[i + 1]
            total_length += self.adjacency_matrix[u][v]
        return total_length

    def format_results(self, start_vertex, end_vertex, path, length, visited_percent, time_taken, algorithm_name):
        """Форматирует результаты для вывода"""
        result_lines = []
        
        if path:
            # Определяем размеры сетки
            max_row = max(self.coordinates[v][1] for v in self.coordinates)
            max_col = max(self.coordinates[v][0] for v in self.coordinates)
            
            path_points = []
            for vertex in path:
                x, y, h = self.coordinates[vertex]
                path_points.append(f"({int(y)}, {int(x)}, h={int(h)})")
            
            start_x, start_y, start_h = self.coordinates[start_vertex]
            end_x, end_y, end_h = self.coordinates[end_vertex]
            
            result_lines.append(f"Algorithm: {algorithm_name}")
            result_lines.append(f"{length:.2f} - length of path between ({int(start_y)}, {int(start_x)}, h={int(start_h)}) and ({int(end_y)}, {int(end_x)}, h={int(end_h)}) points.")
            result_lines.append("Path:")
            result_lines.append("[" + ", ".join(path_points) + "]")
        else:
            start_x, start_y, start_h = self.coordinates[start_vertex]
            end_x, end_y, end_h = self.coordinates[end_vertex]
            
            result_lines.append(f"Algorithm: {algorithm_name}")
            result_lines.append(f"No path found between ({int(start_y)}, {int(start_x)}, h={int(start_h)}) and ({int(end_y)}, {int(end_x)}, h={int(end_h)})")
        
        result_lines.append(f"Visited: {visited_percent:.2f}%")
        result_lines.append(f"Time: {time_taken:.2f} ms")
        
        return "\n".join(result_lines)
