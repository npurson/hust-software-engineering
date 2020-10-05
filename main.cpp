#include <iostream>
#include "cmdline.h"
#include <cstdlib>

extern int init_money;

p_player_t next_player = nullptr;


int main() {
    srand(time(nullptr));
    init_map();
    string command;
    system("chcp 65001");
    system("cls");
    while (true) {
    // skip_player(next_player);
        show_cmd();
        if (getline(cin, command)) {
            if (command == "quit") {
                break;
            } else {
                parse_cmd(command);
            }
        }
        plot_map();
    }
    return 0;
}
