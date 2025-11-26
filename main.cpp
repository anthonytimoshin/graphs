#include <iostream>
#include "classes.h"

int main() {
	Menu menu;
	Graph graph;
	
	// graph.get_graph();
	while (true) {
		int selected = menu.menu(graph.function_items);
		if (selected == -1) {
			break;
		}
		graph.func_handler(selected); 
		menu.press_enter();
	}
	
	return 0;
}
