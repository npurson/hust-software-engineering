#include "cmdline.h"

static map_t map;
extern int sleep_time;


p_map_t init_map()
{
    map.clear();
    for (int i = 0; i < MAP_SIZE; ++i) {
        switch (i) {
            case START_POS: map.emplace_back(i, START); break;
            case HOSPITAL_POS: map.emplace_back(i, PARK); break;                // requirements changed
            case ITEM_HOUSE_POS: map.emplace_back(i, ITEM_HOUSE); break;
            case GIFT_HOUSE_POS: map.emplace_back(i, GIFT_HOUSE); break;
            case PRISON_POS: map.emplace_back(i, PARK); break;                  // requirements changed
            case MAGIC_HOUSE_POS: map.emplace_back(i, MAGIC_HOUSE); break;

            case 64: map.emplace_back(i, MINE, 60); break;
            case 65: case 68: map.emplace_back(i, MINE, 80); break;
            case 66: map.emplace_back(i, MINE, 40); break;
            case 67: map.emplace_back(i, MINE, 100); break;
            case 69: map.emplace_back(i, MINE, 20); break;
            default:
                if (i > START_POS && i < ITEM_HOUSE_POS && i != HOSPITAL_POS)
                    map.emplace_back(i, VACANCY, 200);
                else if (i > ITEM_HOUSE_POS && i < GIFT_HOUSE_POS)
                    map.emplace_back(i, VACANCY, 500);
                else if (i > GIFT_HOUSE_POS && i <= MAGIC_HOUSE_POS && i != PRISON_POS)
                    map.emplace_back(i, VACANCY, 300);
        }
    }
    return &map;
}


p_map_t get_map()
{
    return &map;
}


void plot_map()
{
    static const char hash_table[29 * 8] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
            69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29,
            68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30,
            67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31,
            66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
            65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33,
            64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34,
            63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35
    };
    static const char type_table[9] = { 'S', '0', 'T', 'G', 'M', 'H', 'P', '$', 'P' };
    static const unsigned short color_table[4] = { FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE,
                                                   FOREGROUND_RED | FOREGROUND_GREEN };
    static const char item_table[4] = { '\0', '#', '@', 'R' };

    HANDLE h_out;
    h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    SetConsoleCursorPosition(h_out, (COORD){ 0, 0 });

    for(int i = 0; i < 29 * 8; ++i) {
        SetConsoleTextAttribute(h_out, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        // empty space
        if (hash_table[i] == 0 && i != 0) { putchar(' '); continue; }

        // basic map
        char buf = type_table[map[hash_table[i]].type];
        if (buf == '0') {
            buf += map[hash_table[i]].estate_lvl;
            if (map[hash_table[i]].owner) {
                SetConsoleTextAttribute(h_out, color_table[map[hash_table[i]].owner->e_color]);
            }
        }

        // item
        if ((map[hash_table[i]].item == BLOCK) || (map[hash_table[i]].item == BOMB)) {
            buf = item_table[map[hash_table[i]].item];
        }

        // player
        if (!map[hash_table[i]].players.empty()) {
            buf = map[hash_table[i]].players.back()->uid;
            SetConsoleTextAttribute(h_out, color_table[map[hash_table[i]].players.back()->e_color]);
        }

        // render
        putchar(buf);
        if ((i % 29) == 28) printf("\n");
    }
    SetConsoleCursorPosition(h_out, (COORD){ 0, 10 });
}


void buy_estate(map_t& curr_map, player_t& player)
{
    int map_node_idx = player.n_pos;
    if (curr_map[map_node_idx].type != VACANCY ||
        curr_map[map_node_idx].owner ||
        player.n_money < curr_map[map_node_idx].value)
        return;

    string choice;
    cout << "[买房] 是否购买房产？(请输入y/n)" << endl;

    while (true) {
        show_cmd();
        choice = get_cmd();
        if (choice == "y") {
            player.n_money -= curr_map[map_node_idx].value;
            player.estate.push_back(&curr_map[map_node_idx]);
            curr_map[map_node_idx].owner = &player;
            break;
        }
        else if (choice == "n") break;
        else cout << "[买房] 选择无效，请重新输入" << endl;
    }
}


void update_estate(map_t& curr_map, player_t& player)
{
    int map_node_idx = player.n_pos;
    if (curr_map[map_node_idx].type != VACANCY ||
        !(curr_map[map_node_idx].owner) ||
        curr_map[map_node_idx].owner->uid != player.uid ||
        curr_map[map_node_idx].estate_lvl == SKYSCRAPER ||
        player.n_money < curr_map[map_node_idx].value)
        return;

    string choice;
    cout << "[升级] 升级需支付 " << curr_map[player.n_pos].value << " 元" << endl;
    cout << "      是否需要升级？(y/n)" << endl;

    while (true) {
        show_cmd();
        choice = get_cmd();
        if (choice == "y") {
            player.n_money -= curr_map[map_node_idx].value;
            curr_map[map_node_idx].estate_lvl += 1;
            cout << "[升级] 建筑升级成功" << endl;
            Sleep(sleep_time);
            break;
        }
        else if (choice == "n") break;
        else cout << "[升级] 选择无效，请重新输入" << endl;
    }
}


