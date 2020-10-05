#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <string>
#include <regex>
#include "player.h"
#include "map.h"

int parse_cmd(const std::string& cmd);
void tolower(std::string &str);
int do_preset(std::string cmd);
int do_roll();
void do_dump();
void do_robot(p_player_t player);
void do_bomb(int step, p_player_t player);
void do_block(int step, p_player_t player);
void do_sell(map_t& map, player_t& player, int map_node_idx);
int do_query(player_t& player);
int do_help();
void show_cmd();
int do_step(int step);

#endif //RICH_CMDLINE_H
