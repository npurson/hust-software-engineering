//
// Created by asterwyx on 2020/10/4.
//

#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <string>
#include <regex>
#include "player.h"
#include "map.h"

int parse_cmd(std::string cmd);
int do_preset(std::string cmd);
int do_roll();
void do_dump();
int do_item(map_t& map, player_t& player);
int do_query(player_t& player);
int do_help();
int do_quit();
void show_cmd();


#endif //RICH_CMDLINE_H
