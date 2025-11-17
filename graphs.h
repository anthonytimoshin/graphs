#ifndef GRAPHS_H
#define GRAPHS_H

#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <vector>
#include <string>

class Graph {
public:
    std::string path;
    int type;
    std::vector<std::string> function_items = {
        "Size",
        "Weight",
        "Is_Edge",
        "Add_Vertex",
        "Add_Edge",
        "List_of_Edges"
    };

public:
    void get_graph();
    void open_graph();
    int size();
    int weight(int a, int b);
    bool is_edge(int a, int b);
    void add_vertex(int num);
    void add_edge(int a, int b);
    void list_of_edges(int num);
};

class Menu {
public:
    void menu_init();
    int menu(std::vector<std::string> menu_items);
    bool key_handler(int key, int& selected, int menu_items_size);
    void menu_end(int selected);
};

#endif // GRAPHS_H
