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

void Graph::get_graph() {
    std::cout << "Enter the path to the graph: ";
    std::cin >> path;
    std::cout <<"\nEnter the graph type (1 - directed, 2 - undirected): ";
    std::cin >> type;
    std::cout << "\n";
}

void Graph::open_graph() {
    std::fstream graph(path);
    if (!graph.is_open()) {
        std::cout << "Error! File can't be opened";
    }
}

void Menu::menu_init() {
    initscr();              // инициализация ncurses
    noecho();               // убираем эхо-печать нажатых клавиш
    cbreak();               // немедленная реакция на нажатие клавиш
    keypad(stdscr, TRUE);   // подключение обработки стрелок
    curs_set(0);            // скрывает мигающий курсор (потестить 0, 1, 2)
}

int Menu::menu(std::vector<std::string> menu_items) {
    menu_init();

    int selected = 0;       // текущий выбранный пункт
    int key;                // переменная для хранения нажатой клавиши
    int menu_items_size = menu_items.size();

    while (true) {
        clear();            // очистка экрана каждый раз перед отрисовкой
        mvprintw(5, 10, "Function menu");
        for (int i = 0; i < menu_items_size; i++) {
            move(7 + i, 12);
            if (i == selected) {
                attron(A_REVERSE);      // инверсия цветов выбранного пункта
                printw("> %s", menu_items[i].c_str());
                attroff(A_REVERSE);
            } else {
                printw("  %s", menu_items[i].c_str());
            }
        }

        mvprintw(15, 5, "Use UP/DOWN arrows to move");
        mvprintw(16, 5, "Press ENTER to choose or press ESC to exit");

        refresh();              // обновляем экран
        key = getch();          // ожидание нажатия клавиши

        if (key_handler(key, selected, menu_items_size) == false) {
            break;
        }
    }

    menu_end(selected);
    return selected;
}

bool Menu::key_handler(int key, int& selected, int menu_items_size) {
    switch (key) {
        case KEY_UP:        // перемещаемся выше
            if (selected > 0) {
                selected--; 
            }
            return true;

        case KEY_DOWN:      // перемещаемся ниже
            if (selected < menu_items_size - 1) {
                selected++; 
            }
            return true;

        case '\n': 
        case KEY_ENTER:     // окончательный выбор и выход из меню
            return false;

        case 27:            // выход из меню
            selected = -1;
            return false;

        default:            // при нажатии на другую клавишу программа продолжает работу
            return true;
    }
}

void Menu::menu_end(int selected) {
    endwin();
    if (selected != -1) {
        std::cout << "You selected option №" << selected << std::endl;
    } else {
        std::cout << "You interrupted the program execution." << std::endl;
    }
}

int Graph::size() {

}

#endif // GRAPHS_H
