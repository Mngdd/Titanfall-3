#include "game_logic.h"
#include "draw.h"
#include "graphics.h"
#include "level_gen.h"
#include "server.h"
#include <Graph_lib/Graph.h>

void event_close(Fl_Widget *widget, void *)
{
    wanna_exit = true;
    Fl_Window *window = dynamic_cast<Fl_Window *>(widget);
    window->hide();
    while (Fl::first_window())
    {
        delete Fl::first_window();
    }
}

void game_loop()
{
    while (!wanna_exit)
    {
        //================ MENU ================
        MY_IP = get_my_ip();
        // главное окно, переименовал для ясности
        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, GameName};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit)
        {
            return;
        }

        //================ GAME ================
        // резервируем память для игроков и обстаклов
        std::vector<Player> players;
        players.reserve(NumOfPlayers);
        std::vector<Obstacle> obstacles;
        obstacles.reserve(NumOfHugeObs + NumOfMediumObs + NumOfSmallObs);

        // наша очередь стрелять?
        bool my_turn = true;
        // какой игрок стреляет
        size_t shoot_turn = 0; // TODO: МЕНЯТЬ СЧЕТЧИК

        Player real_player = Player{UserNick};
        players.push_back(real_player); // наш игрок всегда первый в списке

        //*declare a server
        if (IM_A_HOST)
        { // DO IT PARRALLEL
            GenerateObstacles(obstacles);

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
        for (size_t i = 0; i < players.size(); ++i)
        {
            if (players[i].NeedResp())
            {
                GeneratePlayer(players[i], obstacles, players);
                players[i].Revive();
            }
        }
        for (int i = 0; i < 1000; ++i)
        { // TODO: DELETE ME
            input_data equation = main_win.control_win.wait_for_game_button();
            game_draw(main_win, players, obstacles, equation, real_player); // рендерим
        }

        // *server send
        // *server recv

        main_win.control_show();    // даем управление
        main_win.wait_for_button(); // если жмет чета
        // *client shoots
        // *client send
        // *client recv ans
        //------
        main_win.control_hide();
    }
}

void game_master()
{
}