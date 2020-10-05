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
void do_robot(std::int step, p_player_t player);
void do_bomb(std::int step, p_player_t player);
void do_block(std::int step, p_player_t player);
void do_sell(map_t& map, player_t& player, int map_node_idx);
int do_query(player_t& player);
int do_help();
void tolower(std::string &str);
void show_cmd();
int do_step(std::int step);

#endif //RICH_CMDLINE_H
