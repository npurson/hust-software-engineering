#include <iostream>
#include "map.h"
#include "player.h"

p_player_t next_player;


int main() {
    srand(time(0));

    p_map_t map = init_map();
    plot_map();

    return 0;
}
