#include <iostream>
#include <cstdlib>
#include "cmdline.h"

p_player_t next_player = nullptr;
extern int init_money;


int main() {
    srand(time(nullptr));
    init_map();
    string command;
    system("chcp 65001");
    system("cls");
    while (true) {
        show_cmd();
        command = get_cmd();
        if (command == "quit") break;
        parse_cmd(command);
        plot_map();
    }
    return 0;
}
