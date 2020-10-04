#include <iostream>
#include <cstdlib>
#include <cctype>
#include "cmdline.h"
#include "player.h"
#include "map.h"

extern p_player_t next_player;


void start_game() {
    std::cout << "请选择参与的玩家数量(2-4人): ";
}

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

void tolower(std::string &str) {
    for (auto& c : str) {
        if (std::isalpha(c)) {
            c = tolower(c);
        }
    }
}

int parse_cmd(const std::string& cmd) {
    static bool start = false;
    std::vector<std::string> word_vec = split_cmd(cmd);
    tolower(word_vec[0]);
    if (!start) {
        if (word_vec[0] == "preset") {
            std::string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            start = true;
            return 0;
        } else if (word_vec[0] == "start") {
            if (word_vec.size() != 1) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            start_game();
            start = true;
        } else {
            std::cerr << "无效的命令" << std::endl;
        }
    } else {
        if (word_vec[0] == "preset") {
            std::string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            return 0;
        } else if (word_vec[0] == "roll") {
            if (word_vec.size() != 1) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            do_roll();
        } else if (word_vec[0] == "dump") {
            if (word_vec.size() != 1) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            do_dump();
        } else if (word_vec[0] == "sell") {
            if (word_vec.size() != 2) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            int map_id = atoi(word_vec[1].c_str());
            sell_estate(*get_map(), *next_player, map_id);
        } else if (word_vec[0] == "block") {
            if (word_vec.size() != 2) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            int block_step = atoi(word_vec[1].c_str());
            do_block(block_step);
        } else if (word_vec[0] == "bomb") {
            if (word_vec.size() != 2) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            int bomb_step = atoi(word_vec[1].c_str());
            do_bomb(bomb_step);
        } else if (word_vec[0] == "robot") {
            if (word_vec.size() != 1) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            do_robot(next_player->n_pos);
        } else if (word_vec[0] == "step") {
            if (word_vec.size() != 2) {
                std::cerr << "无效的命令" << std::endl;
                return -1;
            }
            std::uint8_t step = atoi(word_vec[1].c_str());
            do_step(step);
        }
    }

    return -1;
}

int do_roll() {
    roll_dice(*get_map(), *next_player);
    return 0;
}

int do_block(std::uint8_t pos){
    return 0;
}
int do_sell(std::uint8_t pos){
    return 0;
}
int do_bomb(std::uint8_t pos){
    return 0;
}
int do_robot(std::uint8_t pos){
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
            std::cerr << "map " << static_cast<int>(p_estate->id) << " " << player.uid << " " << static_cast<int>(p_estate->estate_lvl) << std::endl;
        }
        std::cerr << "fund " << player.uid << " " << player.n_money << std::endl;
        std::cerr << "credit " << player.uid << " " << player.n_points << std::endl;
        std::cerr << "userloc " << player.uid << " " << static_cast<int>(player.n_pos) << " "  << static_cast<int>(player.n_empty_rounds) << std::endl;
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
                std::cerr << "bomb " << static_cast<int>(map_node.id) << std::endl;
                break;
            case BLOCK:
                std::cerr << "barrier " << static_cast<int>(map_node.id) << std::endl;
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
    if (next_player != nullptr) {
        std::cout << next_player->name;
    }
    std::cout << ">";
}

int do_step(std::uint8_t step) {
    step_forward(*get_map(), *next_player, step);
    return 0;
}

int do_preset(std::string cmd) {
    if (cmd.back() == '\n') {
        cmd.pop_back();
    }
    auto word_vec = split_cmd(cmd);
    if (word_vec[0] == "user") {
        auto player_vec = get_player_vec();
        player_vec->clear();
        for (char uid : word_vec[1]) {
            add_player(uid);
        }
        next_player = &player_vec->front();
    } else if(word_vec[0] == "map") {
        int n_map = atoi(word_vec[1].c_str());
        p_map_t map;
        if (n_map == START_POS || n_map == HOSPITAL_POS || n_map == ITEM_HOUSE_POS || n_map == GIFT_HOUSE_POS || n_map == PRISON_POS || n_map == MAGIC_HOUSE_POS) {
            return -1;
        } else {
            map = get_map();
            char player_name = word_vec[2].front();
            if (map->at(n_map).owner == get_player_by_uid(player_name)) {
                return -1;
            }
            map->at(n_map).owner = get_player_by_uid(player_name);
        }
        int level = atoi(word_vec[3].c_str());
        map->at(n_map).estate_lvl = level;
        map->at(n_map).owner->estate.push_back(&map->at(n_map));
    } else if (word_vec[0] == "fund") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        int money = atoi(word_vec[2].c_str());
        player->n_money = money;
    } else if (word_vec[0] == "credit") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        int points = atoi(word_vec[2].c_str());
        player->n_points = points;
    } else if (word_vec[0] == "gift") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        auto prop_name = word_vec[2];
        int number = atoi(word_vec[3].c_str());
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
    } else if (word_vec[0] == "userloc") {
        char player_name = word_vec[1].front();
        int n_map_id = atoi(word_vec[2].c_str());
        int rest_days = atoi(word_vec[3].c_str());
        auto player = get_player_by_uid(player_name);
        player->n_empty_rounds = rest_days;
        player->n_pos = n_map_id;
        p_map_t map = get_map();
        map->at(n_map_id).players.push_back(player);
    } else if (word_vec[0] == "nextuser") {
        char player_name = word_vec[1].front();
        next_player = get_player_by_uid(player_name);
    } else {
        auto map = get_map();
        if (word_vec[0] == "bomb") {
            int map_id = atoi(word_vec[1].c_str());
            map->at(map_id).item = BOMB;
        } else if (word_vec[0] == "barrier") {
            int map_id = atoi(word_vec[1].c_str());
            map->at(map_id).item = BLOCK;
        } else {
            std::cerr << "无效的命令" << std::endl;
        }
    }
    return 0;
}

