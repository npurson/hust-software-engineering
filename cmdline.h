//
// Created by asterwyx on 2020/10/4.
//

#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <string>
#include <regex>
#include "player.h"

int parse_cmd(const std::string& cmd);
int do_preset(std::string cmd);
int do_roll();
void do_dump();
int do_block(std::uint8_t pos);
int do_sell(std::uint8_t pos);
int do_bomb(std::uint8_t pos);
int do_robot(std::uint8_t pos);
int do_step(std::uint8_t step);
int do_query();
int do_help();
int do_quit();
void show_cmd();


#endif //RICH_CMDLINE_H
