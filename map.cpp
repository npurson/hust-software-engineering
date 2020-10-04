#include "map.h"


map_t init_map()
{
    map_t map;
    for (int i = 0; i < 70; ++i) {
        // 地段1
        if (i >= 1 && i <=27 && i != 14) {

        }
        // 地段2
        else if (i >= 29 && i <=34) {

        }
        // 地段3
        else if (i >= 36 && i <= 62 && i != 49) {

        }
        // 起点
        else if (i == 0) {

        }
        else if (i == 14) {

        }
        else if (i == )
    }
}


void plot_map()
{

}


void buy_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
{

}


void update_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
{

}


void sell_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
{

}


void apply_item(map_t& map, uint8_t item_type, uint8_t pos)
{

}


void roll_dice(map_t& map, player_t& player)
{
    for (int dice = rand() % max + 1; dice; --dice) {

    }
}