void apply_item(map_t& curr_map, player_t& player, int item, int pos)
{
    auto item_pos = (player.n_pos + pos + MAP_SIZE) % MAP_SIZE;
    switch(item) {
        case BLOCK:
            if (!player.n_block) {
                cout << "[道具] 没有路障，无法使用道具" << endl;
            }
            else {
                if (pos > 10 || pos < -10) {
                    cout << "[路障] 路障使用范围错误，只能在前后十格内使用路障" << endl;
                    Sleep(sleep_time);
                }
                else {
                    auto p_players = get_player_vec();
                    for (auto & p : *p_players) {
                        if (p.n_money >= 0 && item_pos == p.n_pos) {
                            cout << "[路障] 不能在玩家处使用路障" << endl;
                            Sleep(sleep_time);
                            return;
                        }
                    }
                    if (item_pos < 0 || item_pos >= MAP_SIZE || item_pos == START_POS ||
                        item_pos == HOSPITAL_POS || item_pos == ITEM_HOUSE_POS || item_pos == GIFT_HOUSE_POS
                        || item_pos == PRISON_POS || item_pos == MAGIC_HOUSE_POS
                        || (item_pos >= 63 && item_pos <= 69)) {
                        cout << "[路障] 不能在特殊位置处使用路障" << endl;
                        Sleep(sleep_time);
                        return;
                    }
                    if (curr_map[item_pos].item == NONE) {
                        curr_map[item_pos].item = BLOCK;
                        player.n_block -= 1;
                        cout << "[路障] 路障放置成功" << endl;
                    }
                }
            }
            break;
        case ROBOT:
            if (!player.n_robot) {
                cout << "[道具] 没有娃娃，无法使用道具" << endl;
            }
            else {
                for (int i = 0; i <= 10; ++i)
                    curr_map[player.n_pos + i].item = NONE;
                player.n_robot -= 1;
                cout << "[机器娃娃] 机器娃娃使用成功" << endl;
            }
            break;
        default:
            break;
    }
}


void buy_item(player_t& player)
{
    if (player.n_bomb + player.n_robot + player.n_block >= 10)
        cout << "[道具] 道具栏已满，无法购买道具" << endl;
    else if (player.n_points < 30)
        cout << "[道具] 点数不足，无法购买道具，自动退出道具屋" << endl;
    else {
        string choice;
        cout << "[道具屋] 欢迎光临道具屋，请选择你需要的道具：" << endl;
        cout << "        1. 路障    2. 机器娃娃" << endl;

        while (true) {
            if (player.n_points < 30) {
                cout << "[道具] 点数不足，已无法再购买任何道具，自动退出道具屋" << endl;
                break;
            }
            show_cmd();
            choice = get_cmd();
            if (choice == "1") {
                if (player.n_points < 50) {
                    cout << "[道具] 点数不足，无法购买路障" << endl;
                }
                else {
                    player.n_points -= 50;
                    player.n_block += 1;
                    cout << "[路障] 购买路障，失去点数 50 点" << endl;
                }
            }
            else if (choice == "2") {
                player.n_points -= 30;
                player.n_robot += 1;
                cout << "[机器娃娃] 购买机器娃娃，失去点数 30 点" << endl;
            }
            else if (choice == "f") return;
            else cout << "[道具屋] 选择无效，请重新输入。输入f退出" << endl;
            if (player.n_bomb + player.n_robot + player.n_block == 10) {
                cout << "[道具] 道具栏已满，已无法再购买道具，自动退出道具屋" << endl;
                break;
            }
        }
    }
    Sleep(sleep_time);
}


void get_gift(player_t& player)
{
    string choice;
    cout << "[礼品屋] 欢迎光临礼品屋，请选择一件你喜欢的礼品：" << endl;
    cout << "        1. 奖金    2. 点数卡    3. 财神" << endl;

    while (true) {
        show_cmd();
        choice = get_cmd();
        if (choice == "1") {
            player.n_money += 2000;
            cout << "[奖金] 获得奖金 2000 元" << endl;
            break;
        }
        else if (choice == "2") {
            player.n_points += 200;
            cout << "[点数] 获得点数 200 点" << endl;
            break;
        }
        else if (choice == "3") {
            player.n_god_buff += 5;
            cout << "[财神] 获得财神附身 5 回合" << endl;
            break;
        }
        else cout << "[礼品屋] 选择无效，退出" << endl; break;
    }
    Sleep(sleep_time);
}


bool roll_dice(map_t& curr_map, player_t& player)
{
    return step_forward(curr_map, player, rand() % 6 + 1);
}


