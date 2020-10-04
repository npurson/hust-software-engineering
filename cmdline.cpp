#include <iostream>
#include <cstdlib>
#include <cctype>
#include "cmdline.h"

extern p_player_t next_player;


void start_game() {
    std::string inputs;
    std::uint8_t num_players;

    while (true){
        std::cout << "请选择参与的玩家数量(2-4人): ";
        std::cin >> inputs;
        std::cin.clear();
        std::cin.sync();
        num_players = std::stoi(inputs);
        if (num_players >= 2 and num_players <= 4)   break;
        std::cout << "输入范围有误";
    }

    std::uint8_t reset_flag = 0;
    while (true){
        reset_flag = 0;
        std::cout << "请按顺序输入" ;
        printf("%d", num_players);
        std::cout<< "位角色: Q-钱夫人 A-阿土伯 S-孙小美 J-金贝贝" << std::endl;
        std::cin >> inputs;
        std::cin.clear();
        std::cin.sync();
        if (inputs.length() != num_players){
            std::cout << "输入角色个数有误"<< std::endl;
            continue;
        }
        for (std::uint64_t i = 0; i < inputs.length(); i++) {
            if (inputs[i] != 'Q' && inputs[i] != 'A' && inputs[i] != 'S' && inputs[i] != 'J') {
                reset_flag = 1;
                break;
            }
            for (std::uint64_t j = i + 1; j < inputs.length(); j++) {
                if (inputs[i] == inputs[j]) {
                    reset_flag = 1;
                    break;
                }
            }
        }
        if (reset_flag){
            std::cout << "输入角色名有误" << std::endl;
            continue;
        }

        auto tmp = get_player_vec();
        tmp->clear();
        
        for (char input : inputs) {
            add_player(input);
        }
        next_player = &((*tmp)[0]);
        break;
    }
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
    if (cmd.empty()) {
        return -1;
    }
    tolower(word_vec[0]);
    if (!start) {
        if (word_vec[0] == "preset") {
            std::string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            start = true;
            return 0;
        } else if (word_vec[0] == "start") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，start命令格式为：start" << std::endl;
                return -1;
            }
            start_game();
            start = true;
        } else {
            std::cerr << "请输入start开始游戏" << std::endl;
        }
    } else {
        if (word_vec[0] == "preset") {
            std::string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            return 0;
        } else if (word_vec[0] == "roll") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，roll命令格式为：roll" << std::endl;
                return -1;
            }
            do_roll();
        } else if (word_vec[0] == "dump") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，dump命令格式为：dump" << std::endl;
                return -1;
            }
            do_dump();
        } else if (word_vec[0] == "sell") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，sell命令格式为：sell n，n指定玩家房产的地块编号" << std::endl;
                return -1;
            }
            auto map_id = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_sell(*get_map(), *next_player, static_cast<int>(map_id));
        } else if (word_vec[0] == "query") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，query命令格式为：query" << std::endl;
                return -1;
            }
            do_query(*next_player);
        } else if (word_vec[0] == "block") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，block命令格式为：block n，n指定与当前位置的相对距离，范围为[-10,10]" << std::endl;
                return -1;
            }
            auto block_step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_block(static_cast<uint8_t>(block_step), next_player);
        } else if (word_vec[0] == "bomb") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，bomb命令格式为：bomb n，n指定与当前位置的相对距离，范围为[-10,10]" << std::endl;
                return -1;
            }
            auto bomb_step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_bomb(static_cast<uint8_t>(bomb_step), next_player);
        } else if (word_vec[0] == "robot") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，robot命令格式为：robot" << std::endl;
                return -1;
            }
            auto robot_step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_robot(static_cast<uint8_t>(robot_step), next_player);
        } else if (word_vec[0] == "step") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，step命令格式为：step n，n为指定的步数" << std::endl;
                return -1;
            }
            auto step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_step(static_cast<uint8_t>(step));
        } else if (word_vec[0] == "query") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，query命令格式为：query" << std::endl;
                return -1;
            }
            do_query(*next_player);
        } else if (word_vec[0] == "help") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，query命令格式为：query" << std::endl;
                return -1;
            }
            do_help();
        }
    }
    return -1;
}

