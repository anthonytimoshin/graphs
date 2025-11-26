#include "classes.h"

void Graph::get_graph() {
    std::cout << "Enter the path to the graph: ";
    std::cin >> path;
    std::cout << std::endl;
}

bool Graph::check_type() {
    
}

void Graph::open_graph() {
    std::fstream graph(path);
    if (!graph.is_open()) {
        std::cout << "Error! File can't be opened";
    }
}

 void Graph::func_handler(int selected) {
    switch (selected) {
        case 0: {       // size
            size();
            break;
        }

        case 1: {       // weight
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            weight(a, b);
            break;
        }

        case 2: {       // is_edge
            int a, b;
            std::cout << "Enter the numbers of vertexes:\n";
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            is_edge(a, b);
            break;
        }

        case 3: {       // add_vertex
            int num;
            std::cout << "Enter the number of vertex:\n";
            std::cout << "num: ";
            std::cin >> num;
            add_vertex(num);
            break;
        }

        case 4: {       // add_edge
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

        case 5: {       // list_of_all_edges
            print_edges();
            break;
        }
        
        case 6: {
            int num;
            std::cout << "Enter the number of vertex:\n";
            std::cout << "num: ";
            std::cin >> num;
            add_vertex(num);
            break;
        }
    }
 }

int Graph::size() {
    std::cout << "Size: " << std::endl;
}

int Graph::weight(int a, int b) {
    is_edge(a, b); // проверка на существование ребра
    std::cout << "Weight: " << a << " " <<  b << std::endl;
}

bool Graph::is_edge(int a, int b) {
    std::cout << "Is_edge: " << a << " " <<  b << std::endl;
}

void Graph::add_vertex(int num) {
    std::cout << "Add_vertex: " << num << std::endl;
}

void Graph::add_edge(int a, int b, int weight) {
    std::cout << "Add_edge: " << a << " " <<  b << " " << weight << std::endl;
}

void Graph::print_edges() {
    std::cout << "Print_edges: " << std::endl;
}

void Graph::list_of_edges(int num) {
    std::cout << "List_of_edges: " << num << std::endl;
}