#include "map.h"
#include "player.h"


map_t init_map()
{
    map_t map;
    for (int i = 0; i < MAP_SIZE; ++i) {
        switch (i) {
            case START_POS: map.emplace_back(START); break;
            case HOSPITAL_POS: map.emplace_back(HOSPITAL); break;
            case ITEM_HOUSE_POS: map.emplace_back(ITEM_HOUSE); break;
            case GIFT_HOUSE_POS: map.emplace_back(GIFT_HOUSE); break;
            case PRISON_POS: map.emplace_back(PRISON); break;
            case MAGIC_HOUSE_POS: map.emplace_back(MAGIC_HOUSE); break;
            case 64: map.emplace_back(MINE, 60); break;
            case 65: map.emplace_back(MINE, 80); break;
            case 66: map.emplace_back(MINE, 40); break;
            case 67: map.emplace_back(MINE, 100); break;
            case 68: map.emplace_back(MINE, 20); break;
            default:
                // 地段1
                if (i > START_POS && i < ITEM_HOUSE_POS && i != HOSPITAL_POS)
                    map.emplace_back(VACANCY, 200);
                // 地段2
                else if (i > ITEM_HOUSE_POS && i < GIFT_HOUSE_POS)
                    map.emplace_back(VACANCY, 500);
                // 地段3
                else if (i > GIFT_HOUSE_POS && i <= MAGIC_HOUSE_POS && i != PRISON_POS)
                    map.emplace_back(VACANCY, 300);
        }
    }
    return map;
}


void plot_map(map_t& map)
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

    char buf=0;
    system("echo \033[0;0H");
    for(int i=0; i<29*8; i++){
        system("echo \033[0m");
        if(hash_table[i] == 0) { putchar(' '); continue; }

        // basic map
        buf = type_table[ map[hash_table[i]].type ];
        if(buf == '0') {
            buf += map[hash_table[i]].estate_lvl;
        }

        // item
        if((map[hash_table[i]].item==BLOCK) || (map[hash_table[i]].item==BOMB))
            buf = item_table[ map[hash_table[i]].item ];

        putchar(buf);
        if(i % 28 == 28) putchar('\n');
    }
}


void buy_estate(map_t& map, player_t& player)
{
    uint8_t map_node_idx = player.n_pos;
    // basic rules
    if (map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner != nullptr){
        return;
    }

    char map_node_input[10];
    int update;
    while (true){
        std::cout << "是否购买房产(1:购买 2:不购买)： ";
        std::cin >> map_node_input;
        update = std::stoi(map_node_input);
        if (update == 1) break;
        if (update == 2) return;
        std::cout << "输入有误";
    }

    // lack of money
    if (map[map_node_idx].value > player.n_money){
        printf("购买失败");
        return;
    }

    // update player info
    player.n_money -= map[map_node_idx].value;
    player.estate.push_back(&map[map_node_idx]);

    // update estate info
    map[map_node_idx].estate_lvl = 0;
    map[map_node_idx].owner = &player;

    printf("升级成功");
}


void update_estate(map_t& map, player_t& player)
{
    uint8_t map_node_idx = player.n_pos;
    // basic rules
    if (map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner == nullptr ||
        map[map_node_idx].owner->uid != player.uid ||
        map[map_node_idx].estate_lvl == SKYSCRAPER){
        return;
    }

    char map_node_input[10];
    int update;
    while (true){
        std::cout << "是否升级房产(1:升级 2:不升级)： ";
        std::cin >> map_node_input;
        update = std::stoi(map_node_input);
        if (update == 1) break;
        if (update == 2) return;
        std::cout << "输入有误";
    }

    // lack of money
    int update_price;
    if      (map_node_idx < 28)  update_price = 200;
    else if (map_node_idx < 35) update_price = 500;
    else    update_price = 300;
    if (update_price > player.n_money){
        printf("升级失败");
        return;
    }

    // update player info
    player.n_money -= update_price;

    // update estate info
    map[map_node_idx].estate_lvl += 1;
    map[map_node_idx].value += update_price;

    printf("升级成功");
}


void sell_estate(map_t& map, player_t& player, uint8_t map_node_idx)
{
    // basic rules
    if (player.b_sell_estate == 1 ||
        map[map_node_idx].type != VACANCY ||
        map[map_node_idx].owner == nullptr ||
        map[map_node_idx].owner->uid != player.uid)
    {
        printf("卖出失败");
        return;
    }

    // update player info
    player.b_sell_estate = 1;
    player.n_money += 2 * map[map_node_idx].value;
    for (int i = 0; i < player.estate.size(); ++i){
        if (player.estate.id == map_node_idx){
            player.estate.erase(i);
            break;
        }
    }

    // update estate info
    map[map_node_idx].owner = nullptr;
    map[map_node_idx].estate_lvl = WASTELAND;
    if (map_node_idx < 28)  map[map_node_idx].value = 200;
    else if (map_node_idx < 35) map[map_node_idx].value = 500;
    else map[map_node_idx].value = 300;

    printf("卖出成功");
}


void apply_item(map_t& map, uint8_t item_type, uint8_t pos)
{

}


void buy_item(player_t& player)
{
    if((player.n_num_props>=10)||(player.n_credit<30))
    {
        printf("您无法购买道具！");
        //退出函数
        return;
    }
    else
    {
        printf("欢迎光临道具屋，请选择您所需要的道具:");
        scanf("%d",&a);
        if (a==1)
        {
            if(player.n_credit<50)
            {
                printf("您的点数不足以购买路障\n");
                return;
            }
            player.n_block += 1;
            player.n_credit -= 50;
        }
        else if (a==2)
        {
            player.n_robot += 1;
            player.n_credit -= 30;
        }
        else if (a==3)
        {
            if(player.n_credit<50)
            {
                printf("您的点数不足以购买炸弹\n");
                return;
            }
            player.n_boom += 1;
            player.n_credit -= 50;
        }
	else 
	{
	    printf("输入无效");
	}
    }
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
            break;
        }
        else if (map[player.n_pos].item == BOMB) {
            map[player.n_pos].item = NONE;
            player.n_pos = HOSPITAL_POS;
            player.n_empty_rounds = 3;
            printf("Bomb! 移动至医院");
        }
    }
    // payment judge
    map[player.n_pos].players.push_back(&player);
    switch (map[player.n_pos].type) {
        case VACANCY:
            if (map[player.n_pos].owner) {
                uint8_t payment = get_estate_price(map[player.n_pos]);
                if (player.n_money < payment) {
                    map[player.n_pos].owner->n_money += player.n_money;
                    printf("嘤嘤嘤破产辽");
                    return true;
                }
                map[player.n_pos].owner->n_money += payment;
                player.n_money -= payment;
            }
            return false;
        case ITEM_HOUSE: buy_item(player); return false;
        case MINE:
            player.n_points += map[player.n_pos].value;
            printf("获得点数");
            return false;
        case PRISON:
            player.n_empty_rounds = 2;
            printf("打工是不可能打工的，这辈子都不可能打工的");
            return false;
        default: return false;
    }
}
