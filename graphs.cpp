#include "classes.h"

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
    
    for (int i = 0; i < vertexes; i++) {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string pair;
        
        while (ss >> pair) {
            size_t colon_pos = pair.find(':');      // деление строки по символу ':'
            if (colon_pos != std::string::npos) {
                int vertex = std::stoi(pair.substr(0, colon_pos));
                int weight = std::stoi(pair.substr(colon_pos + 1));
                
                graph[i].push_back({vertex, weight});   // добавляем ребро в граф
            }
        }
    }

    file.close();
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
    // Для ориентированного графа проверяем, есть ли несимметричные рёбра
    for (int i = 0; i < vertexes; i++) {
        for (const auto& edge : graph[i]) {
            bool found = false;
            for (const auto& rev_edge : graph[edge.vertex - 1]) {
                if (rev_edge.vertex == i + 1 && rev_edge.weight == edge.weight) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return true; // ориентированный
            }
        }
    }
    return false; // неориентированный     
}

void Graph::size() {
    std::cout << "Size: " << vertexes << std::endl;
}

int Graph::weight(int a, int b) {
    if (!is_edge(a, b)) {
        std::cout << "Edge doesn't exist!" << std::endl;
        return -1;
    }
    
    for (const auto& edge : graph[a-1]) {
        if (edge.vertex == b) {
            std::cout << "Weight between " << a << " and " << b << ": " << edge.weight << std::endl;
            return edge.weight;
        }
    }
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
    
    // Проверяем, есть ли уже такое ребро
    for (auto& edge : graph[a-1]) {
        if (edge.vertex == b) {
            edge.weight = weight;
            std::cout << "Edge weight updated!" << std::endl;
            return;
        }
    }
    
    // Добавляем новое ребро
    graph[a-1].push_back({b, weight});
    std::cout << "Edge added successfully!" << std::endl;
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
        for (const auto& edge : graph[i]) {
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
    
    // Удаляем ребро a -> b
    auto it = std::remove_if(graph[a-1].begin(), graph[a-1].end(),
        [b](const Edge& e) { return e.vertex == b; });
    
    if (it != graph[a-1].end()) {
        graph[a-1].erase(it, graph[a-1].end());
        std::cout << "Edge removed successfully!" << std::endl;
    } else {
        std::cout << "Edge doesn't exist!" << std::endl;
    }
}

bool Graph::check_connectivity() {
    if (vertexes == 0) return true;
    
    std::vector<bool> visited(vertexes, false);
    std::queue<int> q;
    
    // Начинаем с вершины 0 (пользовательская вершина 1)
    q.push(0);
    visited[0] = true;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        for (const auto& edge : graph[current]) {
            int neighbor = edge.vertex - 1; // Преобразуем в индекс программы
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    
    // Проверяем, все ли вершины посещены
    for (bool v : visited) {
        if (!v) return false;
    }
    
    return true;
}

void Graph::print_connectivity_components() {
    std::vector<bool> visited(vertexes, false);
    std::vector<std::vector<int>> components;
    
    for (int i = 0; i < vertexes; i++) {
        if (!visited[i]) {
            std::vector<int> component;
            std::queue<int> q;
            
            q.push(i);
            visited[i] = true;
            
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                component.push_back(current + 1); // Сохраняем пользовательский номер
                
                for (const auto& edge : graph[current]) {
                    int neighbor = edge.vertex - 1; // Преобразуем в индекс программы
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            
            components.push_back(component);
        }
    }
    
    std::cout << "Number of connectivity components: " << components.size() << std::endl;
    for (size_t i = 0; i < components.size(); i++) {
        std::cout << "Component " << i + 1 << ": ";
        for (int v : components[i]) {
            std::cout << v << " "; // v уже в пользовательской нумерации
        }
        std::cout << std::endl;
    }
}

int Graph::distance(int a, int b) {
    if (a < 1 || a > vertexes || b < 1 || b > vertexes) {
        std::cout << "Vertex numbers are out of range!" << std::endl;
        return -1;
    }
    
    // Преобразуем в индексы программы
    int source = a - 1;
    int target = b - 1;
    
    // Алгоритм Беллмана-Форда
    std::vector<int> dist(vertexes, std::numeric_limits<int>::max());
    dist[source] = 0;
    
    // Релаксация всех рёбер (V-1) раз
    for (int i = 0; i < vertexes - 1; i++) {
        for (int u = 0; u < vertexes; u++) {
            for (const auto& edge : graph[u]) {
                int v = edge.vertex - 1; // Преобразуем в индекс программы
                if (dist[u] != std::numeric_limits<int>::max() && 
                    dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                }
            }
        }
    }
    
    // Проверка на отрицательные циклы
    for (int u = 0; u < vertexes; u++) {
        for (const auto& edge : graph[u]) {
            int v = edge.vertex - 1; // Преобразуем в индекс программы
            if (dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + edge.weight < dist[v]) {
                std::cout << "Graph contains negative weight cycle!" << std::endl;
                return -1;
            }
        }
    }
    
    if (dist[target] == std::numeric_limits<int>::max()) {
        std::cout << "No path between " << a << " and " << b << std::endl;
        return -1;
    }
    
    std::cout << "Distance between " << a << " and " << b << ": " << dist[target] << std::endl;
    return dist[target];
}

int Graph::all_distances(int num) {
    if (num < 1 || num > vertexes) {
        std::cout << "Vertex number is out of range!" << std::endl;
        return -1;
    }
    
    int source = num - 1; // Преобразуем в индекс программы
    
    // Алгоритм Беллмана-Форда от вершины source
    std::vector<int> dist(vertexes, std::numeric_limits<int>::max());
    dist[source] = 0;
    
    for (int i = 0; i < vertexes - 1; i++) {
        for (int u = 0; u < vertexes; u++) {
            for (const auto& edge : graph[u]) {
                int v = edge.vertex - 1; // Преобразуем в индекс программы
                if (dist[u] != std::numeric_limits<int>::max() && 
                    dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                }
            }
        }
    }
    
    // Проверка на отрицательные циклы
    for (int u = 0; u < vertexes; u++) {
        for (const auto& edge : graph[u]) {
            int v = edge.vertex - 1; // Преобразуем в индекс программы
            if (dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + edge.weight < dist[v]) {
                std::cout << "Graph contains negative weight cycle!" << std::endl;
                return -1;
            }
        }
    }
    
    std::cout << "Distances from vertex " << num << " to all other vertices:" << std::endl;
    for (int i = 0; i < vertexes; i++) {
        if (i == source) continue;
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << "To vertex " << i+1 << ": No path" << std::endl;
        } else {
            std::cout << "To vertex " << i+1 << ": " << dist[i] << std::endl;
        }
    }
    
    return 0;
}

int Graph::spanning_tree() {
    if (vertexes == 0) {
        std::cout << "Graph is empty!" << std::endl;
        return -1;
    }
    
    // Алгоритм Прима
    std::vector<bool> inMST(vertexes, false);
    std::vector<int> key(vertexes, std::numeric_limits<int>::max());
    std::vector<int> parent(vertexes, -1);
    
    // Используем priority_queue для эффективного выбора минимального ребра
    using Pair = std::pair<int, int>; // вес, вершина
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    
    // Начинаем с вершины 0 (пользовательская вершина 1)
    key[0] = 0;
    pq.push({0, 0});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (inMST[u]) continue;
        inMST[u] = true;
        
        for (const auto& edge : graph[u]) {
            int v = edge.vertex - 1; // Преобразуем в индекс программы
            int weight = edge.weight;
            
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
    
    // Вывод минимального остовного дерева
    std::cout << "Edges in Minimum Spanning Tree:" << std::endl;
    int total_weight = 0;
    
    for (int i = 1; i < vertexes; i++) {
        if (parent[i] != -1) {
            std::cout << (parent[i] + 1) << " - " << (i + 1) << std::endl;
            // Находим вес ребра
            for (const auto& edge : graph[parent[i]]) {
                if (edge.vertex == i + 1) {
                    total_weight += edge.weight;
                    break;
                }
            }
        }
    }
    
    std::cout << "Total weight of MST: " << total_weight << std::endl;
    return total_weight;
}

void Graph::func_handler(int selected) {
    switch (selected) {
        case 0: {       // print_graph
            print_graph();
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

        case 13 : {     // print_connectivity_components
            print_connectivity_components();
            break;
        }

        case 14 : {     // distance
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
