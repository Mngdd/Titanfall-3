#include "game_logic.h"
#include "client.h"
#include "draw.h"
#include "level_gen.h"
#include "server.h"
#include <Graph_lib/Graph.h>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <utility>
#include <stdexcept>
#include <functional>

void event_close(Fl_Widget *widget, void *) {
    wanna_exit = true;
    Fl_Window *window = dynamic_cast<Fl_Window *>(widget);
    window->hide();
    while (Fl::first_window()) {
        delete Fl::first_window();
    }
}

PlayerOrder& PlayerOrder::operator++() {
    index = (index + 1) % total_players;
    return *this;
}

PlayerOrder& PlayerOrder::operator--() {
    --index;
    if (index < 0)
        index = total_players - 1;
    return *this;
}

void PlayerOrder::add_name(const std::string& name) {
    order_list[total_players] = name;
    ++total_players;
}

void PlayerOrder::remove_name(const std::string& name) {
    int pos = -1;
    for (int x = 1; x < total_players; ++x) {
        if (order_list[x] == name) {
            pos = x;
            break;
        }
    }
    if (pos == -1)
        return;

    order_list.erase(pos);
    update();
}

// проверка на уход номеров игроков в минус не нужна, это невозможно
void PlayerOrder::update() {
    int minus_after = -1;
    for(int x = 1; x < total_players; ++x) {
        if (!order_list.count(x)) {
            minus_after = x;
            break;
        }
    }

    if (minus_after >= 0) {
        --total_players;
        if (index > minus_after) { --index; }

        const int mx_num = order_list.rbegin()->first;
        bool need_another_upd = false;
        for(int x = minus_after + 1; x <= mx_num; ++x) {
            if (order_list.count(x)) {
                auto nodeHandler = order_list.extract(x);
                nodeHandler.key() = x - 1;
                order_list.insert(std::move(nodeHandler));
            } else {
                need_another_upd = true;
            }
        }
        if (need_another_upd){ update(); }
    }

    if (index < 0 || index >= total_players) {
        index = 0;
        std::cout << "WRONG INDEX, MOVING TO ZERO\n";
    }
    if (0 > total_players) {
        total_players = 0;
        std::cout << "WRONG PLAYER AMOUNT, MOVING TO ZERO\n";
    }
}

