#include "game_logic.h"
#include "draw.h"
#include "graphics.h"
#include "level_gen.h"
#include "server.h"
#include <Graph_lib/Graph.h>

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
        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, "TF3"};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit) { return; }

        //================ GAME ================
        std::vector<Player> players(NumOfPlayers);
        std::vector<Obstacle> obstacles(NumOfHugeObs + NumOfMediumObs + NumOfSmallObs);
        if (IM_A_HOST) {// DO IT PARRALLEL
            GenerateObstacles(obstacles);

            Player real_player = Player{"Вы", 530, 150};
            players.push_back(real_player);// наш игрок всегда первый в списке

            //init a server

            // listen

            // generate level

            //------------------
            // recv

            // check_game_conditions

            // send data
        }

        // connect

        // send

        // recv

        // ! удалить потом ручное создание игроков !
        Player tmp;
        tmp = Player{"КОРНЕПЛОД", -1, -1};
        players.push_back(tmp);
        tmp = Player{"ПЕРУН", -1, -1};
        players.push_back(tmp);
        tmp = Player{"КИШЕМИР", -1, -1};
        players.push_back(tmp);
        tmp = Player{"СКАРЛАТИНА", -1, -1};
        players.push_back(tmp);

        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i].NeedResp()) {
                GeneratePlayer(players[i], obstacles, players);
                players[i].Revive();
            }
        }


        game_draw(main_win, players, obstacles);// рендерим
        main_win.control_show();                // даем управление
        main_win.wait_for_button();             // если жмет чета -
        main_win.control_hide();
    }
}