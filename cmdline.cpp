#include <cctype>
#include "cmdline.h"


extern p_player_t next_player;
extern int init_money;


bool check_num(const std::string& num_str) {
    if (num_str.empty()) {
        return false;
    }
    if (num_str[0] == 0) {
        return false;
    } else {
        for (const auto& c : num_str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }
}


void start_game() {
    string inputs;
    int num_players;

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
        std::cout << "输入范围有误" << std::endl;
    }

    while (true) {
        cout << "请选择参与的玩家数量(2-4人): ";
        getline(cin, inputs);
        if (check_num(inputs)) {
            num_players = std::stoi(inputs);
            if (num_players >= 2 && num_players <= 4) break;
        }
        std::cout << "输入范围有误" << std::endl;
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
    if (comment_pos != std::string::npos) {
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
    if (cmd.empty()) {
        return -1;
    }
    tolower(word_vec[0]);
    if (!start) {
        if (word_vec[0] == "preset") {
            string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            start = true;
            return 0;
        } else if (word_vec[0] == "start") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，start命令格式为：start" << endl;
                return -1;
            }
            start_game();
            start = true;
        } else {
            std::cerr << "请输入start开始游戏" << endl;
        }
    } else {
        if (word_vec[0] == "preset") {
            string::size_type space_pos = cmd.find(' ');
            do_preset(cmd.substr(space_pos + 1));
            return 0;
        } else if (word_vec[0] == "roll") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，roll命令格式为：roll" << endl;
                return -1;
            }
            do_roll();
        } else if (word_vec[0] == "dump") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，dump命令格式为：dump" << endl;
                return -1;
            }
            do_dump();
        } else if (word_vec[0] == "sell") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，sell命令格式为：sell n，n指定玩家房产的地块编号" << endl;
                return -1;
            }
            auto map_id = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_sell(*get_map(), *next_player, static_cast<int>(map_id));
        } else if (word_vec[0] == "query") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，query命令格式为：query" << endl;
                return -1;
            }
            do_query(*next_player);
        } else if (word_vec[0] == "block" || word_vec[0] == "barrier") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，block命令格式为：block n，n指定与当前位置的相对距离，范围为[-10,10]" << endl;
                return -1;
            }
            auto block_step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_block(static_cast<int>(block_step), next_player);
        // } else if (word_vec[0] == "bomb") {
        //     if (word_vec.size() != 2) {
        //         std::cerr << "命令格式错误，bomb命令格式为：bomb n，n指定与当前位置的相对距离，范围为[-10,10]" << endl;
        //         return -1;
        //     }
        //     auto bomb_step = std::strtol(word_vec[1].c_str(), nullptr, 10);
        //     do_bomb(static_cast<int>(bomb_step), next_player);
        } else if (word_vec[0] == "robot") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，robot命令格式为：robot" << endl;
                return -1;
            }
            do_robot(next_player);
        } else if (word_vec[0] == "step") {
            if (word_vec.size() != 2) {
                std::cerr << "命令格式错误，step命令格式为：step n，n为指定的步数" << endl;
                return -1;
            }
            auto step = std::strtol(word_vec[1].c_str(), nullptr, 10);
            do_step(static_cast<int>(step));
        } else if (word_vec[0] == "help") {
            if (word_vec.size() != 1) {
                std::cerr << "命令格式错误，help命令格式为：help" << std::endl;
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


void do_bomb(int step, p_player_t player) {
   apply_item(*get_map(), *player, BOMB, static_cast<int>(step));
}


void do_robot(p_player_t player) {
    apply_item(*get_map(), *player, ROBOT);
}


void do_block(int step, p_player_t player) {
    apply_item(*get_map(), *player, BLOCK, static_cast<int>(step));
}


int do_roll() {
    if (roll_dice(*get_map(), *next_player)) {
        for (auto &it : next_player->estate) {
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

        // check winner
        auto players = get_player_vec();
        auto winner = (*get_player_vec())[0];
        int count = 0;
        for (auto &it : *players) {
            if (it.n_money < 0) count += 1;
            winner = it;
        }
        if (count == (players->size() - 1)) {
            std::cout << "游戏结束，获胜的玩家是:";
            if (winner.uid == 'Q') std::cout << "钱夫人";
            if (winner.uid == 'A') std::cout << "阿土伯";
            if (winner.uid == 'S') std::cout << "孙小美";
            if (winner.uid == 'J') std::cout << "金贝贝";
            exit(EXIT_SUCCESS);
        }
    }
    // switch to next player
    auto players = get_player_vec();
    int c = 0;
    for (auto & it : *players) {
        if (it.uid == next_player->uid) {
            if (c + 1 > players->size() - 1) next_player = &(*(get_player_vec()))[0];
            else next_player = &(*(get_player_vec()))[c + 1];
            break;
        }
        c += 1;
    }
    return 0;
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
            std::cerr << "map " << static_cast<int>(p_estate->id) << " " << player.uid << " " << static_cast<int>(p_estate->estate_lvl) << endl;
        }
        std::cerr << "fund " << player.uid << " " << player.n_money << endl;
        std::cerr << "credit " << player.uid << " " << player.n_points << endl;
        std::cerr << "userloc " << player.uid << " " << static_cast<int>(player.n_pos) << " "  << static_cast<int>(player.n_empty_rounds) << endl;
        if (player.n_bomb != 0) {
            std::cerr << "gift " << player.uid << " bomb " << static_cast<int>(player.n_bomb) << endl;
        }
        if (player.n_block != 0) {
            std::cerr << "gift " << player.uid << " barrier " << static_cast<int>(player.n_block) << endl;
        }
        if (player.n_robot != 0) {
            std::cerr << "gift " << player.uid << " robot " << static_cast<int>(player.n_robot) << endl;
        }
        if (player.n_god_buff != 0) {
            std::cerr << "gift " << player.uid << " god " << static_cast<int>(player.n_god_buff) << endl;
        }
    }

    auto map = get_map();
    for (const auto& map_node : *map) {
        switch(map_node.item) {
            case BOMB:
                std::cerr << "bomb " << static_cast<int>(map_node.id) << endl;
                break;
            case BLOCK:
                std::cerr << "barrier " << static_cast<int>(map_node.id) << endl;
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
        std::cout << next_player->name;
        SetConsoleTextAttribute(h_out,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    } else {
        std::cout << "请输入start开始游戏";
    }
    cout << ">";
}


int do_step(int step) {
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

    // switch to next player
    auto players = get_player_vec();
    int c = 0;
    for (auto & it : *players) {
        if (it.uid == next_player->uid){
            if (c + 1 > players->size() - 1) next_player = &(*(get_player_vec()))[0];
            else next_player = &(*(get_player_vec()))[c + 1];
            break;
        }
        c += 1;
    }
    return 0;
}


int do_preset(string cmd) {
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
    } else if (word_vec[0] == "map") {
        int n_map = std::stoi(word_vec[1]);
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
        int level = std::stoi(word_vec[3]);
        map->at(n_map).estate_lvl = level;
        map->at(n_map).owner->estate.push_back(&map->at(n_map));
    } else if (word_vec[0] == "fund") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        int money = std::stoi(word_vec[2]);
        player->n_money = money;
    } else if (word_vec[0] == "credit") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        int points = std::stoi(word_vec[2]);
        player->n_points = points;
    } else if (word_vec[0] == "gift") {
        char player_name = word_vec[1].front();
        auto player = get_player_by_uid(player_name);
        auto prop_name = word_vec[2];
        int number = std::stoi(word_vec[3]);
        if (prop_name == "barrier") {
            player->n_block = number;
        } else if (prop_name == "robot") {
            player->n_robot = number;
        } else if (prop_name == "god") {
            player->n_god_buff = number;
        } else {
            std::cerr << "请选择有效的道具种类" << std::endl;
            return -1;
        }
    } else if (word_vec[0] == "userloc") {
        char player_name = word_vec[1].front();
        int n_map_id = std::stoi(word_vec[2]);
        int rest_days = std::stoi(word_vec[3]);
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
            int map_id = std::stoi(word_vec[1]);
            map->at(map_id).item = BOMB;
        } else if (word_vec[0] == "barrier") {
            int map_id = std::stoi(word_vec[1]);
            map->at(map_id).item = BLOCK;
        } else {
            std::cerr << "无效的命令" << endl;
        }
    }
    return 0;
}


int do_query(player_t& player)
{
    cout << "资金: " << player.n_money << endl;
    cout << "点数: " << player.n_points << endl;
    cout << "固定资产: ";
    for (auto & it : player.estate) {
        printf("%d号房屋 ", it->id);
    }
    cout << endl;
    cout << "道具:" << " 路障*";
    printf("%d", player.n_block);
    cout <<" 机器娃娃*";
    printf("%d\n", player.n_robot);
    Sleep(1000);
    return 0;
}


int do_help() {
    string help_str = "帮助信息\n";
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