import sys
from ford_fulkerson import FordFulkerson
from astar import AStar
from dijkstra import Dijkstra
from bipartite import BipartiteGraph


def task1_ford_fulkerson(input_file, output_file=None):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        if not lines or not any(line.strip() for line in lines):
            raise ValueError("Ошибка: файл пустой")

        n = int(lines[0].strip())
        matrix = []
        for i in range(1, n + 1):
            row = list(map(int, lines[i].strip().split()))
            matrix.append(row)

        # Перебираем все возможные пары источник-сток для нахождения максимального потока
        best_source = 0
        best_sink = 1
        best_flow = 0
        best_paths = []
        
        print(f"Поиск оптимальной пары источник-сток для графа из {n} вершин...")
        
        # Для оптимизации проверяем только вершины, которые могут быть источниками/стоками
        potential_sources = []
        potential_sinks = []
        
        for i in range(n):
            has_outgoing = any(matrix[i][j] > 0 for j in range(n))
            has_incoming = any(matrix[j][i] > 0 for j in range(n))
            
            if has_outgoing:  # Может быть источником
                potential_sources.append(i)
            if has_incoming:  # Может быть стоком
                potential_sinks.append(i)
        
        # Если списки пусты, используем все вершины
        if not potential_sources:
            potential_sources = list(range(n))
        if not potential_sinks:
            potential_sinks = list(range(n))
        
        # Перебираем все пары
        for source in potential_sources:
            for sink in potential_sinks:
                if source == sink:
                    continue
                    
                ff = FordFulkerson(matrix)
                max_flow, paths, _ = ff.find_max_flow(source, sink)
                
                if max_flow > best_flow:
                    best_flow = max_flow
                    best_paths = paths
                    best_source = source
                    best_sink = sink
                    print(f"  Найдена лучшая пара: источник={source+1}, сток={sink+1}, поток={best_flow}")

        print(f"\nОптимальная пара: источник={best_source+1}, сток={best_sink+1}")
        print(f"Максимальный поток: {best_flow}\n")

        # Получаем финальные результаты
        ff_final = FordFulkerson(matrix)
        max_flow, paths, original_graph = ff_final.find_max_flow(best_source, best_sink)
        
        results = FordFulkerson.format_results(best_source, best_sink, max_flow, paths, original_graph)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(results)
        else:
            print(results)

        return max_flow, paths

    except FileNotFoundError:
        print(f"Ошибка: файл '{input_file}' не найден")
        sys.exit(1)
    except ValueError as e:
        print(f"Ошибка: {e}")
        sys.exit(1)
    except IndexError:
        print(f"Ошибка: недостаточно данных в файле")
        sys.exit(1)
        
