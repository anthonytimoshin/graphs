#include "classes.h"
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <limits>
#include <algorithm>
#include <functional>

void Graph::get_graph() {
    std::cout << "Enter the path to the graph: ";
    std::cin >> path;
    std::cout << std::endl;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error! File can't be opened";
        return;
    }

    file >> vertexes;
    graph.resize(vertexes);     // задаем размер вектора
    std::string line;
    std::getline(file, line);   // читаем оставшуюся часть первой строки
    
    std::set<int> all_vertices;
    
    for (int i = 0; i < vertexes; i++) {
        all_vertices.insert(i + 1); // добавляем текущую вершину
        std::getline(file, line);
        std::stringstream ss(line);
        std::string pair;
        
        while (ss >> pair) {
            size_t colon_pos = pair.find(':');      // деление строки по символу ':'
            if (colon_pos != std::string::npos) {
                int vertex = std::stoi(pair.substr(0, colon_pos));
                int weight = std::stoi(pair.substr(colon_pos + 1));
                
                graph[i].push_back({vertex, weight});
                all_vertices.insert(vertex); // добавляем вершину из ребра
            }
        }
    }

    file.close();
    
    original_graph = graph;
    
    std::cout << "Graph successfully loaded from " << path << std::endl;
}

void Graph::print_graph() {
    for (int i = 0; i < vertexes; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            std::cout << graph[i][j].vertex << ":" << graph[i][j].weight;
            if (j < graph[i].size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool Graph::check_type() {
    bool is_directed = false;
    
    // Создаем map для отслеживания всех рёбер и их весов
    // Ключ: упорядоченная пара вершин, значение: минимальный вес
    std::map<std::pair<int, int>, int> edge_weights;
    
    // Собираем все рёбра из текущего графа
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            int u = i + 1;
            int v = edge.vertex;
            int weight = edge.weight;
            
            // Упорядочиваем вершины
            if (u > v) std::swap(u, v);
            
            auto key = std::make_pair(u, v);
            
            // Если ребро уже есть, берем минимальный вес
            if (edge_weights.find(key) != edge_weights.end()) {
                edge_weights[key] = std::min(edge_weights[key], weight);
            } else {
                edge_weights[key] = weight;
            }
        }
    }
    
    // Проверяем, был ли граф ориентированным
    // Если в исходном графе есть рёбра только в одну сторону - он ориентированный
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            int target = edge.vertex - 1;
            bool found_reverse = false;
            
            for (const auto& rev_edge : graph[target]) {
                if (rev_edge.vertex == i + 1) {
                    found_reverse = true;
                    break;
                }
            }
            
            if (!found_reverse) {
                is_directed = true;
                break;
            }
        }
        if (is_directed) break;
    }
    
    // Перестраиваем граф как неориентированный из edge_weights
    graph.clear();
    graph.resize(vertexes);
    
    for (const auto& entry : edge_weights) {
        int u = entry.first.first - 1;
        int v = entry.first.second - 1;
        int weight = entry.second;
        
        if (u >= 0 && u < vertexes && v >= 0 && v < vertexes) {
            graph[u].push_back({v + 1, weight});
            graph[v].push_back({u + 1, weight});
        }
    }
    
    if (is_directed) {
        std::cout << "\nGraph was directed. Successfully converted to undirected." << std::endl;
    } else {
        std::cout << "Graph is undirected." << std::endl;
    }
    
    return is_directed;
}

void Graph::size() {
    std::cout << "Size: " << vertexes << std::endl;
}

int Graph::weight(int a, int b) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) { 
        std::cout << "Vertex numbers are out of range! (should be 1.." << vertexes << ")" << std::endl;
        return -1;
    }
    
    for (const auto& edge : original_graph[a-1]) {
        if (edge.vertex == b) {
            std::cout << "Weight between " << a << " and " << b << ": " << edge.weight << std::endl;
            return edge.weight;
        }
    }
    
    std::cout << "No edge between " << a << " and " << b << std::endl;
    return -1;
}

bool Graph::is_edge(int a, int b) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) { 
        std::cout << "Vertex numbers are out of range! (should be 1.." << vertexes << ")" << std::endl;
        return false;
    }
    
    for (const auto& edge : graph[a-1]) {
        if (edge.vertex == b) {
            std::cout << "Edge exists between " << a << " and " << b << std::endl;
            return true;
        }
    }
    std::cout << "No edge between " << a << " and " << b << std::endl;
    return false;
}