void do_robot(std::uint8_t step, p_player_t player) {

}
void do_sell(map_t& map, player_t& player, uint8_t map_node_idx)
{
    // basic rules
    if (player.b_sell_estate == 1 ||
        map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner == nullptr ||
        map[map_node_idx].owner->uid != player.uid) {
//        cout << "[卖房] 卖出房产失败" << endl;
        return;
    }

    // update player info
    player.b_sell_estate = 1;
    player.n_money += 2 * get_estate_price(map[map_node_idx]);
    for (auto it = player.estate.begin(); it != player.estate.end(); ++it) {
        if (*it == &map[map_node_idx]) {
            player.estate.erase(it);
            break;
        }
    }

    // update estate info
    map[map_node_idx].owner = nullptr;
    map[map_node_idx].estate_lvl = WASTELAND;
    cout << "[卖房] 卖出房产成功" << endl;
}


void do_bomb(std::uint8_t step, p_player_t player) {

}

void do_block(std::uint8_t step, p_player_t player) {

}


int do_roll() {
    if (roll_dice(*get_map(), *next_player)){
        for (auto & it : next_player->estate){
            it->estate_lvl = 0;
            it->owner = nullptr;
        }
        // empty broken player info
        next_player->n_money = -1;
        next_player->n_points = 0;
        next_player->n_pos = 0;
        next_player->n_empty_rounds = 0;
        next_player->n_god_buff = 0;
        next_player->estate.clear();
        next_player->n_block = 0;
        next_player->n_bomb = 0;
        next_player->n_robot = 0;
        next_player->b_sell_estate = 0;
    }

    // do count
    next_player->b_sell_estate = 0;
    if (next_player->n_god_buff > 0)    next_player->n_god_buff -= 1;
    if (next_player->n_empty_rounds > 0)    next_player->n_empty_rounds -= 1;

    // switch to next player
    auto players = get_player_vec();
    std::uint8_t c = 0;
    for (auto & it : *players) {
        if (it.uid == next_player->uid){
            if (c + 1 > players->size() - 1)    next_player = &(*(get_player_vec()))[0];
            else    next_player = &(*(get_player_vec()))[c + 1];
            break;
        }
        c += 1;
    }
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
        if (player.n_bomb != 0) {
            std::cerr << "gift " << player.uid << " bomb " << static_cast<int>(player.n_bomb) << std::endl;
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
    if (step_forward(*get_map(), *next_player, step)){
        for (auto & it : next_player->estate){
            it->estate_lvl = 0;
            it->owner = nullptr;
        }
        // empty broken player info
        next_player->n_money = -1;
        next_player->n_points = 0;
        next_player->n_pos = 0;
        next_player->n_empty_rounds = 0;
        next_player->n_god_buff = 0;
        next_player->estate.clear();
        next_player->n_block = 0;
        next_player->n_bomb = 0;
        next_player->n_robot = 0;
        next_player->b_sell_estate = 0;
    }

    // do count
    next_player->b_sell_estate = 0;
    if (next_player->n_god_buff > 0)    next_player->n_god_buff -= 1;
    if (next_player->n_empty_rounds > 0)    next_player->n_empty_rounds -= 1;

    // switch to next player
    auto players = get_player_vec();
    std::uint8_t c = 0;
    for (auto & it : *players) {
        if (it.uid == next_player->uid){
            if (c + 1 > players->size() - 1)    next_player = &(*(get_player_vec()))[0];
            else    next_player = &(*(get_player_vec()))[c + 1];
            break;
        }
        c += 1;
    }
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
            player->n_bomb = number;
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


int do_query(player_t& player)
{
    std::cout << "资金: " << player.n_money << std::endl;
    std::cout << "点数: " << player.n_points << std::endl;
    std::cout << "固定资产: ";
    for (auto & it : player.estate) {
        printf("%d号房屋 ", it->id);
    }
    std::cout << std::endl;
    std::cout << "道具: 炸弹*";
    printf("%d", player.n_bomb);
    std::cout << " 路障*";
    printf("%d", player.n_block);
    std::cout <<" 机器娃娃*";
    printf("%d\n", player.n_robot);
    return 0;
}

int do_help() {
    std::string help_str = "帮助信息\n";
    help_str.append("start    —— 开始游戏\n");
    help_str.append("roll     —— 掷随机骰子\n");
    help_str.append("sell n   —— 卖房子，n指示要卖的房子的地块索引\n");
    help_str.append("block n  —— 放置路障，n指示路障与使用玩家当前位置的相对距离\n");
    help_str.append("bomb n   —— 放置炸弹，n表示炸弹与使用玩家当前位置的相对距离\n");
    help_str.append("robot    —— 使用机器娃娃道具\n");
    help_str.append("query    —— 查询当前玩家所有资产信息\n");
    help_str.append("quit     —— 退出游戏\n");
    help_str.append("help     —— 显示此帮助");
    std::cout << help_str << std::endl;
    return 0;
}