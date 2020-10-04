//
// Created by asterwyx on 2020/10/4.
//

#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <string>
#include <regex>

int parse_cmd(std::string cmd);
int do_preset(std::string cmd);
int do_roll();
int do_block(std::uint8_t pos);
int do_sell(std::uint8_t pos);
int do_bomb(std::uin8_t pos);
int do_robot(std::uint8_t pos);
int do_query();
int do_help();
int do_quit();
int do_step(uint8_t step);


#endif //RICH_CMDLINE_H