void Graph::add_vertex() {
    graph.push_back(std::vector<Edge>());
    vertexes = graph.size();
    
    std::cout << "Vertex " << vertexes << " added successfully!" << std::endl;
}

void Graph::add_edge(int a, int b, int weight) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) {
        std::cout << "Vertex numbers are out of range!" << std::endl;
        return;
    }
    
    // Проверяем, есть ли уже такое ребро a->b
    bool edge_exists = false;
    for (auto& edge : graph[a-1]) {
        if (edge.vertex == b) {
            edge.weight = weight;
            edge_exists = true;
            break;
        }
    }
    
    // Если ребра не было, добавляем его
    if (!edge_exists) {
        graph[a-1].push_back({b, weight});
    }
    
    // Проверяем, есть ли обратное ребро b->a
    bool reverse_exists = false;
    for (auto& edge : graph[b-1]) {
        if (edge.vertex == a) {
            edge.weight = weight;
            reverse_exists = true;
            break;
        }
    }
    
    // Если обратного ребра нет, добавляем его
    if (!reverse_exists) {
        graph[b-1].push_back({a, weight});
    }
    
    std::cout << "Edge added/updated successfully!" << std::endl;
}


void Graph::print_edges() {
    std::cout << "All edges in the graph:" << std::endl;
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            std::cout << i+1 << " -> " << edge.vertex << " (weight: " << edge.weight << ")" << std::endl;
        }
    }
}

void Graph::list_of_edges(int num) {
    if (num < 1 || num > vertexes) {
        std::cout << "Vertex number is out of range! (1.." << vertexes << ")" << std::endl;
        return;
    }
    
    std::cout << "Edges from vertex " << num << ":" << std::endl;
    for (const auto& edge : graph[num-1]) {
        std::cout << num << " -> " << edge.vertex << " (weight: " << edge.weight << ")" << std::endl;
    }
    
    std::cout << "Edges to vertex " << num << ":" << std::endl;
    bool found = false;
    for (int i = 1; i <= vertexes; i++) {
        if (i == num) continue;
        for (const auto& edge : graph[i-1]) {
            if (edge.vertex == num) {
                std::cout << i << " -> " << num << " (weight: " << edge.weight << ")" << std::endl;
                found = true;
            }
        }
    }
    if (!found) {
        std::cout << "No edges to this vertex" << std::endl;
    }
}

void Graph::print_vertexes() {
    std::set<int> unique_vertices;
    for (int i = 1; i <= vertexes; i++) {
        unique_vertices.insert(i);
        for (const auto& edge : graph[i-1]) {
            unique_vertices.insert(edge.vertex);
        }
    }
    
    std::cout << "All vertices in the graph:" << std::endl;
    for (int v : unique_vertices) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void Graph::create_empty_graph() {
    graph.clear();
    vertexes = 0;
    path = "";
    std::cout << "Empty graph created!" << std::endl;
}

void Graph::remove_vertex(int num) {
    if (num < 1 || num > vertexes) {
        std::cout << "Vertex number is out of range!" << std::endl;
        return;
    }
    
    // Удаляем все рёбра, ведущие к вершине
    for (int i = 1; i <= vertexes; i++) {
        if (i == num) continue;
        graph[i-1].erase(
            std::remove_if(graph[i-1].begin(), graph[i-1].end(),
                [num](const Edge& e) { return e.vertex == num; }),
            graph[i-1].end()
        );
    }
    
    // Удаляем вершину
    graph.erase(graph.begin() + (num - 1));
    vertexes--;
    
    // Обновляем индексы вершин в рёбрах
    for (int i = 1; i <= vertexes; i++) {
        for (auto& edge : graph[i-1]) {
            if (edge.vertex > num) {
                edge.vertex--;
            }
        }
    }
    
    std::cout << "Vertex " << num << " removed successfully!" << std::endl;
}

void Graph::remove_edge(int a, int b) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) {
        std::cout << "Vertex numbers are out of range!" << std::endl;
        return;
    }
    
    bool removed_forward = false;
    bool removed_backward = false;
    
    // Удаляем ребро a->b
    auto it = std::remove_if(graph[a-1].begin(), graph[a-1].end(),
        [b](const Edge& e) { return e.vertex == b; });
    
    if (it != graph[a-1].end()) {
        graph[a-1].erase(it, graph[a-1].end());
        removed_forward = true;
    }
    
    // Удаляем ребро b->a
    it = std::remove_if(graph[b-1].begin(), graph[b-1].end(),
        [a](const Edge& e) { return e.vertex == a; });
    
    if (it != graph[b-1].end()) {
        graph[b-1].erase(it, graph[b-1].end());
        removed_backward = true;
    }
    
    if (removed_forward || removed_backward) {
        std::cout << "Edge(s) removed successfully!" << std::endl;
    } else {
        std::cout << "Edge doesn't exist!" << std::endl;
    }
}

