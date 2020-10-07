#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <regex>

#include "player.h"
#include "map.h"

#define SLEEP_TIME  2000


string get_cmd();
bool check_num(string num_str);
int parse_cmd(const string& cmd);
int do_preset(const vector<string>& word_vec);
int do_roll();
int do_step(int step);
int do_query(player_t& player);
int do_help();
void do_dump();
void do_robot(p_player_t player);
void do_block(int step, p_player_t player);
void do_sell(map_t& map, player_t& player, int map_node_idx);
void do_skip(p_player_t player);
void show_cmd();
void tolower(string& str);
void erase_player_from_curr_pos(p_player_t p_player);
void switch_player(p_player_t *p_next_player);

#endif // RICH_CMDLINE_H
