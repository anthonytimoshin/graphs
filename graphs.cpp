#include "graphs.h"

void Graph::get_graph() {
    std::cout << "Enter the path to the graph: ";
    std::cin >> path;
    std::cout << "\nEnter the graph type (1 - directed, 2 - undirected): ";
    std::cin >> type;
    std::cout << "\n";
}

void Graph::open_graph() {
    std::fstream graph(path);
    if (!graph.is_open()) {
        std::cout << "Error! File can't be opened";
    }
}

int Graph::size() {

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
            }
            else {
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
    }
    else {
        std::cout << "You interrupted the program execution." << std::endl;
    }
}