bool Graph::check_connectivity() {
    if (vertexes == 0) return true;
    
    // Получаем список всех вершин, которые реально существуют в графе
    std::set<int> all_vertices;
    for (int i = 0; i < vertexes; i++) {
        if (!graph[i].empty()) {
            all_vertices.insert(i + 1);
        }
        for (const auto& edge : graph[i]) {
            all_vertices.insert(edge.vertex);
        }
    }
    
    if (all_vertices.empty()) return true;
    
    // Преобразуем граф в неориентированный для обхода
    std::vector<std::vector<int>> undirected_graph(vertexes);
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            int v = edge.vertex - 1;
            if (v >= 0 && v < vertexes) {
                undirected_graph[i].push_back(v);
                undirected_graph[v].push_back(i);
            }
        }
    }
    
    // Начинаем BFS с первой реальной вершины
    int start_vertex = *all_vertices.begin() - 1;
    std::vector<bool> visited(vertexes, false);
    std::queue<int> q;
    
    q.push(start_vertex);
    visited[start_vertex] = true;
    int visited_count = 1;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        for (int neighbor : undirected_graph[current]) {
            if (neighbor >= 0 && neighbor < vertexes && !visited[neighbor]) {
                visited[neighbor] = true;
                visited_count++;
                q.push(neighbor);
            }
        }
    }
    
    // Проверяем, все ли реальные вершины были посещены
    for (int v : all_vertices) {
        if (v >= 1 && v <= vertexes && !visited[v - 1]) {
            return false;
        }
    }
    
    return true;
}