bool pay_rent(p_player_t player)
{
    auto curr_map = *get_map();
    int payment = get_estate_price(curr_map[player->n_pos]) / 2;
    cout << "[租金] 需支付过路费 " << payment << " 元" << endl;
    if (player->b_god_buff) {
        cout << "[财神] 财神附身，无需付钱" << endl;
        Sleep(sleep_time);
        return false;
    }
    else if (player->n_money < payment) {
        cout << "[破产] 嘤嘤嘤破产辽" << endl;
        Sleep(sleep_time);
        return true;
    }
    else {
        curr_map[player->n_pos].owner->n_money += payment;
        player->n_money -= payment;
        Sleep(sleep_time);
        return false;
    }
}


bool step_forward(map_t& curr_map, player_t& player, int steps)
{
    if (steps < 0) return true;
    cout << "向前行进 " << steps << " 步" << endl;
    for (auto it = curr_map[player.n_pos].players.begin();
         it != curr_map[player.n_pos].players.end(); ++it) {
        if (*it == &player) {
            curr_map[player.n_pos].players.erase(it);
            break;
        }
    }

    while (steps--) {
        player.n_pos = (player.n_pos + 1) % MAP_SIZE;
        // item judge
        if (curr_map[player.n_pos].item == BLOCK) {
            curr_map[player.n_pos].item = NONE;
            cout << "[路障] 噢！在这儿停顿" << endl;
            Sleep(sleep_time);
            break;
        }
        else if (curr_map[player.n_pos].item == BOMB) {
            curr_map[player.n_pos].item = NONE;
            player.n_pos = HOSPITAL_POS;
            player.n_empty_rounds = 3;
            cout << "[炸弹] 你炸了！移动至医院，轮空三回合" << endl;
            Sleep(sleep_time);
        }
    }

    // payment judge
    curr_map[player.n_pos].players.push_back(&player);
    switch (curr_map[player.n_pos].type) {
        case VACANCY:
            // 租金
            if (curr_map[player.n_pos].owner &&
                curr_map[player.n_pos].owner != &player
                ) {
                int payment = get_estate_price(curr_map[player.n_pos]) / 2;
                cout << "[租金] 需支付过路费 " << payment << " 元" << endl;
                if (player.b_god_buff) {
                    cout << "[财神] 财神附身，无需付钱" << endl;
                    Sleep(sleep_time);
                }
                else if (player.n_money < payment) {
                    cout << "[破产] 嘤嘤嘤破产辽" << endl;
                    Sleep(sleep_time);
                    return true;
                }
                else {
                    curr_map[player.n_pos].owner->n_money += payment;
                    player.n_money -= payment;
                    Sleep(sleep_time);
                }
            }
            // 升级
            else if (curr_map[player.n_pos].owner &&
                     curr_map[player.n_pos].owner == &player)
                update_estate(curr_map, player);
            // 买房
            else if (!curr_map[player.n_pos].owner)
                buy_estate(curr_map, player);
            return false;

        case ITEM_HOUSE: buy_item(player); return false;
        case GIFT_HOUSE: get_gift(player); return false;
        case MINE:
            player.n_points += curr_map[player.n_pos].value;
            cout << "[矿地] 获得点数 " << curr_map[player.n_pos].value << " 点" << endl;
            Sleep(sleep_time);
            return false;
        case PRISON:
            player.n_empty_rounds = 2;
            cout << "[监狱] 打工是不可能打工的，这辈子都不可能打工的" << endl;
            Sleep(sleep_time);
            return false;
        case MAGIC_HOUSE:
            magic_house();
            return false;
        case PARK:
            cout << "[公园] 进入了公园，本轮摸鱼" << endl;
            Sleep(sleep_time);
            return false;
        default: return false;
    }
}


void magic_house()
{
    string inputs;
    char ntoidx[4] = {'Q', 'A', 'S', 'J'};
    while (true){
        cout << "[魔法屋] 请输入您想陷害的玩家: 1-钱夫人 2-阿土伯 3-孙小美 4-金贝贝，输入0放弃陷害玩家" << endl;
        show_cmd();
        inputs = get_cmd();
        if (check_num(inputs)) {
            auto n = std::stol(inputs);
            if (n == 0) {
                cout << "[魔法屋] 选择放弃陷害玩家，自动退出魔法屋" << endl;
                Sleep(sleep_time);
                break;
            } else if (n < 0 || n > 4) {
                cout << "[魔法屋] 输入玩家编号无效，请重新选择" << endl;
            } else {
                if (!get_player_by_uid(ntoidx[n - 1]) || get_player_by_uid(ntoidx[n - 1])->n_money < 0) {
                    cout << "[魔法屋] 输入角色无效，请重新选择还在场上的角色" << endl;
                    continue;
                }
                get_player_by_uid(ntoidx[n - 1])->n_empty_rounds += 2;
                break;
            }
        } else {
            cout << "[魔法屋] 输入选择无效，请输入正确的数字编号选择陷害玩家" << endl;
        }
    }
}


int get_estate_price(const map_node_t& map_node)
{
    return map_node.value * (map_node.estate_lvl + 1);
}
