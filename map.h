#ifndef RICH_MAP_H
#define RICH_MAP_H

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <vector>
#include <windows.h>

#include "player.h"

using std::vector;
using std::uint8_t;
using std::uint16_t;


// 地图结点类型
enum NodeType { START, VACANCY, ITEM_HOUSE, GIFT_HOUSE, MAGIC_HOUSE, HOSPITAL, PRISON, MINE };
// 房产等级
enum EstateLevel { WASTELAND, HUT, HOUSE, SKYSCRAPER };
// 玩家索引
enum PlayerIdx { NONE, PLAYER_1, PLAYER_2, PLAYER_3 };
// 道具类型
enum ItemType { BLOCK = 1, BOMB, ROBOT };

typedef struct map_node {
    uint8_t id;                 // 地块序号，用于查询角色拥有的地块
    uint8_t type;               // 地图结点类型，枚举类型为NodeType
    uint8_t estate_lvl;         // 房产等级，type==VACANCY时有效。枚举类型为EstateLevel
    uint8_t owner;              // 房产的拥有者，type==VACANCY时有效。枚举类型为PlayerIdx
    vector<uint8_t> players;    // 当前处于该结点的玩家。枚举类型为PlayerIdx
    uint8_t item;               // 当前置于该结点的道具。枚举类型为ItemType或None
    uint16_t value;             // type==VACANCY时为空地价格，计算房产价值时乘以（房产等级+1）；type==MINE时为可获取的点数

    map_node(uint8_t t) :type(t), estate_lvl(), owner(), players(), item(), value() {}
    map_node(uint8_t t, uint16_t v) :type(t), value(v), estate_lvl(), owner(), players(), item() {}
} map_node_t, * p_map_node_t;

typedef vector<map_node_t> map_t, * p_map_t;


// 初始化地图
map_t init_map();
// 读取地图
p_map_t get_map();
// 绘制地图
void plot_map();
// 购买房产
void buy_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx);
// 升级房产
void update_estate(map_t& map, uint8_t player_idx, uint8_t map_node_idx);
// 卖出房产
void sell_estate(map_t& map, player_t& player, uint8_t map_node_idx);
// 使用道具
void apply_item(map_t& map, uint8_t item_type, uint8_t pos);
// 投骰子，行走过程中判定道具、地段、破产  TODO 破产处理应在主调函数中？
void roll_dice(map_t& map, player_t& player);

#endif // RICH_MAP_H
