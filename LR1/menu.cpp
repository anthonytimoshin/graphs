#include "classes.h"

void Menu::menu_init() {
    initscr();              // инициализация ncurses
    noecho();               // убираем эхо-печать нажатых клавиш
    cbreak();               // немедленная реакция на нажатие клавиш
    keypad(stdscr, TRUE);   // подключение обработки стрелок
    curs_set(0);            // скрывает мигающий курсор
}

int Menu::menu(std::vector<std::string> menu_items) {
    menu_init();

    int selected = 0;       // текущий выбранный пункт
    int key;                // переменная для хранения нажатой клавиши
    int menu_items_size = menu_items.size();

    while (true) {
        clear();            // очистка экрана каждый раз перед отрисовкой
        mvprintw(1, 5, "Function menu");
        for (int i = 0; i < menu_items_size; i++) {
            move(3 + i, 7);
            if (i == selected) {
                attron(A_REVERSE);      // инверсия цветов выбранного пункта
                printw("> %s", menu_items[i].c_str());
                attroff(A_REVERSE);
            } else {
                printw("  %s", menu_items[i].c_str());
            }
        }

        mvprintw(4 + menu_items_size, 5, "Use UP/DOWN arrows to move");
        mvprintw(5 + menu_items_size, 5, "Press ENTER to choose or press ESC to exit");

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
    if (selected == -1) {
        std::cout << "You interrupted the program execution." << std::endl;
    }
}

void Menu::press_enter() {
    std::cout << std::endl << "Press ENTER to continue";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
