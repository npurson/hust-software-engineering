#ifndef RICH_PLAYERS_H
#define RICH_PLAYERS_H

#include <cstdint>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::uint8_t;
using std::uint64_t;


enum color_t { RED, GREEN, BLUE, YELLOW };

typedef struct map_node map_node_t, * p_map_node_t;

typedef struct player {
    char uid;
    string name;
    uint64_t n_money;
    uint64_t n_points;
    uint8_t n_pos;
    uint8_t n_empty_rounds;
    uint8_t n_god_buff;
    color_t e_color;
    vector<p_map_node_t> estate;
    uint8_t n_block;
    uint8_t n_boom;
    uint8_t n_robot;
    uint8_t b_sell_estate; // TODO set 0 in the beginning of every round
} player_t, *p_player_t;

void add_player(char uid);

p_player_t get_player_by_uid(char uid);

vector<player_t> *get_player_vec();

#endif // RICH_PLAYERS_H