def task2_astar_dijkstra(input_file, output_file=None):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        if not lines or not any(line.strip() for line in lines):
            raise ValueError("Ошибка: файл пустой")

        # Убираем пустые строки
        lines = [line.strip() for line in lines if line.strip()]
        
        # Читаем размеры сетки
        rows, cols = map(int, lines[0].split())
        
        # Читаем значения ячеек сетки
        grid = []
        for i in range(1, rows + 1):
            row = list(map(int, lines[i].split()))
            grid.append(row)
        
        # Преобразуем сетку в граф
        n = rows * cols  # Каждая ячейка - вершина
        matrix = [[0] * n for _ in range(n)]
        coordinates = {}
        
        # Заполняем координаты вершин
        # Используем координаты: x - столбец, y - строка (сверху вниз)
        for r in range(rows):
            for c in range(cols):
                vertex = r * cols + c
                coordinates[vertex] = (float(c), float(r))
        
        # Создаем ребра (8-связность - можно ходить по диагонали)
        for r in range(rows):
            for c in range(cols):
                v = r * cols + c
                
                # Проверяем всех соседей
                for dr in [-1, 0, 1]:
                    for dc in [-1, 0, 1]:
                        if dr == 0 and dc == 0:
                            continue  # Пропускаем саму ячейку
                        
                        nr, nc = r + dr, c + dc
                        if 0 <= nr < rows and 0 <= nc < cols:
                            nv = nr * cols + nc
                            # Вес ребра = среднее значение двух ячеек
                            weight = (grid[r][c] + grid[nr][nc]) / 2.0
                            matrix[v][nv] = weight
        
        # Устанавливаем старт и финиш (левый верхний и правый нижний углы)
        start_vertex = 0  # (0, 0) - левый верхний
        end_vertex = (rows - 1) * cols + (cols - 1)  # (rows-1, cols-1) - правый нижний
        
        start_coord = coordinates[start_vertex]
        end_coord = coordinates[end_vertex]
        
        print(f"\nСтартовая вершина: {start_vertex} ({start_coord[0]:.0f}, {start_coord[1]:.0f})")
        print(f"Конечная вершина: {end_vertex} ({end_coord[0]:.0f}, {end_coord[1]:.0f})")
        
        results = []

        # A* с евклидовой эвристикой
        print("\nЗапуск A* с евклидовой эвристикой...")
        astar_euclidean = AStar(matrix, coordinates, 'euclidean')
        path_e, length_e, visited_e, time_e = astar_euclidean.find_path(start_vertex, end_vertex)
        results.append(
            AStar.format_results(start_vertex, end_vertex, path_e, length_e, visited_e, time_e, 'Евклидовой'))
        results.append("\n" + "=" * 60 + "\n")

        # A* с манхэттенской эвристикой
        print("Запуск A* с манхэттенской эвристикой...")
        astar_manhattan = AStar(matrix, coordinates, 'manhattan')
        path_m, length_m, visited_m, time_m = astar_manhattan.find_path(start_vertex, end_vertex)
        results.append(
            AStar.format_results(start_vertex, end_vertex, path_m, length_m, visited_m, time_m, 'Манхэттенской'))
        results.append("\n" + "=" * 60 + "\n")

        # Дейкстра
        print("Запуск алгоритма Дейкстры...")
        dijkstra = Dijkstra(matrix)
        distances, predecessors = dijkstra.find_shortest_path(start_vertex)
        path_d = dijkstra.reconstruct_path(predecessors, start_vertex, end_vertex)
        
        # Форматируем результаты Дейкстры для конечной точки
        dijkstra_result = []
        dijkstra_result.append(f"Алгоритм Дейкстры")
        dijkstra_result.append(f"Начальная вершина: {start_vertex} ({start_coord[0]:.0f}, {start_coord[1]:.0f})")
        dijkstra_result.append(f"Конечная вершина: {end_vertex} ({end_coord[0]:.0f}, {end_coord[1]:.0f})")
        dijkstra_result.append("")
        
        if path_d:
            # Вычисляем длину пути
            path_length = 0
            for i in range(len(path_d) - 1):
                u, v = path_d[i], path_d[i + 1]
                path_length += matrix[u][v]
            
            dijkstra_result.append(f"Длина найденного пути: {path_length:.2f}")
            dijkstra_result.append(f"Вершины составляющие путь: {' -> '.join(map(str, path_d))}")
        else:
            dijkstra_result.append("Путь не найден")
        
        results.append("\n".join(dijkstra_result))

        final_results = "\n".join(results)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(final_results)
            print(f"\nРезультаты сохранены в файл: {output_file}")
        else:
            print("\n" + final_results)

        return path_e, path_m, path_d

    except FileNotFoundError:
        print(f"Ошибка: файл '{input_file}' не найден")
        sys.exit(1)
    except ValueError as e:
        print(f"Ошибка: {e}")
        sys.exit(1)
    except IndexError:
        print(f"Ошибка: недостаточно данных в файле")
        sys.exit(1)

def task3_bipartite(input_file, output_file=None):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        if not lines or not any(line.strip() for line in lines):
            raise ValueError("Ошибка: файл пустой")

        n = int(lines[0].strip())
        matrix = []
        for i in range(1, n + 1):
            row = list(map(int, lines[i].strip().split()))
            matrix.append(row)

        bg = BipartiteGraph(matrix)
        is_bip, partitions = bg.is_bipartite()
        matching_size, matching_edges = 0, []

        if is_bip:
            matching_size, matching_edges = bg.kuhn_algorithm()

        results = BipartiteGraph.format_results(is_bip, partitions, matching_size, matching_edges)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(results)
        else:
            print(results)

        return is_bip, matching_size

    except FileNotFoundError:
        print(f"Ошибка: файл '{input_file}' не найден")
        sys.exit(1)
    except ValueError as e:
        print(f"Ошибка: {e}")
        sys.exit(1)
    except IndexError:
        print(f"Ошибка: недостаточно данных в файле")
        sys.exit(1)


def main():
    if len(sys.argv) < 3:
        print("Использование: python main.py <номер_задачи> <входной_файл> [выходной_файл]")
        print("Номера задач: 1 - Форд-Фалкерсон, 2 - A*/Дейкстра, 3 - Двудольность")
        return

    task_num = int(sys.argv[1])
    input_file = sys.argv[2]
    output_file = sys.argv[3] if len(sys.argv) > 3 else None

    if task_num == 1:
        task1_ford_fulkerson(input_file, output_file)
    elif task_num == 2:
        task2_astar_dijkstra(input_file, output_file)
    elif task_num == 3:
        task3_bipartite(input_file, output_file)
    else:
        print("Неизвестный номер задачи")


if __name__ == "__main__":
    main()
