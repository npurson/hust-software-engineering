#include <cctype>
#include "cmdline.h"


extern p_player_t next_player;
int sleep_time = 0;
extern int init_money;

std::string get_cmd() {
    std::string command;
    if (std::getline(std::cin, command)) {
        // 去除命令前空格
        while (!command.empty() && command.front() == ' ') {
            command.erase(command.begin());
        }
        auto pos = command.find('#');
        // 去除注释
        if (pos != std::string::npos) {
            command.erase(pos, command.size() - pos);
        }
        // 去除命令后空格
        while (!command.empty() && command.back() == ' ') {
            command.pop_back();
        }
        tolower(command);
    } else {
        command = ""; // 确保出现错误的时候command总是为空
    }
    return command;
}

bool check_num(std::string num_str) {
    if (num_str.empty()) {
        return false;
    } else {
        if (num_str.front() == '-') {
            num_str.erase(num_str.begin());
        }
        return std::all_of(num_str.begin(), num_str.end(), isdigit);
    }
}


void start_game() {
    string inputs;
    int num_players;
    sleep_time = SLEEP_TIME;
    while (true) {
        cout << "请选择初始金钱数量(1000-50000): ";
        getline(cin, inputs);
        if (check_num(inputs)) {
            int money = std::stoi(inputs);
            if (money >= 1000 && money <= 50000) {
                init_money = money;
                break;
            }
        }
        cout << "输入范围有误" << endl;
    }

    while (true) {
        cout << "请选择参与的玩家数量(2-4人): ";
        getline(cin, inputs);
        if (check_num(inputs)) {
            num_players = std::stoi(inputs);
            if (num_players >= 2 && num_players <= 4) break;
        }
        cout << "输入范围有误" << endl;
    }

    int reset_flag;
    while (true) {
        reset_flag = 0;
        cout << "请按顺序输入" ;
        printf("%d", num_players);
        cout<< "位角色: 1-钱夫人 2-阿土伯 3-孙小美 4-金贝贝" << endl;
        getline(cin, inputs);
        if (inputs.length() != num_players) {
            cout << "输入角色个数有误"<< endl;
            continue;
        }
        for (int i = 0; i < inputs.length(); i++) {
            if (!(inputs[i] == 'Q' || inputs[i] == 'A' || inputs[i] == 'S' || inputs[i] == 'J' ||
                  inputs[i] == '1' || inputs[i] == '2' || inputs[i] == '3' || inputs[i] == '4')) {
                reset_flag = 1;
                break;
            }
            for (int j = i + 1; j < inputs.length(); j++) {
                if (inputs[i] == inputs[j]) {
                    reset_flag = 1;
                    break;
                }
            }
        }
        if (reset_flag) {
            cout << "输入角色名有误" << endl;
            continue;
        }

        auto tmp = get_player_vec();
        tmp->clear();

        for (char input : inputs) {
            if (input == '1')   input = 'Q';
            if (input == '2')   input = 'A';
            if (input == '3')   input = 'S';
            if (input == '4')   input = 'J';
            add_player(input);
        }
        next_player = &((*tmp)[0]);
        break;
    }
}


vector<string> split_cmd(string cmd) {
    vector<string> word_vec;
    auto comment_pos = cmd.find('#');
    if (comment_pos != string::npos) {
        cmd = cmd.substr(0, comment_pos); // 去除注释
    }
    while (!cmd.empty() && std::isspace(cmd.front())) {
        cmd.erase(cmd.begin());
    }
    while (!cmd.empty()) {
        auto space_pos = cmd.find(' ');
        string word;
        if (space_pos == string::npos) {
            word = cmd;
            cmd = "";
        } else {
            word = cmd.substr(0, space_pos);
            while (isspace(cmd[space_pos])) {
                ++space_pos;
            }
            if (space_pos == cmd.size()) {
                cmd = "";
            } else {
                cmd = cmd.substr(space_pos);
            }
        }
        word_vec.push_back(word);
    }
    return word_vec;
}


