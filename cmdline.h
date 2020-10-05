#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <regex>

#include "player.h"
#include "map.h"

bool check_num(const std::string& num_str);
int parse_cmd(const std::string& cmd);
int do_preset(const std::vector<std::string> &word_vec);
int do_roll();
int do_step(int step);
int do_query(player_t& player);
int do_help();
void do_dump();
void do_robot(p_player_t player);
void do_block(int step, p_player_t player);
void do_sell(map_t& map, player_t& player, int map_node_idx);
void show_cmd();
void tolower(std::string& str);

#endif // RICH_CMDLINE_H
