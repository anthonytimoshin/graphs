import sys
from ford_fulkerson import FordFulkerson
from astar import AStar
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

        # Находим источник и сток по определению
        source = -1
        sink = -1
        
        print(f"Анализ графа из {n} вершин для определения источника и стока...")
        
        for i in range(n):
            # Проверяем входящие ребра
            has_incoming = any(matrix[j][i] > 0 for j in range(n))
            # Проверяем исходящие ребра
            has_outgoing = any(matrix[i][j] > 0 for j in range(n))
            
            # Источник: есть исходящие, нет входящих
            if has_outgoing and not has_incoming:
                if source == -1:
                    source = i
                    print(f"  Найден источник: вершина {i+1}")
                else:
                    print(f"  Внимание: найдено несколько возможных источников (вершина {i+1})")
            
            # Сток: есть входящие, нет исходящих
            if has_incoming and not has_outgoing:
                if sink == -1:
                    sink = i
                    print(f"  Найден сток: вершина {i+1}")
                else:
                    print(f"  Внимание: найдено несколько возможных стоков (вершина {i+1})")
        
        # Если не нашли по строгому определению, используем эвристику
        if source == -1 or sink == -1:
            print("  Не удалось найти источник/сток по строгому определению. Использую эвристику...")
            
            # Эвристика: источник - вершина с максимальной разницей исходящих/входящих
            if source == -1:
                max_diff = -float('inf')
                for i in range(n):
                    out_degree = sum(1 for j in range(n) if matrix[i][j] > 0)
                    in_degree = sum(1 for j in range(n) if matrix[j][i] > 0)
                    diff = out_degree - in_degree
                    if diff > max_diff:
                        max_diff = diff
                        source = i
                print(f"  Выбран источник (эвристика): вершина {source+1} (разница: {max_diff})")
            
            # Эвристика: сток - вершина с максимальной разницей входящих/исходящих
            if sink == -1:
                max_diff = -float('inf')
                for i in range(n):
                    out_degree = sum(1 for j in range(n) if matrix[i][j] > 0)
                    in_degree = sum(1 for j in range(n) if matrix[j][i] > 0)
                    diff = in_degree - out_degree
                    if diff > max_diff:
                        max_diff = diff
                        sink = i
                print(f"  Выбран сток (эвристика): вершина {sink+1} (разница: {max_diff})")
        
        # Проверка на корректность
        if source == -1:
            source = 0
            print(f"  Источник не найден, используется вершина 1 по умолчанию")
        if sink == -1:
            sink = n-1 if n > 1 else 0
            print(f"  Сток не найден, используется вершина {sink+1} по умолчанию")
        
        if source == sink:
            print(f"  Предупреждение: источник и сток совпадают! Выбираю другую вершину для стока...")
            sink = (sink + 1) % n
            print(f"  Новый сток: вершина {sink+1}")

        print(f"\nОпределено:")
        print(f"  Источник: вершина {source+1}")
        print(f"  Сток: вершина {sink+1}")

        # Находим максимальный поток
        ff = FordFulkerson(matrix)
        max_flow, paths, original_graph = ff.find_max_flow(source, sink)
        
        print(f"\nМаксимальный поток: {max_flow}")

        results = FordFulkerson.format_results(source, sink, max_flow, paths, original_graph)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(results)
        else:
            print("\n" + results)

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