int parse_cmd(const string& cmd) {
    static bool start = false;
    vector<string> word_vec = split_cmd(cmd);
    if (word_vec.empty()) {
        return -1;
    }
    tolower(word_vec[0]);
    if (!start) {
        if (word_vec[0] == "preset") {
            word_vec.erase(word_vec.begin());
            auto result = do_preset(word_vec);
            start = true;
            return result;
        } else if (word_vec[0] == "start") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，start命令格式为：start" << endl;
                Sleep(sleep_time);
                return -1;
            }
            start_game();
            start = true;
        } else {
            std::cerr << "请输入start开始游戏" << endl;
            Sleep(sleep_time);
        }
    } else {
        if (word_vec[0] == "preset") {
            word_vec.erase(word_vec.begin());
            auto result = do_preset(word_vec);
            return result;
        } else if (word_vec[0] == "roll") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，roll命令格式为：roll" << endl;
                Sleep(sleep_time);
                return -1;
            }
            do_roll();
        } else if (word_vec[0] == "dump") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，dump命令格式为：dump" << endl;
                Sleep(sleep_time);
                return -1;
            }
            do_dump();
        } else if (word_vec[0] == "sell") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，sell命令格式为：sell n，n指定玩家房产的地块编号" << endl;
                Sleep(sleep_time);
                return -1;
            }
            if (!check_num(word_vec[1])) return -1;
            auto map_id = std::stoi(word_vec[1]);
            if (map_id < 0 || map_id >= MAP_SIZE) {
                std::cerr << "地块编号错误，请输入0-69的地块编号" << std::endl;
                Sleep(sleep_time);
                return -1;
            }
            do_sell(*get_map(), *next_player, map_id);
        } else if (word_vec[0] == "query") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，query命令格式为：query" << endl;
                Sleep(sleep_time);
                return -1;
            }
            do_query(*next_player);
        } else if (word_vec[0] == "block") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，block命令格式为：block n，n指定与当前位置的相对距离，范围为[-10,10]" << endl;
                Sleep(sleep_time);
                return -1;
            }
            if (!check_num(word_vec[1])) return -1;
            auto block_step = std::stoi(word_vec[1]);
            do_block(block_step, next_player);
        } else if (word_vec[0] == "robot") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，robot命令格式为：robot" << endl;
                Sleep(sleep_time);
                return -1;
            }
            do_robot(next_player);
        } else if (word_vec[0] == "step") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，step命令格式为：step n，n为指定的步数" << endl;
                Sleep(sleep_time);
                return -1;
            }
            if (!check_num(word_vec[1])) return -1;
            auto step = std::stoi(word_vec[1]);
            if (step < 0) {
                return -1;
            }
            do_step(step);
        } else if (word_vec[0] == "help") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，help命令格式为：help" << std::endl;
                Sleep(sleep_time);
                return -1;
            }
            do_help();
        }
    }
    return -1;
}