void PlayerOrder::print_order() const {
    for(auto it = order_list.begin(); 
        it != order_list.end(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
}

void button_updater(Screen* obj, input_data* inp_inf, bool* playin, bool* upd) {
    while(*playin){
        *inp_inf = obj->control_win.wait_for_game_button();
        *upd = true;
        //*playin = false;
}}

void game_loop() {
    while (!wanna_exit) {
        //================ MENU ================
        MY_IP = get_my_ip();
        // главное окно, переименовал для ясности

        bool my_turn = false; // ! менять когда я стреляю!!!
        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, GameName, my_turn};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit) { return; }
        //================ GAME ================
        // резервируем память для игроков и обстаклов
        std::vector<Player> players;
        players.reserve(NumOfPlayers);
        std::vector<Obstacle> obstacles;
        obstacles.reserve(NumOfHugeObs + NumOfMediumObs + NumOfSmallObs);
        PlayerOrder player_control{};

        std::string func_text = default_func;
        bool to_right = true;
        std::string curr_shooter;
        bool playin = true; // мы выходим из игры или нет
        input_data input_info = {game_state::IDLE, default_func, true};
        
        // создаю поток для прослушки, если нужно, будет параллельно запущен
        std::thread listen_thread;
        std::thread btn_info;
        
        Server server_control{PORT};
        Client client_control{};
        // создаем себя, потом отдадим на сервер если мы не хост
        Player real_player = Player{UserNick};
        players.push_back(real_player);
        player_control.add_name(real_player.GetName());
        // создаем сервер
        try { //TODO: CONNECT NEW -> ADD TO CONTROL
            if (IM_A_HOST) {
                GenerateObstacles(obstacles);
                // *init a server&listen
                listen_thread = std::thread(&Server::StartListen, server_control);
            } else { // подключаемся и регистрируемся
                std::cout << "CLI Connect_to...\n";
                client_control.Connect_to(CONN_IP, PORT);
                std::cout << "CLI Registation started...\n";
                client_control.Send(real_player.GetName(), false, true, default_func, true);
                client_control.Recv(players, obstacles, func_text, to_right, curr_shooter); 
                std::cout << "CLI Registation finished!\n";
            }
            bool has_updated = false;
            game_draw(main_win, players, obstacles, input_info, players[0]);
            btn_info = std::thread(&button_updater, &main_win, 
                                &input_info, &playin, &has_updated);
            Player* me_ptr; // указатель на меня, вектор может меняться, поэтому есть real_player
            
            //TODO: sleep 2 secs if host
            std::cout << "====================LOOP START===============\n";
            while (playin) { //TODO: LIMIT MAX PLAYERS!
                my_turn = false;
                // update from server side
                if (IM_A_HOST) {
                    curr_shooter = player_control.get_current_shooter();
                    server_control.SendAll(players, obstacles, func_text, 
                                           to_right, curr_shooter);
                } else {
                    client_control.Recv(players, obstacles, func_text, 
                                        to_right, curr_shooter); //TODO: add timeout
                
                }
                if (curr_shooter == real_player.GetName()) { my_turn = true; }
                std::cout << "phase 1\n";

                std::cout << "phase 2\n";
                // проверяем кнопки
                bool need_input = true;
                has_updated = false;
                while (need_input) {
                    need_input = false;
                    switch(input_info.g_s) {
                        case game_state::IDLE:
                            if (my_turn) {
                                need_input = true;
                                //std::cout << input_info.g_s << "\n";
                            }
                            break;
                        case game_state::FIRE:
                            if (!my_turn) {
                                //TODO: add info msg on false!
                            }
                            need_input = false;
                            std::cout << "phase FIRE\n";
                            break;
                        case game_state::RESTART:
                            if (IM_A_HOST) {
                                //TODO: do smth
                                break;
                            } else {
                                //TODO: not ur server
                            }

                            if (my_turn) {
                                need_input = true;
                            }
                            std::cout << "phase RESTART\n";
                            break;
                        case game_state::RESPAWN:
                            if (me_ptr->IsAlive()) {
                                //TODO: say that im alive
                            } else {
                                me_ptr->awaits_respawn = true;
                            }

                            if (my_turn) {
                                need_input = true;
                            }
                            std::cout << "phase RESPAWN\n";
                            break;
                        case game_state::LEAVE:
                            //TODO:
                            std::cout << "phase LEAVE\n";
                            break;
                        default:
                            std::string err = "UNEXPECTED PARAM FROM game_state: ";
                            err += std::to_string(input_info.g_s);
                            throw std::invalid_argument(err);
                            break;
                    }
                    if (has_updated){
                        input_info.g_s = game_state::IDLE; // рестартим состояние
                        has_updated = false;
                        }                   
                }
                
                
                // обновляем себя
                for (size_t i = 0; i < players.size(); ++i) {
                    std::cout << "phase 2.2 " << players[i].GetName() << "\n";
                    if (players[i].GetName() == real_player.GetName()) {
                        real_player = players[i];
                        //me_ptr = &players[i];
                        //break; // FIXME: это chel почемут работает как continue для while...
                }

                std::cout << "phase 3 " << players[i].GetName() << "\n";
                // update from client side
                bool updated_shooter = false;
                if (IM_A_HOST) {
                    int res = -1;
                    for (auto& pl : players) {
                        if (pl.GetName() == real_player.GetName() && IM_A_HOST) {
                            if (my_turn)
                                func_text = input_info.equation;
                                to_right = input_info.right;
                                updated_shooter = true;
                            continue;
                        }
                        res = server_control.Recv(&pl);
                        if (res == 0) { // disconnected/timeout
                            player_control.remove_name(pl.GetName());
                            for (auto i = players.begin(); i != players.end(); ++i) {
                                if (i->GetName() == pl.GetName()) {
                                    players.erase(i);
                                    break;
                                }}
                        }
                        if (pl.GetName() == curr_shooter) {
                            func_text = pl.func;
                            to_right = pl.right;
                            updated_shooter = true;
                        }}
                } else {
                    client_control.Send(real_player.GetName(), real_player.awaits_respawn, 
                                        real_player.IsAlive(), input_info.equation, input_info.right);
                }
                
                std::cout << "phase 4 " << players[i].GetName() << "\n";
                // revive all
                for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i].NeedResp()) {
                        GeneratePlayer(players[i], obstacles, players);
                        players[i].Revive();
                    }
                }

                std::cout << "phase 5 " << players[i].GetName() << "\n";
                // стрелок не ответил...
                if (!updated_shooter && IM_A_HOST){
                    func_text = default_func;
                    to_right = true;
                    curr_shooter = real_player.GetName();
                }

                std::cout << "phase 5.3 " << players[i].GetName() << "\n";
                // все ввели, теперь заново всем рассылаем
                if (IM_A_HOST) {
                    server_control.SendAll(players, obstacles, func_text, 
                                           to_right, curr_shooter);
                } else {
                    client_control.Recv(players, obstacles, func_text, 
                                        to_right, curr_shooter);
                
                }
                
                std::cout << "phase 5.6 " << players[i].GetName() << "\n";
                // ищем стрелка по нику
                bool found = false;
                const Player* shooter_ptr;
                for (const auto& pl : players) {
                    //std::cout << pl.GetName() << " shtr: " << curr_shooter <<"\n";
                    if (!pl.GetName().compare(curr_shooter)) {
                        shooter_ptr = &pl;
                        found = true;
                        break;
                    }
                }
                Fl::check();
                Fl::lock();
                //SuspendThread(&btn_info);
                std::cout << "phase 6 " << players[i].GetName() << "\n";
                // рисуем если стрелок есть
                    if (found) { //TODO: if func non empty MAYBEEE, test it
                        input_info.equation = func_text;
                        input_info.right = to_right;
                        
                        std::cout << input_info.equation << "\n";
                        game_draw(main_win, players, obstacles, input_info, *shooter_ptr);
                    } else {
                        throw std::out_of_range("got unknown player to draw: " 
                                                + curr_shooter + "!");
                    }
                //ResumeThread(&btn_info);
                //Fl::awake();
                Fl::unlock();



                // main_win.control_show();   // даем управление
                // main_win.wait_for_button();// если жмет чета
                //                            // ?PLAYER shoots
                //                            // *client send
                //                            // *client recv ans
                //                            //------

                // main_win.control_hide();
                if (IM_A_HOST){
                    ++player_control;                    
                }

            }
        }} catch (const std::exception& exp) { //TODO: terminate threads!
            std::cout << exp.what() << std::endl;
            listen_thread.join();
            btn_info.join();
            playin = false;
        }//TODO: make try/catch more local
    }
}

void game_master() {
}
