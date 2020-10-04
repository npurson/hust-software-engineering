#include <iostream>
#include "map.h"
#include "player.h"
#include "cmdline.h"


extern uint64_t init_money;
p_player_t next_player;


int main() {
    srand(time(0));
    p_map_t map = init_map();
//    plot_map();
    std::string command;
    init_money = 10000;
    add_player('Q');
    next_player = &get_player_vec()->front();
    while (true) {
        show_cmd();
        if (std::getline(std::cin, command)) {
            if (command == "quit") {
                break;
            }
            parse_cmd(command);
        }
    }
    return 0;
}
