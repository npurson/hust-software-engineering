#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <regex>

#include "player.h"
#include "map.h"


int parse_cmd(const string& cmd);
int do_preset(string cmd);
int do_roll();
int do_step(int step);
int do_query(player_t& player);
int do_help();
void do_dump();
void do_robot(p_player_t player);
// void do_bomb(int step, p_player_t player);
void do_block(int step, p_player_t player);
void do_sell(map_t& map, player_t& player, int map_node_idx);
void show_cmd();
void tolower(string& str);

#endif // RICH_CMDLINE_H