def task2_astar(input_file, start_row=None, start_col=None, goal_row=None, goal_col=None, output_file=None):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        if not lines or not any(line.strip() for line in lines):
            raise ValueError("Ошибка: файл пустой")

        # Читаем размеры карты
        rows, cols = map(int, lines[0].strip().split())
        
        # Читаем матрицу высот
        height_map = []
        for i in range(1, rows + 1):
            row = list(map(int, lines[i].strip().split()))
            height_map.append(row)
        
        # Определяем старт и финиш, если их не задать 
        if start_row is None or start_col is None:
            start_row, start_col = 0, 0
        if goal_row is None or goal_col is None:
            goal_row, goal_col = rows-1, cols-1
        
        print(f"Карта: {rows}×{cols}")
        print(f"Старт: ({start_row}, {start_col}), высота: {height_map[start_row][start_col]}")
        print(f"Финиш: ({goal_row}, {goal_col}), высота: {height_map[goal_row][goal_col]}")
        
        # Проверяем, что старт и финиш не являются непроходимыми (высота = 0)
        if height_map[start_row][start_col] == 0:
            print("Ошибка: стартовая точка непроходима (высота = 0)")
            return None
        if height_map[goal_row][goal_col] == 0:
            print("Ошибка: конечная точка непроходима (высота = 0)")
            return None
        
        # Создаем граф из высотной карты с 4-связностью
        n = rows * cols  # Каждая ячейка - вершина
        adjacency_matrix = [[0.0] * n for _ in range(n)]
        coordinates = {}
        
        # Заполняем координаты вершин
        for r in range(rows):
            for c in range(cols):
                vertex = r * cols + c
                coordinates[vertex] = (float(c), float(r), float(height_map[r][c]))  # (x, y, высота)
        
        # 4 направления (по вертикали и горизонтали)
        directions = [
            (-1, 0),  # вверх
            (1, 0),   # вниз
            (0, -1),  # влево
            (0, 1)    # вправо
        ]
        
        # Создаем рёбра с весом по формуле
        for r in range(rows):
            for c in range(cols):
                current = r * cols + c
                current_height = height_map[r][c]
                
                # Если текущая клетка непроходима, пропускаем
                if current_height == 0:
                    continue
                
                for dr, dc in directions:
                    nr, nc = r + dr, c + dc
                    
                    if 0 <= nr < rows and 0 <= nc < cols:
                        neighbor = nr * cols + nc
                        neighbor_height = height_map[nr][nc]
                        
                        # Если соседняя клетка непроходима, не создаем ребро
                        if neighbor_height == 0:
                            continue
                        
                        # Вычисляем вес по формуле: abs(k-i) + abs(l-j) + abs(a_kl - a_ij)
                        weight = abs(dr) + abs(dc) + abs(neighbor_height - current_height)
                        
                        adjacency_matrix[current][neighbor] = float(weight)
                        adjacency_matrix[neighbor][current] = float(weight)
        
        # Находим соответствующие вершины
        start_vertex = start_row * cols + start_col
        end_vertex = goal_row * cols + goal_col
        
        print(f"Стартовая вершина: {start_vertex}")
        print(f"Конечная вершина: {end_vertex}")
        
        results = []
        
        # Запускаем A* с разными эвристиками
        # Для 4-связности Manhattan - оптимальная эвристика
        algorithms = [
            ("A* Manhattan", 'manhattan'),
            ("A* Euclidean", 'euclidean'),
            ("A* Chebyshev", 'chebyshev')
        ]
        
        for algo_name, heuristic_type in algorithms:
            astar = AStar(adjacency_matrix, coordinates, heuristic_type)
            path, length, visited_percent, time_taken = astar.find_path(start_vertex, end_vertex)
            
            # Преобразуем путь из вершин в координаты (строка, столбец)
            path_coords = []
            path_heights = []
            if path:
                for vertex in path:
                    row = vertex // cols
                    col = vertex % cols
                    height = height_map[row][col]
                    path_coords.append((row, col, height))
                
                results.append(f"Algorithm: {algo_name}")
                results.append(f"{length:.2f} - length of path between ({start_row}, {start_col}) and ({goal_row}, {goal_col}) points.")
                results.append("Path:")
                
                # Форматируем путь
                path_str = "["
                for i, (r, c, h) in enumerate(path_coords):
                    if i > 0:
                        path_str += ", "
                    path_str += f"({r}, {c}, h={h})"
                path_str += "]"
                results.append(path_str)
                
                results.append(f"Visited: {visited_percent:.2f}%")
                results.append(f"Time: {time_taken:.2f} ms")
            else:
                results.append(f"Algorithm: {algo_name}")
                results.append(f"No path found between ({start_row}, {start_col}) and ({goal_row}, {goal_col})")
                results.append(f"Visited: {visited_percent:.2f}%")
                results.append(f"Time: {time_taken:.2f} ms")
            
            results.append("") 
        
        final_results = "\n".join(results)
        
        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(final_results)
        else:
            print(final_results)
        
        return results

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

        # Считываем матрицу смежности
        n = int(lines[0].strip())
        matrix = []
        for i in range(1, n + 1):
            row = list(map(int, lines[i].strip().split()))
            if len(row) != n:
                raise ValueError(f"Неверное количество элементов в строке {i+1}")
            matrix.append(row)

        # Создаем объект BipartiteGraph и проверяем двудольность
        bg = BipartiteGraph(matrix)
        is_bip, partitions = bg.is_bipartite()
        
        # Находим максимальное паросочетание
        matching_size, matching_edges = bg.kuhn_algorithm()
        
        # Форматируем результаты
        results = BipartiteGraph.format_results(is_bip, partitions, matching_size, matching_edges)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(results)
        else:
            print(results)

        return is_bip, partitions, matching_size, matching_edges

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
        print("Номера задач: 1 - Форд-Фалкерсон, 2 - A*, 3 - Двудольность")
        print("Для задачи 2 также можно указать координаты старта и финиша:")
        print("  python main.py 2 <входной_файл> <старт_строка> <старт_колонка> <финиш_строка> <финиш_колонка> [выходной_файл]")
        return
    
    task_num = int(sys.argv[1])
    input_file = sys.argv[2]
    
    if task_num == 1:
        output_file = sys.argv[3] if len(sys.argv) > 3 else None
        task1_ford_fulkerson(input_file, output_file)
    
    elif task_num == 2:
        print("="*60)
        print("ЗАДАЧА 2: Поиск пути на карте (A* с разными эвристиками)")
        print("="*60)
        
        # Проверяем, переданы ли координаты
        if len(sys.argv) >= 7:
            # Формат: python main.py 2 input.txt start_row start_col goal_row goal_col [output.txt]
            start_row = int(sys.argv[3])
            start_col = int(sys.argv[4])
            goal_row = int(sys.argv[5])
            goal_col = int(sys.argv[6])
            output_file = sys.argv[7] if len(sys.argv) > 7 else None
            
            task2_astar(input_file, start_row, start_col, goal_row, goal_col, output_file)
    
    elif task_num == 3:
        output_file = sys.argv[3] if len(sys.argv) > 3 else None
        task3_bipartite(input_file, output_file)
    
    else:
        print("Неизвестный номер задачи")


if __name__ == "__main__":
    main()
