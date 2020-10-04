//
// Created by asterwyx on 2020/10/4.
//
#include <cstring>
#include "cmdline.h"
#include "player.h"
#include "map.h"


int parse_cmd(std::string cmd) {
    if (std::regex_match(cmd, std::regex("^preset.*"))) {
        std::string::size_type space_pos = cmd.find(' ');
        do_preset(cmd.substr(space_pos + 1));
        return 0;
    }
    return -1;

}

int do_preset(std::string cmd) {
    if (cmd.back() == '\n') {
        cmd.pop_back();
    }
    if (std::regex_match(cmd, std::regex("^user.*"))) {
        auto player_vec = get_player_vec();
        player_vec->clear();
        for (char uid : cmd) {
            add_player(uid);
        }
    } else if(std::regex_match(cmd, std::regex("^map.*"))) {
        auto first_pos = cmd.find(' ');
        auto second_pos = cmd.find(' ', first_pos + 1);
        auto map_id = cmd.substr(first_pos + 1, second_pos - first_pos - 1);
        int n_map = atoi(map_id.c_str());
        if (n_map == START_POS || n_map == HOSPITAL_POS || n_map == PROP_HOUSE_POS || n_map == GIFT_HOUSE_POS || n_map == PRISON_POS || n_map == MAGIC_HOUSE_POS) {
            return -1;
        } else {
            p_map_t map = get_map();
            auto third_pos = cmd.find(' ', second_pos + 1);
            auto player_name = cmd.substr(second_pos + 1, third_pos - second_pos - 1);
            map->at(n_map).
        }

    }
}

