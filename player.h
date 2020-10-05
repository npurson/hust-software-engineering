#ifndef RICH_PLAYERS_H
#define RICH_PLAYERS_H

#include <cstdint>
#include <vector>
#include <string>
#include <map>

#define DEFAULT_MONEY   10000

using std::vector;
using std::string;


enum color_t { RED, GREEN, BLUE, YELLOW };

typedef struct map_node map_node_t, * p_map_node_t;

typedef struct player {
    char uid;                       // 标识角色的字符, A,Q,S,J
    string name;                    // 角色的名字, 如钱夫人
    int n_money;                    // 玩家的资金
    int n_points;                   // 玩家的点数
    int n_pos;                      // 玩家在地图上的位置
    int n_empty_rounds;             // 玩家轮空的轮数
    int n_god_buff;                 // 玩家现有的财神buff的轮数
    color_t e_color;                // 玩家显示的颜色
    vector<p_map_node_t> estate;    // 玩家的地产
    int n_block;                    // 玩家的路障道具数量
    int n_bomb;                     // 玩家的炸弹道具数量
    int n_robot;                    // 玩家的机器娃娃道具数量
    int b_sell_estate;
} player_t, * p_player_t;

void add_player(char uid);
vector<player_t>* get_player_vec();
p_player_t get_player_by_uid(char uid);
p_player_t skip_player(p_player_t next_player);

#endif // RICH_PLAYERS_H
