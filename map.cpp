#include "map.h"
#include "player.h"


map_t init_map()
{
    map_t map;
    for (int i = 0; i < MAP_SIZE; ++i) {
        switch (i) {
            case START_POS: map.emplace_back(START); break;
            case HOSPITAL_POS: map.emplace_back(HOSPITAL); break;
            case ITEM_HOUSE_POS: map.emplace_back(ITEM_HOUSE); break;
            case GIFT_HOUSE_POS: map.emplace_back(GIFT_HOUSE); break;
            case PRISON_POS: map.emplace_back(PRISON); break;
            case MAGIC_HOUSE_POS: map.emplace_back(MAGIC_HOUSE); break;
            case 64: map.emplace_back(MINE, 60); break;
            case 65: map.emplace_back(MINE, 80); break;
            case 66: map.emplace_back(MINE, 40); break;
            case 67: map.emplace_back(MINE, 100); break;
            case 68: map.emplace_back(MINE, 20); break;
            default:
                // 地段1
                if (i > START_POS && i < ITEM_HOUSE_POS && i != HOSPITAL_POS)
                    map.emplace_back(VACANCY, 200);
                // 地段2
                else if (i > ITEM_HOUSE_POS && i < GIFT_HOUSE_POS)
                    map.emplace_back(VACANCY, 500);
                // 地段3
                else if (i > GIFT_HOUSE_POS && i <= MAGIC_HOUSE_POS && i != PRISON_POS)
                    map.emplace_back(VACANCY, 300);
        }
    }
    return map;
}


void plot_map()
{

}


void buy_estate(map_t& map, player_t& player)
{

}


void update_estate(map_t& map, player_t& player)
{

}


void sell_estate(map_t& map, player_t& player)
{
    if (player.b_sell_estate == 1 ||
        map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner != player.uid)
    {
        printf("卖出失败");
        return;
    }

    player.b_sell_estate = 1;
    player.n_money += map[map_node_idx].value;
    for (int i = 0; i < player.estate.size(); ++i){
        if (player.estate.id == map_node_idx){
            player.estate.erase(i);
            break;
        }
    }

    map[map_node_idx].owner = NONE;
    map[map_node_idx].estate_lvl = WASTELAND;
    if (map_node_idx < 28)  map[map_node_idx].value = 200;
    else if (map_node_idx < 35) map[map_node_idx].value = 500;
    else    map[map_node_idx].value = 300;

    printf("卖出成功");
}


void apply_item(map_t& map, uint8_t item_type, uint8_t pos)
{

}


void roll_dice(map_t& map, player_t& player)
{
    step_forward(map, player, rand() % 6 + 1);
}


void step_forward(map_t& map, player_t& player, uint8_t steps)
{
    while (steps--) {
        player.n_pos = (player.n_pos + 1) % MAP_SIZE;
        // 道具判定
        if (map[player.n_pos].item == BLOCK) {
            map[player.n_pos].item = NONE;
            break;
        }
        else if (map[player.n_pos].item == BOMB) {
            map[player.n_pos].item = NONE;
            player.n_pos = HOSPITAL_POS;
            player.n_empty_rounds = 3;
            cout << "Bomb! 移动至医院" << endl;
        }
    }
    // 租金判定
    map[player.n_pos].players.push_back(&player);
    if (map[player.n_pos].owner) {

    }
}