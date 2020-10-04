#ifndef RICH_CMDLINE_H
#define RICH_CMDLINE_H

#include <string>
#include <regex>
#include "player.h"

int parse_cmd(string cmd);
int do_preset(string cmd);
int do_roll();
int do_block(uint8_t pos);
int do_sell(uint8_t pos);
int do_bomb(uint8_t pos);
int do_robot(uint8_t pos);
int do_query();
int do_help();
int do_quit();
int do_step(uint8_t step);
void do_dump();
void show_cmd();

#endif //RICH_CMDLINE_H
