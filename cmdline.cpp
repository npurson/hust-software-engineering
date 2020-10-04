//
// Created by asterwyx on 2020/10/4.
//
#include <cstdlib>
#include "cmdline.h"
#include "player.h"
#include "map.h"
#include <iostream>

extern p_player_t next_player;

std::vector<std::string> split_cmd(std::string cmd) {
    std::vector<std::string> word_vec;
    while (!cmd.empty()) {
        auto space_pos = cmd.find(' ');
        std::string word;
        if (space_pos == std::string::npos) {
            word = cmd;
            cmd = "";
        } else {
            word = cmd.substr(0, space_pos);
            cmd = cmd.substr(space_pos + 1);
        }
        word_vec.push_back(word);
    }
    return word_vec;
}

int parse_cmd(std::string cmd) {
    std::vector<std::string> word_vec = split_cmd(cmd);
    if (std::regex_match(cmd, std::regex("^preset.*"))) {
        std::string::size_type space_pos = cmd.find(' ');
        do_preset(cmd.substr(space_pos + 1));
        return 0;
    } else if (std::regex_match(cmd, std::regex("Roll"))) {
        do_roll();
    } else if (word_vec[0] == "dump") {
        do_dump();
    }
    return -1;

}

int do_roll() {
    return 0;
}

void do_dump() {
    std::string dump_text = "user ";
    auto player_vec = get_player_vec();

    for (const auto& player : *player_vec) {
        dump_text += player.uid;
    }
    std::cerr << dump_text << std::endl;
    for (const auto& player : *player_vec) {
        for (const auto& p_estate : player.estate) {
            std::cerr << "map " << p_estate->id << " " << player.uid << " " << p_estate->estate_lvl << std::endl;
        }
        std::cerr << "fund " << player.uid << " " << player.n_money << std::endl;
        std::cerr << "credit " << player.uid << " " << player.n_points << std::endl;
        std::cerr << "userloc " << player.uid << " " << player.n_pos  << " "  << player.n_empty_rounds << std::endl;
        if (player.n_boom != 0) {
            std::cerr << "gift " << player.uid << " bomb " << static_cast<int>(player.n_boom) << std::endl;
        }
        if (player.n_block != 0) {
            std::cerr << "gift " << player.uid << " barrier " << static_cast<int>(player.n_block) << std::endl;
        }
        if (player.n_robot != 0) {
            std::cerr << "gift " << player.uid << " robot " << static_cast<int>(player.n_robot) << std::endl;
        }
        if (player.n_god_buff != 0) {
            std::cerr << "gift " << player.uid << " god " << static_cast<int>(player.n_god_buff) << std::endl;
        }
    }
    auto map = get_map();
    for (const auto& map_node : *map) {
        switch(map_node.item) {
            case BOMB:
                std::cerr << "bomb " << map_node.id << std::endl;
                break;
            case BLOCK:
                std::cerr << "barrier " << map_node.id << std::endl;
                break;
            case NONE:
            default:
                break;
        }
    }
    std::cerr << "nextuser " << next_player->uid << std::endl;
    exit(EXIT_SUCCESS);
}

void show_cmd() {
    std::cout << std::endl;
    std::cout << next_player->name << ">";
}


int do_preset(std::string cmd) {
    if (cmd.back() == '\n') {
        cmd.pop_back();
    }
    auto word_vec = split_cmd(cmd);
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
        p_map_t map = nullptr;
        if (n_map == START_POS || n_map == HOSPITAL_POS || n_map == ITEM_HOUSE_POS || n_map == GIFT_HOUSE_POS || n_map == PRISON_POS || n_map == MAGIC_HOUSE_POS) {
            return -1;
        } else {
            map = get_map();
            char player_name = cmd[second_pos + 1];
            map->at(n_map).owner = get_player_by_uid(player_name);
        }
        auto third_pos = cmd.find(' ', second_pos + 1);
        auto level_str = cmd.substr(third_pos + 1);
        int level = atoi(level_str.c_str());
        map->at(n_map).estate_lvl = level;
    } else if (std::regex_match(cmd, std::regex("^fund.*"))) {
        auto first_pos = cmd.find(' ');
        char player_name = cmd[first_pos + 1];
        auto player = get_player_by_uid(player_name);
        auto second_pos = cmd.find(' ');
        auto money_string = cmd.substr(second_pos + 1);
        int money = atoi(money_string.c_str());
        player->n_money = money;
    } else if (std::regex_match(cmd, std::regex("^credit.*"))) {
        auto first_pos = cmd.find(' ');
        char player_name = cmd[first_pos + 1];
        auto player = get_player_by_uid(player_name);
        auto second_pos = cmd.find(' ');
        auto point_string = cmd.substr(second_pos + 1);
        int points = atoi(point_string.c_str());
        player->n_points = points;
    } else if (std::regex_match(cmd, std::regex("^gift.*"))) {
        auto first_pos = cmd.find(' ');
        char player_name = cmd[first_pos + 1];
        auto player = get_player_by_uid(player_name);
        auto second_pos = cmd.find(' ', first_pos + 1);
        auto third_pos = cmd.find(' ', second_pos + 1);
        auto prop_name = cmd.substr(second_pos + 1, third_pos - second_pos - 1);
        auto number_str = cmd.substr(third_pos + 1);
        int number = atoi(number_str.c_str());
        if (prop_name == "bomb") {
            player->n_boom = number;
        } else if (prop_name == "barrier") {
            player->n_block = number;
        } else if (prop_name == "robot") {
            player->n_robot = number;
        } else if (prop_name == "god") {
            player->n_god_buff = number;
        } else {
            return -1;
        }
    } else if (std::regex_match(cmd, std::regex("^user_loc.*"))) {
        char player_name = word_vec[1][0];
        int n_map_id = atoi(word_vec[2].c_str());
        int rest_days = atoi(word_vec[3].c_str());
        auto player = get_player_by_uid(player_name);
        player->n_empty_rounds = rest_days;
        player->n_pos = n_map_id;
    } else if (std::regex_match(cmd, std::regex("^nextuser.*"))) {
        char player_name = word_vec[1][0];
        next_player = get_player_by_uid(player_name);
    } else {
        int map_id = atoi(word_vec[1].c_str());
        auto map = get_map();
        if (word_vec[0] == "bomb") {
            map->at(map_id).item = BOMB;
        } else if (word_vec[0] == "barrier") {
            map->at(map_id).item = BLOCK;
        }
    }
    return 0;
}

