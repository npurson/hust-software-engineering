#include "map.h"
#include "player.h"


map_t init_map()
{
    map_t map;
    for (int i = 0; i < kMapSize; ++i) {
        switch (i) {
            case kStartPos: map.emplace_back(START); break;
            case kHospitalPos: map.emplace_back(HOSPITAL); break;
            case kItemHousePos: map.emplace_back(ITEM_HOUSE); break;
            case kGiftHousePos: map.emplace_back(GIFT_HOUSE); break;
            case kPrisonPos: map.emplace_back(PRISON); break;
            case kMagicHousePos: map.emplace_back(MAGIC_HOUSE); break;
            case 64: map.emplace_back(MINE, 60); break;
            case 65: map.emplace_back(MINE, 80); break;
            case 66: map.emplace_back(MINE, 40); break;
            case 67: map.emplace_back(MINE, 100); break;
            case 68: map.emplace_back(MINE, 20); break;
            default:
                // 地段1
                if (i > kStartPos && i < kItemHousePos && i != kHospitalPos)
                    map.emplace_back(VACANCY, 200);
                // 地段2
                else if (i > kItemHousePos && i < kGiftHousePos)
                    map.emplace_back(VACANCY, 500);
                // 地段3
                else if (i > kGiftHousePos && i <= kMagicHousePos && i != kPrisonPos)
                    map.emplace_back(VACANCY, 300);
        }
    }
    return map;
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


void sell_estate(map_t& map, player_t& player, uint8_t map_node_idx)
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
   for (int dice = rand() % max + 1; dice; --dice) {
       player.n_pos = (player.n_pos + 1) % kMapSize;
       // 道具判定
       if (map[player.n_pos].item == BLOCK) {
           map[player.n_pos].item = NONE;
           break;
       }
       else if (map[player.n_pos].item == BOMB) {
           map[player.n_pos].item = NONE;
           player.n_pos = kHospitalPos;
           player.n_empty_rounds = 3
       }
   }
   // 租金判定
   map[player.n_pos].players.push_back();
}