void Graph::print_connectivity_components() {
    if (vertexes == 0) {
        std::cout << "Graph is empty!" << std::endl;
        return;
    }
    
    // Преобразуем граф в неориентированный для обхода
    std::vector<std::vector<int>> undirected_graph(vertexes);
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            int v = edge.vertex - 1;
            if (v >= 0 && v < vertexes) {
                undirected_graph[i].push_back(v);
                undirected_graph[v].push_back(i);
            }
        }
    }
    
    // Находим все компоненты связности
    std::vector<bool> visited(vertexes, false);
    std::vector<std::vector<int>> components;
    
    for (int i = 0; i < vertexes; i++) {
        // Проверяем, является ли вершина реальной (имеет ребра или является конечной точкой ребра)
        bool is_real_vertex = false;
        if (!graph[i].empty()) {
            is_real_vertex = true;
        } else {
            // Проверяем, есть ли ребра, которые ведут к этой вершине
            for (int j = 0; j < vertexes; j++) {
                for (const auto& edge : graph[j]) {
                    if (edge.vertex - 1 == i) {
                        is_real_vertex = true;
                        break;
                    }
                }
                if (is_real_vertex) break;
            }
        }
        
        if (is_real_vertex && !visited[i]) {
            std::vector<int> component;
            std::queue<int> q;
            
            q.push(i);
            visited[i] = true;
            
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                component.push_back(current + 1);
                
                for (int neighbor : undirected_graph[current]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            
            // Сортируем компоненту для красивого вывода
            std::sort(component.begin(), component.end());
            components.push_back(component);
        }
    }
    
    std::cout << "Number of connectivity components: " << components.size() << std::endl;
    for (size_t i = 0; i < components.size(); i++) {
        std::cout << "Component " << i + 1 << ": ";
        for (size_t j = 0; j < components[i].size(); j++) {
            std::cout << components[i][j];
            if (j < components[i].size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}
int Graph::distance(int a, int b) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) {
        std::cout << "Vertex numbers are out of range!" << std::endl;
        return -1;
    }
    
    // Используем ОРИГИНАЛЬНЫЙ граф (ориентированный)
    int source = a - 1;
    int target = b - 1;
    
    std::vector<int> dist(vertexes, std::numeric_limits<int>::max());
    std::vector<int> parent(vertexes, -1); // Массив предков для восстановления пути
    dist[source] = 0;
    
    // Алгоритм Беллмана-Форда на оригинальном графе
    // Релаксация всех рёбер (V-1) раз
    for (int i = 0; i < vertexes - 1; i++) {
        for (int u = 0; u < vertexes; u++) {
            for (const auto& edge : original_graph[u]) {
                int v = edge.vertex - 1;
                if (dist[u] != std::numeric_limits<int>::max() && 
                    dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                    parent[v] = u; // Запоминаем предка
                }
            }
        }
    }
    
    // Проверка на отрицательные циклы
    for (int u = 0; u < vertexes; u++) {
        for (const auto& edge : original_graph[u]) {
            int v = edge.vertex - 1;
            if (dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + edge.weight < dist[v]) {
                std::cout << "Graph contains negative weight cycle!" << std::endl;
                return -1;
            }
        }
    }
    
    // Восстанавливаем путь
    std::vector<int> path = reconstruct_path(parent, source, target);
    
    // Выводим результат с путем
    std::cout << "Directed distance from " << a << " to " << b << ": " << dist[target] << std::endl;
    std::cout << "Path: " << std::endl;
    std::cout << "  ";
    
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " → ";
        }
    }
    std::cout << std::endl;
}
int Graph::all_distances(int num) {
    if (num < 1 || num > vertexes) {
        std::cout << "Vertex number is out of range!" << std::endl;
        return -1;
    }
    
    int source = num - 1;
    
    // Алгоритм Беллмана-Форда от вершины source
    std::vector<int> dist(vertexes, std::numeric_limits<int>::max());
    std::vector<int> parent(vertexes, -1);
    dist[source] = 0;
    
    for (int i = 0; i < vertexes - 1; i++) {
        for (int u = 0; u < vertexes; u++) {
            for (const auto& edge : graph[u]) {
                int v = edge.vertex - 1;
                if (dist[u] != std::numeric_limits<int>::max() && 
                    dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                    parent[v] = u;
                }
            }
        }
    }
    
    // Проверка на отрицательные циклы
    for (int u = 0; u < vertexes; u++) {
        for (const auto& edge : graph[u]) {
            int v = edge.vertex - 1;
            if (dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + edge.weight < dist[v]) {
                std::cout << "Graph contains negative weight cycle!" << std::endl;
                return -1;
            }
        }
    }
    
    std::cout << "======================================" << std::endl;
    std::cout << "Distances from vertex " << num << " to all other vertices:" << std::endl;
    std::cout << "======================================" << std::endl;
    
    for (int i = 0; i < vertexes; i++) {
        if (i == source) continue;
        
        std::cout << std::endl << "To vertex " << (i + 1) << ": ";
        
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << "No path" << std::endl;
        } else {
            std::cout << "Distance = " << dist[i] << std::endl;
            
            // Восстанавливаем и выводим путь
            std::vector<int> path = reconstruct_path(parent, source, i);
            
            std::cout << "  Path: ";
            for (size_t j = 0; j < path.size(); j++) {
                std::cout << path[j];
                if (j < path.size() - 1) {
                    std::cout << " → ";
                }
            }
            std::cout << std::endl;
            
            // Выводим детали пути
            std::cout << "  Steps: ";
            int step_count = 0;
            for (size_t j = 0; j < path.size() - 1; j++) {
                int u = path[j] - 1;
                int v = path[j + 1] - 1;
                
                // Находим вес ребра
                int edge_weight = -1;
                for (const auto& edge : graph[u]) {
                    if (edge.vertex == v + 1) {
                        edge_weight = edge.weight;
                        break;
                    }
                }
                
                if (edge_weight != -1) {
                    if (step_count > 0) std::cout << " + ";
                    std::cout << edge_weight;
                    step_count++;
                }
            }
            
            if (step_count > 0) {
                std::cout << " = " << dist[i] << std::endl;
            }
        }
    }
    
    std::cout << "======================================" << std::endl;
    return 0;
}

