#include "game_logic.h"
#include "client.h"
#include "draw.h"
#include "graphics.h"
#include "level_gen.h"
#include "server.h"
#include <Graph_lib/Graph.h>
#include <thread>
#include <unistd.h>

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
        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, GameName};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit) { return; }

        //================ GAME ================
        // резервируем память для игроков и обстаклов
        std::vector<Player> players;
        players.reserve(NumOfPlayers);
        std::vector<Obstacle> obstacles;
        obstacles.reserve(NumOfHugeObs + NumOfMediumObs + NumOfSmallObs);
        std::thread srv(server_test, 12345);//!!!

        // наша очередь стрелять?
        bool my_turn = true;
        // какой игрок стреляет
        size_t shoot_turn = 0;// TODO: МЕНЯТЬ СЧЕТЧИК
        //*declare a server
        if (IM_A_HOST) {// DO IT PARRALLEL
            GenerateObstacles(obstacles);

            Player real_player = Player{UserNick};
            players.push_back(real_player);// наш игрок всегда первый в списке

            // *init a server
            // *listen
        }

        // *decl&init a client
        // *connect

        // *client send
        // *client recv

        // TODO: удалить потом ручное создание игроков !
        Player tmp;
        tmp = Player{"КОРНЕПЛОД"};
        players.push_back(tmp);
        tmp = Player{"ПЕРУН"};
        players.push_back(tmp);
        tmp = Player{"КИШЕМИР"};
        players.push_back(tmp);
        tmp = Player{"СКАРЛАТИНА"};
        players.push_back(tmp);

        // ! while loop HERE
        // *check_game_conditions
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i].NeedResp()) {
                GeneratePlayer(players[i], obstacles, players);
                players[i].Revive();
            }
        }
        game_draw(main_win, players, obstacles);// рендерим

        std::thread clie(game_master);
        clie.join();
        srv.join();
        // *server send
        // *server recv

        main_win.control_show();   // даем управление
        main_win.wait_for_button();// если жмет чета
        // ?PLAYER shoots
        // *client send
        // *client recv ans
        //------
        main_win.control_hide();
        if (wanna_exit && IM_A_HOST) {
        }
    }
}

void game_master() {
    try {
        Client cli{"26.10.48.122", 12345};
        cli.Send("dunke", "1000/x-100", true);
        int res = -1;
        do {
            res = cli.Recv();
        } while (res > 0);
        std::cout << "res=0\n";
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
}