void do_sell(map_t& map, player_t& player, int map_node_idx)
{
    // basic rules
    if (player.b_sell_estate == 1 ||
        map[map_node_idx].type != VACANCY ||
        !(map[map_node_idx].owner) ||
        map[map_node_idx].owner->uid != player.uid) {
        cout << "[卖房] 卖出房产失败" << endl;
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


void do_robot(p_player_t player) {
    apply_item(*get_map(), *player, ROBOT);
}


void do_block(int step, p_player_t player) {
    apply_item(*get_map(), *player, BLOCK, step);
}

void erase_player_from_curr_pos(p_player_t p_player) {
    p_map_t p_map = get_map();
    auto &node = p_map->at(next_player->n_pos);
    for(auto it=node.players.begin(); it!=node.players.end();++it){
        if(*it == next_player){
            node.players.erase(it);
            break;
        }
    }
}

int do_roll() {
    if (roll_dice(*get_map(), *next_player)) {
        erase_player_from_curr_pos(next_player);
        for (auto &it : next_player->estate) {
            it->estate_lvl = 0;
            it->owner = nullptr;
        }

        // empty broken player info
        next_player->n_money = -1;
        next_player->estate.clear();

        // check winner
        auto players = get_player_vec();
        auto winner = (*get_player_vec())[0];
        int count = 0;
        for (auto &it : *players) {
            if (it.n_money < 0) count += 1;
            winner = it;
        }
        if (count == (players->size() - 1)) {
            std::cout << "游戏结束，获胜的玩家是:" << winner.name << std::endl;
            switch_player(&next_player);
            do_dump();
        }
    }
    next_player->b_god_buff = 0;
    // switch to next player
    switch_player(&next_player);
//    auto players = get_player_vec();
//    int c = 0;
//    for (auto & it : *players) {
//        if (it.uid == next_player->uid) {
//            if (c + 1 > players->size() - 1) next_player = &(*(get_player_vec()))[0];
//            else next_player = &(*(get_player_vec()))[c + 1];
//            break;
//        }
//        c += 1;
//    }
//    skip_player(next_player);
    return 0;
}

void do_skip(p_player_t player) {
    // 处理在原地交租的情形
    auto curr_map = *get_map();
    if (curr_map[player->n_pos].owner && curr_map[player->n_pos].owner != player) {
        if (pay_rent(player)) {
            erase_player_from_curr_pos(player);
            for (auto &it : player->estate) {
                it->estate_lvl = 0;
                it->owner = nullptr;
            }

            // empty broken player info
            player->n_money = -1;
            player->estate.clear();

            // check winner
            auto players = get_player_vec();
            auto winner = &players->front();
            int count = 0;
            for (auto &it : *players) {
                if (it.n_money < 0) {
                    count += 1;
                }
                else {
                    winner = &it;
                }
            }
            if (count == (players->size() - 1)) {
                std::cout << "游戏结束，获胜的玩家是:" << winner->name << std::endl;
                next_player = winner;
                do_dump();
            }
        }
    }
}

void switch_player(p_player_t *p_next_player) {
    auto players = get_player_vec();
    std::vector<player_t>::size_type c = 0;
    for (auto & it : *players) {
        if (it.uid == (*p_next_player)->uid) {
            break;
        }
        ++c;
    }
    while (true) {
        c = (c + 1) % players->size();
        if (players->at(c).n_money < 0) {
            continue;
        } else {
            if (players->at(c).n_god_buff > 0) {
                players->at(c).b_god_buff = 1;
                --(players->at(c).n_god_buff);
            } else {
                players->at(c).b_god_buff = 0;
            }
            if (players->at(c).n_empty_rounds  > 0) {
                --(players->at(c).n_empty_rounds);
                std::cout << "玩家" << players->at(c).name << "轮空" << std::endl;
                do_skip(&players->at(c));
                Sleep(sleep_time);
                continue;
            } else {
                break;
            }
        }
    }
    *p_next_player = &players->at(c);
    (*p_next_player)->b_sell_estate = 0;
}

void do_dump() {
    string dump_text = "user ";
    auto player_vec = get_player_vec();

    for (const auto& player : *player_vec) {
        dump_text += player.uid;
    }
    std::cerr << dump_text << endl;
    for (const auto& player : *player_vec) {
        for (const auto& p_estate : player.estate) {
            std::cerr << "map " << p_estate->id << " " << player.uid << " " << p_estate->estate_lvl << endl;
        }
        std::cerr << "fund " << player.uid << " " << player.n_money << endl;
        std::cerr << "credit " << player.uid << " " << player.n_points << endl;
        std::cerr << "userloc " << player.uid << " " << player.n_pos << " "  << player.n_empty_rounds << endl;
        if (player.n_bomb != 0) {
            std::cerr << "gift " << player.uid << " bomb " << player.n_bomb << endl;
        }
        if (player.n_block != 0) {
            std::cerr << "gift " << player.uid << " barrier " << player.n_block << endl;
        }
        if (player.n_robot != 0) {
            std::cerr << "gift " << player.uid << " robot " << player.n_robot << endl;
        }
        if (player.n_god_buff != 0) {
            std::cerr << "gift " << player.uid << " god " << player.n_god_buff + player.b_god_buff << endl;
        }
    }

    auto map = get_map();
    for (const auto& map_node : *map) {
        switch(map_node.item) {
            case BLOCK:
                std::cerr << "barrier " << map_node.id << endl;
                break;
            case NONE:
            default: break;
        }
    }
    std::cerr << "nextuser " << next_player->uid << endl;
    exit(EXIT_SUCCESS);
}


void show_cmd() {
    HANDLE h_out=GetStdHandle(STD_OUTPUT_HANDLE);
    static const unsigned short color_table[4] = { FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE,
                                                   FOREGROUND_RED | FOREGROUND_GREEN };

    if (next_player != nullptr) {
        SetConsoleTextAttribute(h_out, color_table[ next_player->e_color ]);
        cout << next_player->name;
        SetConsoleTextAttribute(h_out,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    } else {
        cout << "请输入start开始游戏";
    }
    cout << ">";
}


int do_step(int step) {
    if (step_forward(*get_map(), *next_player, step)) {
        erase_player_from_curr_pos(next_player);
        for (auto & it : next_player->estate){
            it->estate_lvl = 0;
            it->owner = nullptr;
        }
        // empty broken player info
        next_player->n_money = -1;
        next_player->estate.clear();

        // check winner
        auto players = get_player_vec();
        auto winner = (*get_player_vec())[0];
        int count = 0;
        for (auto &it : *players) {
            if (it.n_money < 0) count += 1;
            else    winner = it;
        }
        if (count == (players->size() - 1)) {
            std::cout << "游戏结束，获胜的玩家是:" << winner.name << std::endl;
            switch_player(&next_player);
            do_dump();
        }
    }

    next_player->b_god_buff = 0;
    // switch to next player
    switch_player(&next_player);
//    auto players = get_player_vec();
//    int c = 0;
//    for (auto & it : *players) {
//        if (it.uid == next_player->uid){
//            if (c + 1 > players->size() - 1) next_player = &(*(get_player_vec()))[0];
//            else next_player = &(*(get_player_vec()))[c + 1];
//            break;
//        }
//        c += 1;
//    }
//    next_player = skip_player(next_player);
    return 0;
}


int do_preset(const std::vector<std::string>& word_vec) {
    if (word_vec.empty()) {
        return -1;
    }
    if (word_vec[0] == "user") {
        if (word_vec.size() != 2) {
            return -1;
        }
        auto player_vec = get_player_vec();
        init_map();
        init_money = DEFAULT_MONEY;
        sleep_time = 0;
        player_vec->clear();
        next_player = nullptr;
        for (char uid : word_vec[1]) {
            add_player(uid);
        }
        if (player_vec->size() < 2 && player_vec->size() > 4) {
            player_vec->clear();
            return -1;
        }
        next_player = &player_vec->front();
    } else if (word_vec[0] == "map") {
        if (word_vec.size() != 4) return -1;
        if (!check_num(word_vec[1]) || !check_num(word_vec[3])) return -1;
        int n_map = std::stoi(word_vec[1]);
        p_map_t p_map;
        if (n_map < 0 || n_map >= MAP_SIZE || n_map == START_POS || n_map == HOSPITAL_POS || n_map == ITEM_HOUSE_POS || n_map == GIFT_HOUSE_POS || n_map == PRISON_POS || n_map == MAGIC_HOUSE_POS || n_map >= 63 || n_map <= 69) {
            return -1;
        }
        p_map = get_map();
        char player_name = word_vec[2].front();
        auto player = get_player_by_uid(player_name);
        if (player == nullptr) {
            return -1;
        }
        if (p_map->at(n_map).owner == player) {
            return -1;
        }
        int level = std::stoi(word_vec[3]);
        if (level < 0 || level > 3) {
            return -1;
        }
        p_map->at(n_map).owner = player;
        p_map->at(n_map).estate_lvl = level;
        p_map->at(n_map).owner->estate.push_back(&p_map->at(n_map));
    } else if (word_vec[0] == "fund") {
        if (word_vec.size() != 3) return -1;
        if (!check_num(word_vec[2])) return -1;
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        int money = std::stoi(word_vec[2]);
        if (money < 0) {
            return -1;
        }
        player->n_money = money;
    } else if (word_vec[0] == "credit") {
        if (word_vec.size() != 3) return -1;
        if (!check_num(word_vec[2])) return -1;
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        if (player == nullptr) {
            return -1;
        }
        int points = std::stoi(word_vec[2]);
        if (points < 0) {
            return -1;
        }
        player->n_points = points;
    } else if (word_vec[0] == "gift") {
        if (word_vec.size() != 4) return -1;
        if (!check_num(word_vec[3])) return -1;
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        if (player == nullptr) {
            return -1;
        }
        auto prop_name = word_vec[2];
        int number = std::stoi(word_vec[3]);
        if (number >= 0) {
            if (prop_name == "barrier") {
                if ((player->n_robot + number) <= 10) {
                    player->n_block = number;
                } else {
                    return -1;
                }
            } else if (prop_name == "robot") {
                if ((player->n_block + number) <= 10) {
                    player->n_robot = number;
                } else {
                    return -1;
                }
            } else if (prop_name == "god") {
                player->n_god_buff = number;
                player->b_god_buff = 0;
            } else {
                return -1;
            }
        }
    } else if (word_vec[0] == "userloc") {
        if (word_vec.size() != 4) return -1;
        if (!check_num(word_vec[2]) || !check_num(word_vec[3])) return -1;
        char player_name = word_vec[1].front();

        int n_map_id = std::stoi(word_vec[2]);
        int rest_days = std::stoi(word_vec[3]);
        if (n_map_id < 0 || n_map_id >= MAP_SIZE || rest_days < 0) {
            return -1;
        }
        auto player = get_player_by_uid(player_name);
        if (player == nullptr) {
            return -1;
        }
        p_map_t map = get_map();
        erase_player_from_curr_pos(player);
        player->n_empty_rounds = rest_days;
        player->n_pos = n_map_id;
        map->at(n_map_id).players.push_back(player);
    } else if (word_vec[0] == "nextuser") {
        if (word_vec.size() != 2) return -1;
        char player_name = word_vec[1].front();
        auto p = get_player_by_uid(player_name);
        if (p != nullptr) {
            next_player = p;
        }
    } else {
        auto p_map = get_map();
        auto players = get_player_vec();
        if (word_vec[0] == "barrier") {
            if (!check_num(word_vec[1])) {
                return -1;
            }
            int map_id = std::stoi(word_vec[1]);
            if (map_id < 0 || map_id >= 70) {
                return -1;
            }
            for (const auto& p : *players) {
                if (p.n_pos == map_id) {
                    return -1;
                }
            }
            if (p_map->at(map_id).item != NONE) {
                return -1;
            }
            p_map->at(map_id).item = BLOCK;
        } else {
            return -1;
        }
    }
    return 0;
}


int do_query(player_t& player)
{
    std::cout << "资金: " << player.n_money << std::endl;
    std::cout << "点数: " << player.n_points << std::endl;
    std::cout << "财神buff轮数: " << player.n_god_buff + player.b_god_buff << std::endl;
    std::cout << std::endl;
    std::cout << "固定资产: " << std::endl;
    for (auto & it : player.estate) {
        int area;
        switch(it->value) {
            case AREA_1_PRICE:
                area = 1;
                break;
            case AREA_2_PRICE:
                area = 2;
                break;
            case AREA_3_PRICE:
                area = 3;
                break;
            default:
                return -1;
        }
        std::cout << "地段" << area << ", ";
        std::cout << it->id << "号房屋" << ", ";
        std::cout << "等级" << it->estate_lvl << ", ";
        std::cout << "价值" << get_estate_price(*it) << "元" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "道具:" << " 路障*" << player.n_block << " 机器娃娃*" << player.n_robot << std::endl;
    system("pause");
    return 0;
}

int do_help() {
    string help_str = "帮助信息:\n";
    help_str.append("start    —— 开始游戏\n");
    help_str.append("roll     —— 掷随机骰子\n");
    help_str.append("sell n   —— 卖房子，n指示要卖的房子的地块索引\n");
    help_str.append("block n  —— 放置路障，n指示路障与使用玩家当前位置的相对距离\n");
    help_str.append("bomb n   —— 放置炸弹，n表示炸弹与使用玩家当前位置的相对距离\n");
    help_str.append("robot    —— 使用机器娃娃道具\n");
    help_str.append("query    —— 查询当前玩家所有资产信息\n");
    help_str.append("quit     —— 退出游戏\n");
    help_str.append("help     —— 显示此帮助");
    cout << help_str << endl;
    system("pause");
    return 0;
}


void tolower(string &str) {
    for (auto& c : str) {
        if (std::isalpha(c)) {
            c = tolower(c);
        }
    }
}