int Graph::spanning_tree() {
    if (vertexes == 0) {
        std::cout << "Graph is empty!" << std::endl;
        return -1;
    }
    
    // Сначала проверяем связность
    if (!check_connectivity()) {
        std::cout << "Graph is not connected!" << std::endl;
        std::cout << "Cannot build spanning tree for disconnected graph." << std::endl;
        return -1;
    }
    
    // Алгоритм Прима
    std::vector<bool> inMST(vertexes, false);
    std::vector<int> key(vertexes, std::numeric_limits<int>::max());
    std::vector<int> parent(vertexes, -1);
    
    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    
    // Начинаем с вершины 0
    key[0] = 0;
    pq.push({0, 0});
    
    int total_weight = 0;
    int edges_added = 0;
    
    while (!pq.empty() && edges_added < vertexes - 1) {
        int u = pq.top().second;
        int current_key = pq.top().first;
        pq.pop();
        
        if (inMST[u]) continue;
        
        inMST[u] = true;
        
        // Добавляем вес ребра к общему весу (кроме первой вершины)
        if (parent[u] != -1) {
            edges_added++;
            total_weight += current_key;
        }
        
        // Рассматриваем соседей
        for (const auto& edge : graph[u]) {
            int v = edge.vertex - 1;
            int weight = edge.weight;
            
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({weight, v});
            }
        }
    }
    
    // Проверяем, что MST содержит все вершины
    int mst_vertex_count = 0;
    for (bool in_tree : inMST) {
        if (in_tree) mst_vertex_count++;
    }
    
    // if (mst_vertex_count != vertexes) {
    //     std::cout << "Warning: MST doesn't include all vertices. Something went wrong." << std::endl;
    //     return -1;
    // }
    
    // Выводим рёбра MST
    std::cout << "Edges in Minimum Spanning Tree:" << std::endl;
    
    // Собираем рёбра MST
    std::vector<std::tuple<int, int, int>> mst_edges;
    for (int i = 1; i < vertexes; i++) {
        if (parent[i] != -1) {
            mst_edges.push_back(std::make_tuple(parent[i] + 1, i + 1, key[i]));
        }
    }
    
    // Сортируем рёбра для красивого вывода
    std::sort(mst_edges.begin(), mst_edges.end(), 
        [](const auto& a, const auto& b) {
            if (std::get<0>(a) != std::get<0>(b))
                return std::get<0>(a) < std::get<0>(b);
            return std::get<1>(a) < std::get<1>(b);
        });
    
    // Выводим рёбра
    for (const auto& edge : mst_edges) {
        std::cout << std::get<0>(edge) << " - " << std::get<1>(edge) 
                  << " (weight: " << std::get<2>(edge) << ")" << std::endl;
    }
    
    std::cout << "Total weight of MST: " << total_weight << std::endl;
    return total_weight;
}

// Восстановление пути от source до target по массиву предков
std::vector<int> Graph::reconstruct_path(const std::vector<int>& parent, int source, int target) {
    std::vector<int> path;
    
    // Начинаем с конечной вершины
    int current = target;
    
    // Восстанавливаем путь в обратном порядке
    while (current != -1) {
        path.push_back(current + 1); // +1 для пользовательских номеров
        current = parent[current];
    }
    
    // Разворачиваем путь (чтобы был от source до target)
    std::reverse(path.begin(), path.end());
    
    return path;
}

void Graph::func_handler(int selected) {
    switch (selected) {
        case 0: {       // print_graph
            print_graph();
            break;
        }

        case 1: {       // size
            size();
            break;
        }

        case 2: {       // weight
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            weight(a, b);
            break;
        }

        case 3: {       // is_edge
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            is_edge(a, b);
            break;
        }

        case 4: {       // add_vertex
            add_vertex();
            break;
        }

        case 5: {       // add_edge
            int a, b, weight;
            std::cout << "Enter the numbers of vertexes and weight of edge:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b; 
            std::cout << "weight: ";
            std::cin >> weight;
            add_edge(a, b, weight);
            break;
        }

        case 6: {       // print_edges
            print_edges();
            break;
        }
        
        case 7: {       // list_of_edges
            int num;
            std::cout << "Enter the number of vertex:\n";
            std::cout << "num: ";
            std::cin >> num;
            list_of_edges(num);
            break;
        }

        case 8: {       // print_vertexes
            print_vertexes();
            break;  
        }

        case 9: {       // create_empty_graph
            create_empty_graph();
            break;
        }

        case 10: {      // read_graph_from_file
            get_graph();
            break;
        }

        case 11: {      // remove_vertex
            int num;
            std::cout << "Enter the number of vertex to remove:\n";
            std::cout << "num: ";
            std::cin >> num;
            remove_vertex(num);
            break;
        }

        case 12: {      // remove_edge
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            remove_edge(a, b);
            break;
        }

        case 13: {     // print_connectivity_components
            print_connectivity_components();
            break;
        }

        case 14: {     // distance
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            distance(a, b);
            break;
        }

        case 15: {      // all_distances
            int num;
            std::cout << "Enter the number of source vertex:\n";
            std::cout << "num: ";
            std::cin >> num;
            all_distances(num);
            break;
        }

        case 16: {      // spanning_tree
            spanning_tree();
            break;
        }
    }
}