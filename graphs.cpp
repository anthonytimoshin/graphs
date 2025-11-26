#include "classes.h"

void Graph::get_graph() {
    std::cout << "Enter the path to the graph: ";
    std::cin >> path;
    std::cout << std::endl;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error! File can't be opened";
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
        for (size_t j = 0; j < graph[i].size(); j++) {
            std::cout << graph[i][j].vertex << ":" << graph[i][j].weight;
            if (j < graph[i].size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool Graph::check_type() {
    
}

int Graph::size() {
    std::cout << "Size: " << vertexes;
}

int Graph::weight(int a, int b) {
    is_edge(a, b); // проверка на существование ребра
    
}

bool Graph::is_edge(int a, int b) {
    
}

void Graph::add_vertex(int num) {
    //
    vertexes++;
}

void Graph::add_edge(int a, int b, int weight) {
    
}

void Graph::print_edges() {
    
}

void Graph::list_of_edges(int num) {
    
}

void Graph::print_vertexes() {

}

void Graph::create_empty_graph() {

}

void Graph::remove_vertex(int num) {

}

void Graph::remove_edge(int a, int b) {

}

bool Graph::check_connectivity() {

}

void Graph::print_connectivity_components() {

}

int Graph::distance(int a, int b) {

}

int Graph::all_distances(int num) {

}

int Graph::spanning_tree() {

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
            int num;
            std::cout << "Enter the number of vertex:\n";
            std::cout << "num: ";
            std::cin >> num;
            add_vertex(num);
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
            add_vertex(num);
            break;
        }

        case 8: {       // print_vertexes
        
        }

        case 9: {       // create_empty_graph

        }

        case 10: {      // remove_vertex

        }

        case 11: {      // remove_edge

        }

        case 12 : {     // print_connectivity_components

        }

        case 13 : {     // distance

        }

        case 14: {      // all_distances

        }

        case 15: {      // spanning_tree

        }
    }
 }
 