#include "map.h"


map_t init_map()
{
    map_t map;
    for (int i = 0; i < 70; ++i) {
        switch (i) {
            case 0: map.emplace_back(START); break;
            case 14: map.emplace_back(HOSPITAL); break;
            case 28: map.emplace_back(ITEM_HOUSE); break;
            case 35: map.emplace_back(GIFT_HOUSE); break;
            case 49: map.emplace_back(PRISON); break;
            case 63: map.emplace_back(MAGIC_HOUSE); break;
            case 64: map.emplace_back(MINE, 60); break;
            case 65: map.emplace_back(MINE, 80); break;
            case 66: map.emplace_back(MINE, 40); break;
            case 67: map.emplace_back(MINE, 100); break;
            case 68: map.emplace_back(MINE, 20); break;
            default:
                // 地段1
                if (i >= 1 && i <=27 && i != 14)
                    map.emplace_back(VACANCY, 200);
                // 地段2
                else if (i >= 29 && i <=34)
                    map.emplace_back(VACANCY, 500);
                    // 地段3
                else if (i >= 36 && i <= 62 && i != 49)
                    map.emplace_back(VACANCY, 300);
        }
    }
    return map;
}


//void plot_map()
//{
//
//}
//
//
//void buy_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
//{
//
//}
//
//
//void update_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
//{
//
//}
//
//
//void sell_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx)
//{
//
//}
//
//
//void apply_item(map_t& map, uint8_t item_type, uint8_t pos)
//{
//
//}
//
//
//void roll_dice(map_t& map, player_t& player)
//{
//    for (int dice = rand() % max + 1; dice; --dice) {
//
//    }
//}
