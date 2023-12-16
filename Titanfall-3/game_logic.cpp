#include "game_logic.h"
#include "client.h"
#include "draw.h"
#include "graphics.h"
#include "level_gen.h"
#include "server.h"
#include <Graph_lib/Graph.h>
#include <thread>
#include <unistd.h>
#define SRV_TEST 1

void event_close(Fl_Widget *widget, void *) {
    wanna_exit = true;
    Fl_Window *window = dynamic_cast<Fl_Window *>(widget);
    window->hide();
    while (Fl::first_window()) {
        delete Fl::first_window();
    }
}

void game_loop() {
    while (!wanna_exit) {
        //================ MENU ================
        MY_IP = get_my_ip();
        // главное окно, переименовал для ясности
        bool my_turn = true;

        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, GameName, my_turn};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit) {
            return;
        }

        //================ GAME ================
        // резервируем память для игроков и обстаклов
        std::vector<Player> players;
        players.reserve(NumOfPlayers);
        std::vector<Obstacle> obstacles;
        obstacles.reserve(NumOfHugeObs + NumOfMediumObs + NumOfSmallObs);
        std::string func_text = "x";
#if SRV_TEST == 1
        //std::thread srv;
        std::thread srv_manager;
#endif

        // наша очередь стрелять?
        bool playin = true;
        // какой игрок стреляет
        size_t shoot_turn = 0;// TODO: МЕНЯТЬ СЧЕТЧИК

        Player real_player = Player{UserNick}; //todo: искать себя по нику а не так вот
        players.push_back(real_player);// наш игрок всегда первый в списке

        //*declare a server
        if (IM_A_HOST) {// DO IT PARRALLEL
            GenerateObstacles(obstacles);

            // *init a server
#if SRV_TEST == 1
            //srv = std::thread(server_test, 12345);
            srv_manager = std::thread(game_master);
#endif
            // *listen
        }

        // *decl&init a client + connect
#if SRV_TEST == 1
        Client cli{"26.10.48.122", 12345};//FIXME: hardcoded

        // *client send
        //cli.Send(UserNick, "sin(1/x)", false, false);
        // *client recv

        int res = -1;
        // do {
        //     res = cli.Recv();
        // } while (res > 0);
        std::cout << res << " res=0\n";
#endif

        // TODO: удалить потом ручное создание игроков !
        Player tmp;
        // tmp = Player{"КОРНЕПЛОД"};
        // players.push_back(tmp);
        // tmp = Player{"ПЕРУН"};
        // players.push_back(tmp);
        // tmp = Player{"КИШЕМИР"};
        // players.push_back(tmp);
        // tmp = Player{"СКАРЛАТИНА"};
        // players.push_back(tmp);

        while (playin) {
            // *check_game_conditions
            for (size_t i = 0; i < players.size(); ++i) {
                if (players[i].NeedResp()) {
                    GeneratePlayer(players[i], obstacles, players);
                    players[i].Revive();
                }
            }

            //FIXME: пока кринж какой-то
            game_draw(main_win, players, obstacles, input_data{game_state::FIRE, "x", true}, players[0]);// рендерим
            for (int i = 0; i < 1000; ++i) {// TODO: DELETE ME
                input_data equation = main_win.control_win.wait_for_game_button();
                game_draw(main_win, players, obstacles, equation, players[0]);// рендерим
            }

            //std::thread clie(game_master);
            //clie.join();
            //srv.join();
            // *server send
            // *server recv

            main_win.control_show();   // даем управление
            main_win.wait_for_button();// если жмет чета
                                       // ?PLAYER shoots
                                       // *client send
                                       // *client recv ans
                                       //------

#if SRV_TEST == 1
            main_win.control_hide();
            if ((wanna_exit || !playin) && IM_A_HOST) {
                //srv.join();// хочю выйти...
                srv_manager.join();
            }
#endif
        }
    }
}

void game_master() {
}