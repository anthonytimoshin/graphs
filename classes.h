#ifndef GRAPHS_H
#define GRAPHS_H

#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <unistd.h> // sleep (потом убрать)

class Graph {
public:
    std::string path;                           // путь к файлу, в котором хранится граф
    int type;                                   // хранит тип графа - ориентированнный/неориентированный
    std::vector<std::string> function_items = { // список функционала для меню
        "Size", 
        "Weight", 
        "Is_Edge", 
        "Add_Vertex", 
        "Add_Edge", 
        "Print_Edges",
        "List_of_Edges_of_a_Vertex",
        "Print_Vertexes",
        "Create_Empty_Graph",
        "Read_Graph_From_File",
        "Remove_Vertex",
        "Remove_Edge",
        "Check_Connectivity",
        "Print_Connectivity_Components",
        "Distance_Between_Vertexes",
        "All_Distances_Between_Vertexes",
        "Spanning_Tree"
    };

public:
    void get_graph();                           // принимает путь к файлу, где хранится граф
    bool check_type();                          // определяет тип графа (TRUE - ориентированный, FALSE - неориентированный)
    void open_graph();                          // ? функция открытия файла
    void close_graph();                         // ? функция закрытия файла
    void func_handler(int selected);            // обработчик функций
    int size();                                 // возвращает количество вершин в графе
    int weight(int a, int b);                   // возвращает вес ребра/дуги
    bool is_edge(int a, int b);                 // проверка на наличие ребра
    void add_vertex(int num);                   // добавить вершину
    void add_edge(int a, int b, int weight);    // добавить ребро 
    void print_edges();                         // список всех ребер
    void list_of_edges(int num);                // список ребер входящих/выходящих в/из вершины
    void print_vertexes();                      // печать всех вершин
    void create_empty_graph();                  // создать пустой граф
    void read_graph_from_file();                // считать граф из файла
    void remove_vertex(int num);                // удалить вершину
    void remove_edge(int a, int b);             // удалить ребро
    bool check_connectivity();                  // проверить на связность
    void print_connectivity_components();       // вывести компоненты связности графа/компоненты слабой связности орграфа
    int distance(int a, int b);                 // найти кратчайший путь и его длину между двумя вершинами 
    int all_distances(int num);                 // найти расстояние от вершины до всех остальных вершин
    int spanning_tree();                        // найти минимальное остовное дерево
};

class Menu {
public:
    void menu_init();
    int menu(std::vector<std::string> menu_items);
    bool key_handler(int key, int& selected, int menu_items_size);
    void menu_end(int selected);
    void press_enter();
};

#endif // GRAPHS_H
