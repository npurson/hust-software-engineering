#ifndef RICH_MAP_H
#define RICH_MAP_H

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <vector>
#include <windows.h>

#include "player.h"

#define AREA_1_PRICE        200
#define AREA_2_PRICE        500
#define AREA_3_PRICE        300

#define START_POS           0
#define HOSPITAL_POS        14
#define ITEM_HOUSE_POS      28
#define GIFT_HOUSE_POS      35
#define PRISON_POS          49
#define MAGIC_HOUSE_POS     63
#define MAP_SIZE            70

using std::getline;


// 地图结点类型
enum NodeType { START, VACANCY, ITEM_HOUSE, GIFT_HOUSE, MAGIC_HOUSE, HOSPITAL, PRISON, MINE, PARK };
// 房产等级
enum EstateLevel { WASTELAND, HUT, HOUSE, SKYSCRAPER };
// 道具类型
enum ItemType { NONE, BLOCK, BOMB, ROBOT };

typedef struct map_node {
    int id;
    int type;                       // 地图结点类型，枚举类型为NodeType
    int estate_lvl;                 // 房产等级，type==VACANCY时有效。枚举类型为EstateLevel
    p_player_t owner;               // 房产的拥有者，type==VACANCY时有效。枚举类型为PlayerIdx
    vector<p_player_t> players;     // 当前处于该结点的玩家。枚举类型为PlayerIdx
    int item;                       // 当前置于该结点的道具。枚举类型为ItemType或None
    int value;                      // type==VACANCY时为空地价格，计算房产价值时乘以（房产等级+1）；type==MINE时为可获取的点数

    map_node(int i, int t) :id(i), type(t), estate_lvl(), owner(), players(), item(), value() {}
    map_node(int i, int t, int v) :id(i), type(t), value(v), estate_lvl(), owner(), players(), item() {}
} map_node_t, * p_map_node_t;

typedef vector<map_node_t> map_t, * p_map_t;


// 初始化地图
p_map_t init_map();
// 读取地图
p_map_t get_map();
// 绘制地图
void plot_map();
// 购买房产
void buy_estate(map_t& curr_map, player_t& player);
// 升级房产
void update_estate(map_t& curr_map, player_t& player);
// 使用道具
void apply_item(map_t& curr_map, player_t& player, int item, int pos = 0);
// 购买道具
void buy_item(player_t& player);
// 获得礼物
void get_gift(player_t& player);
// 投骰子，并调用step_forward
bool roll_dice(map_t& curr_map, player_t& player);
// 向前行进指定步数，返回true则破产
bool step_forward(map_t& curr_map, player_t& player, int steps);
// 获取某地房产的价值，返回true则破产
int get_estate_price(const map_node_t& map_node);
// 使用魔法
void magic_house();
// 支付租金
bool pay_rent(p_player_t player);

#endif // RICH_MAP_H
