#include "game.h"

#include <stdio.h>
#include <time.h>

#include "cJSON.h"
#include "network_controller.h"
#include "network_protocol.h"

void initialize_game(Game* game) {
    initialize_player(&game->demon,true);
    initialize_player(&game->gambler,false);
    initialize_pistol(&game->pistol);
    game->demon_turn = false; // 默认赌徒先手
    game->round = 1;
}

int shoot(Game* game, const Player* source, Player* target) {
    bool is_real = game->pistol.is_real_bullet[ROUND_NUM - game->pistol.bullet_remaining_num];
    game->pistol.bullet_remaining_num--;

    int damage = 0;
    if (is_real) {
        game->demon.bullets[game->round - 1] = 1;
        game->gambler.bullets[game->round - 1] = 1;

        if (source->knife_use) {
            damage = 2;
        }
        else {
            damage = 1;
        }
        calculate_damage(target, damage);
    }
    else {
        game->demon.bullets[game->round - 1] = -1;
        game->gambler.bullets[game->round - 1] = -1;
    }

    if (source != target || is_real) {
        game->demon_turn = !game->demon_turn;
    }

    return damage;
}

bool is_game_over(const Game* game) {
    return game->gambler.hp <= 0 || game->demon.hp <= 0;
}

void start_pvp_game(Game* game, SOCKET demon_socket, SOCKET gambler_socket) {
    setbuf(stdout,NULL);

    srand(time(NULL));
    initialize_game(game);

    Player* demon = &game->demon;
    Player* gambler = &game->gambler;
    Pistol* pistol = &game->pistol;

    // ReSharper disable CppJoinDeclarationAndAssignment
    // ReSharper disable CppTooWideScope

    cJSON* game_begin_data;
    cJSON* game_over_data;
    cJSON* game_state_data;
    cJSON* item_assign_data;
    cJSON* item_use_data;
    cJSON* phone_result_data;
    cJSON* player_shoot_data;
    cJSON* player_state_data;
    cJSON* shoot_result_data;

    // 就绪同步
    {
        game_begin_data = build_game_begin_data();
        send_type_json_message(demon_socket, GAME_BEGIN, game_begin_data);
        receive_json_message(demon_socket);

        game_begin_data = build_game_begin_data();
        send_type_json_message(gambler_socket, GAME_BEGIN, game_begin_data);
        receive_json_message(gambler_socket);
    }

    printf("Game begins.\n");

    // 发送初始玩家状态
    {
        player_state_data = build_player_state_data(demon);
        send_type_json_message(demon_socket, PLAYER_STATE, player_state_data);
        receive_json_message(demon_socket);

        player_state_data = build_player_state_data(gambler);
        send_type_json_message(gambler_socket, PLAYER_STATE, player_state_data);
        receive_json_message(gambler_socket);
    }

    // 发送初始游戏状态
    {
        game_state_data = build_game_state_data(game->demon_turn, gambler, 1, 0);
        send_type_json_message(demon_socket, GAME_STATE, game_state_data);
        receive_json_message(demon_socket);

        game_state_data = build_game_state_data(!game->demon_turn, demon, 1, 0);
        send_type_json_message(gambler_socket, GAME_STATE, game_state_data);
        receive_json_message(gambler_socket);
    }

    // 每局
    while (!is_game_over(game)) {
        // 生成子弹
        generate_bullets(pistol);

        // 分配道具
        {
            assign_items(demon);
            item_assign_data = build_item_assign_data(demon->knife_num,
                                                      demon->beer_num,
                                                      demon->phone_num);
            send_type_json_message(demon_socket, ITEM_ASSIGN, item_assign_data);
            receive_json_message(demon_socket);

            assign_items(gambler);
            item_assign_data = build_item_assign_data(gambler->knife_num,
                                                      gambler->beer_num,
                                                      gambler->phone_num);
            send_type_json_message(gambler_socket, ITEM_ASSIGN, item_assign_data);
            receive_json_message(gambler_socket);
        }

        // 每回合
        for (game->round; game->round <= ROUND_NUM; game->round++) {
            Player* current_player = game->demon_turn == true ? demon : gambler;
            Player* opponent_player = game->demon_turn == false ? demon : gambler;
            SOCKET current_socket = game->demon_turn == true ? demon_socket : gambler_socket;
            SOCKET opponent_socket = game->demon_turn == false ? demon_socket : gambler_socket;

            /*
             * 通知游戏状态
             * - 当前持枪者
             * - 当前回合数
             * - 当局实弹数
             */
            {
                game_state_data = build_game_state_data(true,
                                                        opponent_player,
                                                        game->round,
                                                        pistol->real_bullet_num);
                send_type_json_message(current_socket, GAME_STATE, game_state_data);
                receive_json_message(current_socket);

                game_state_data = build_game_state_data(false,
                                                        current_player,
                                                        game->round,
                                                        pistol->real_bullet_num);
                send_type_json_message(opponent_socket, GAME_STATE, game_state_data);
                receive_json_message(opponent_socket);
            }

            // 执行当前玩家动作
            {
                while (true) {
                    cJSON* recv_json = receive_json_message(current_socket);
                    acknowledge(current_socket);

                    NetMessageType recv_type = string_to_type(cJSON_GetObjectItem(recv_json, "type")->valuestring);
                    // 如果使用道具
                    if (recv_type == ITEM_USE) {
                        item_use_data = cJSON_GetObjectItem(recv_json, "data");

                        // 道具使用效果结算
                        char* item = cJSON_GetObjectItem(item_use_data, "item")->valuestring;
                        if (strcmp(item, "KNIFE") == 0) {
                            use_knife(current_player);
                        }
                        else if (strcmp(item, "BEER") == 0) {
                            use_beer(current_player);
                        }
                        else if (strcmp(item, "PHONE") == 0) {
                            phone_result_data = use_phone(current_player, pistol);
                            send_type_json_message(current_socket, PHONE_RESULT, phone_result_data);
                            receive_json_message(current_socket);
                        }
                        else {
                            exit(777);
                        }

                        // 更新当前玩家状态
                        {
                            player_state_data = build_player_state_data(current_player);
                            send_type_json_message(current_socket, PLAYER_STATE, player_state_data);
                            receive_json_message(current_socket);
                        }

                        // 更新对手视野状态
                        {
                            send_type_json_message(opponent_socket, ITEM_USE, item_use_data);
                            receive_json_message(opponent_socket);
                        }
                        {
                            game_state_data = build_game_state_data(false,
                                                                    opponent_player,
                                                                    game->round,
                                                                    pistol->real_bullet_num);
                            send_type_json_message(opponent_socket, GAME_STATE, game_state_data);
                            receive_json_message(opponent_socket);
                        }
                    }
                    // 如果开枪
                    else if (recv_type == PLAYER_SHOOT) {
                        player_shoot_data = cJSON_GetObjectItem(recv_json, "data");
                        // 获取开枪结果
                        bool is_attack = cJSON_GetObjectItem(player_shoot_data, "is_attack")->valueint; // 0 - 朝自己开枪
                        Player* source = current_player;
                        Player* target = is_attack ? opponent_player : current_player;
                        int damage = shoot(game, source, target);

                        // 通知开枪结果
                        {
                            shoot_result_data = build_shoot_result_data(is_attack,
                                                                        damage);

                            send_type_json_message(current_socket, SHOOT_RESULT, shoot_result_data);
                            receive_json_message(current_socket);

                            shoot_result_data = build_shoot_result_data(is_attack,
                                                                        damage);
                            send_type_json_message(opponent_socket, SHOOT_RESULT, shoot_result_data);
                            receive_json_message(opponent_socket);
                        }
                        break;
                    }
                    else { exit(777); }
                }
            }

            // 如果游戏结束
            if (is_game_over(game)) {
                bool demon_is_winner = gambler->hp <= 0;
                // 发送游戏结果
                {
                    game_over_data = build_game_over_data(demon_is_winner);
                    send_type_json_message(demon_socket, GAME_OVER, game_over_data);
                    receive_json_message(demon_socket);

                    game_over_data = build_game_over_data(!demon_is_winner);
                    send_type_json_message(gambler_socket, GAME_OVER, game_over_data);
                    receive_json_message(gambler_socket);
                }

                break;
            }
        }
    }
}
