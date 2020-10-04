#include "map.h"
#include "player.h"
#include "cmdline.h"


static map_t map;


p_map_t init_map()
{
    for (int i = 0; i < MAP_SIZE; ++i) {
        switch (i) {
            case START_POS: map.emplace_back(i, START); break;
            case HOSPITAL_POS: map.emplace_back(i, HOSPITAL); break;
            case ITEM_HOUSE_POS: map.emplace_back(i, ITEM_HOUSE); break;
            case GIFT_HOUSE_POS: map.emplace_back(i, GIFT_HOUSE); break;
            case PRISON_POS: map.emplace_back(i, PRISON); break;
            case MAGIC_HOUSE_POS: map.emplace_back(i, MAGIC_HOUSE); break;

            case 64: map.emplace_back(i, MINE, 60); break;
            case 65:
            case 68: map.emplace_back(i, MINE, 80); break;
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


p_map_t get_map() {
    return &map;
}


void plot_map()
{
    static const char hash_table[29*8]={
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
            69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29,
            68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30,
            67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31,
            66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
            65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33,
            64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34,
            63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35
    };

    static const char type_table[8] = {'S','0','T','G','M','H','P','$'};
    static const char player_color_table[4] = {'Q','A','S','J'};
    static const char item_table[4] = {'\0','#','@','R'};
    HANDLE h_out;
    h_out=GetStdHandle(STD_OUTPUT_HANDLE);

    char buf=0;

    //save cursor
    CONSOLE_SCREEN_BUFFER_INFOEX screen_infoex;
    GetConsoleScreenBufferInfoEx(h_out,&screen_infoex);

    SetConsoleCursorPosition(h_out,(COORD){0,0});

    for(int i=0; i<29*8; i++){
        SetConsoleTextAttribute(h_out, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);

        if(hash_table[i] == 0 && i!=0) { putchar(' '); continue; }

        // basic map
        buf = type_table[ map[hash_table[i]].type ];
        if(buf == '0') {
            buf += map[hash_table[i]].estate_lvl;
        }

        // item
        if((map[hash_table[i]].item==BLOCK) || (map[hash_table[i]].item==BOMB))
            buf = item_table[ map[hash_table[i]].item ];

        //player
        if(map[hash_table[i]].players.empty() == false){
            buf=map[hash_table[i]].players.back()->uid;
            switch(map[hash_table[i]].players.back()->e_color){
                case RED:
                    SetConsoleTextAttribute(h_out, FOREGROUND_RED|FOREGROUND_INTENSITY);
                    break;
                case GREEN:
                    SetConsoleTextAttribute(h_out, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                    break;
                case YELLOW:
                    SetConsoleTextAttribute(h_out, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    break;
                case BLUE:
                    SetConsoleTextAttribute(h_out, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                    break;
                default:break;
            }
        }
        putchar(buf);
        if((i % 29) == 28) printf("\n");

    }
    //SetConsoleScreenBufferInfoEx(h)
    //SetConsoleCursorPosition(h_out,(COORD){0,0});
    printf("X:%d, Y%d\n",screen_infoex.dwCursorPosition.X,screen_infoex.dwCursorPosition.Y);
}


void buy_estate(map_t& map, player_t& player)
{
    uint8_t map_node_idx = player.n_pos;
    if (map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner != nullptr ||
        player.n_money < map[map_node_idx].value)
        return;

    string choice;
    cout << "[买房] 是否购买房产？(请输入y/n)" << endl;

    while (true) {
        show_cmd();
        cin >> choice;
        cin.clear();
        cin.sync();
        tolower(choice);
        if (choice == "y") {
            player.n_money -= map[map_node_idx].value;
            player.estate.push_back(&map[map_node_idx]);
            map[map_node_idx].owner = &player;
            break;
        }
        else if (choice == "n") break;
        else cout << "[买房] 选择无效，请重新输入" << endl;
    }
}


void update_estate(map_t& map, player_t& player)
{
    uint8_t map_node_idx = player.n_pos;

    if (map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner == nullptr ||
        map[map_node_idx].owner->uid != player.uid ||
        map[map_node_idx].estate_lvl == SKYSCRAPER ||
        player.n_money < map[map_node_idx].value)
        return;

    string choice;
    cout << "[升级] 升级需支付 " << map[player.n_pos].value << " 元" << endl;
    cout << "      是否需要升级？(y/n)" << endl;

    while (true) {
        show_cmd();
        cin >> choice;
        cin.clear();
        cin.sync();
        tolower(choice);
        if (choice == "y") {
            player.n_money -= map[map_node_idx].value;
            map[map_node_idx].estate_lvl += 1;
            cout << "[升级] 建筑升级成功" << endl;
        }
        else if (choice == "n") break;
        else cout << "[升级] 选择无效，请重新输入" << endl;
    }
}


void apply_item(map_t& map, player_t& player, uint8_t item, uint8_t pos=0)
{
    if (item == BLOCK) {
        if (!player.n_block) {
            cout << "[道具] 没有路障，无法使用道具" << endl;
        }
        else {
            if (pos > 10 || pos < -10 ||
                map[(player.n_pos + pos) % MAP_SIZE].item)
                cout << "[路障] 无法在所选位置放置路障" << endl;
            else {
                map[(player.n_pos + pos) % MAP_SIZE].item = BLOCK;
                cout << "[路障] 路障放置成功" << endl;
            }
        }
    }
    else if (item == ROBOT) {
        if (!player.n_robot) {
            cout << "[道具] 没有娃娃，无法使用道具" << endl;
        }
        else {
            for (int i = 0; i < 10; ++i)
                map[player.n_pos + i].item = NONE;
            player.n_robot -= 1;
            cout << "[机器娃娃] 机器娃娃使用成功" << endl;
        }
    }
    else if (item == BOMB) {
        if (!player.n_block) {
            cout << "[道具] 没有炸弹，无法使用道具" << endl;
        } else {
            if (pos > 10 || pos < -10 ||
                map[(player.n_pos + pos) % MAP_SIZE].item)
                cout << "[炸弹] 无法在所选位置放置路障" << endl;
            else {
                map[(player.n_pos + pos) % MAP_SIZE].item = BLOCK;
                cout << "[炸弹] 炸弹放置成功" << endl;
                cout << "      阿姨，开始你的炸弹秀" << endl;
            }
        }
    }
    return;
}


void buy_item(player_t& player)
{
    if (player.n_bomb + player.n_robot + player.n_block >= 10) {
        cout << "[道具] 道具栏已满，无法购买道具" << endl;
        return;
    }

    string choice;
    cout << "[道具屋] 欢迎光临道具屋，请选择你需要的道具：" << endl;
    cout << "        1. 路障    2. 机器娃娃    3. 炸弹" << endl;

    while (true) {
        show_cmd();
        cin >> choice;
        cin.clear();
        cin.sync();
        tolower(choice);
        if (choice == "1") {
            if (player.n_points < 50) {
                cout << "[道具] 点数不足，无法购买道具" << endl;
            }
            else {
                player.n_points -= 50;
                player.n_block += 1;
                cout << "[路障] 购买路障，失去点数 50 点" << endl;
            }
            break;
        }
        else if (choice == "2") {
            if (player.n_points < 30) {
                cout << "[道具] 点数不足，无法购买道具" << endl;
            }
            else {
                player.n_points -= 30;
                player.n_robot += 1;
                cout << "[机器娃娃] 购买机器娃娃，失去点数 30 点" << endl;
            }
            break;
        }
        else if (choice == "3") {
            if (player.n_points < 50) {
                cout << "[道具] 点数不足，无法购买道具" << endl;
            }
            else {
                player.n_points -= 50;
                player.n_bomb += 1;
                cout << "[炸弹] 购买炸弹，失去点数 50 点" << endl;
            }
            break;
        }
        else cout << "[道具屋] 选择无效，请重新输入。输入q退出" << endl;
    }
    return;
}


void get_gift(player_t& player)
{
    string choice;
    cout << "[礼品屋] 欢迎光临礼品屋，请选择一件你喜欢的礼品：" << endl;
    cout << "        1. 奖金    2. 点数卡    3. 财神" << endl;

    while (true) {
        show_cmd();
        cin >> choice;
        cin.clear();
        cin.sync();
        tolower(choice);
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
            player.n_god_buff = 5;
            cout << "[财神] 获得财神附身 5 回合" << endl;
            break;
        }
        else if (choice == "q") break;
        else cout << "[礼品屋] 选择无效，请重新输入。输入q退出" << endl;
    }
    return;
}


bool roll_dice(map_t& map, player_t& player)
{
    return step_forward(map, player, rand() % 6 + 1);
}


bool step_forward(map_t& map, player_t& player, uint8_t steps)
{
    while (steps--) {
        player.n_pos = (player.n_pos + 1) % MAP_SIZE;
        // item judge
        if (map[player.n_pos].item == BLOCK) {
            map[player.n_pos].item = NONE;
            cout << "[路障] 噢！在这儿停顿" << endl;
            break;
        }
        else if (map[player.n_pos].item == BOMB) {
            map[player.n_pos].item = NONE;
            player.n_pos = HOSPITAL_POS;
            player.n_empty_rounds = 3;
            cout << "[炸弹] 你炸了！移动至医院，轮空三回合" << endl;
        }
    }

    // payment judge
    map[player.n_pos].players.push_back(&player);
    switch (map[player.n_pos].type) {
        case VACANCY:
            // 租金
            if (map[player.n_pos].owner &&
                map[player.n_pos].owner != &player &&
                !map[player.n_pos].owner->n_empty_rounds
                ) {
                uint8_t payment = get_estate_price(map[player.n_pos]) / 2;
                cout << "[租金] 需支付过路费 " << payment << " 元" << endl;
                if (player.n_god_buff)
                    cout << "[财神] 财神附身，无需付钱" << endl;
                else if (player.n_money < payment) {
                    map[player.n_pos].owner->n_money += player.n_money;
                    cout << "[破产] 嘤嘤嘤破产辽，游戏结束" << endl;
                    return true;
                }
                map[player.n_pos].owner->n_money += payment;
                player.n_money -= payment;
            }
            // 升级
            else if (map[player.n_pos].owner &&
                     map[player.n_pos].owner != &player)
                update_estate(map, player);
            // 买房
            else if (!map[player.n_pos].owner)
                buy_estate(map, player);
            return false;

        case ITEM_HOUSE: buy_item(player); return false;
        case GIFT_HOUSE: get_gift(player); return false;
        case MINE:
            player.n_points += map[player.n_pos].value;
            cout << "[矿地] 获得点数 " << map[player.n_pos].value << " 点" << endl;
            return false;
        case PRISON:
            player.n_empty_rounds = 2;
            cout << "[监狱] 打工是不可能打工的，这辈子都不可能打工的" << endl;
            return false;
        default: return false;
    }
}


uint8_t get_estate_price(const map_node_t& map_node)
{
    return map_node.value * (map_node.estate_lvl + 1);
}
