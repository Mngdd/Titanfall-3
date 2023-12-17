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
        bool my_turn = true;

        Screen main_win{Point(100, 100), FieldWidth, FieldHeight, GameName, my_turn};
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

        GenerationSettings data = GenerationSettings{.NumOfPlayers = NumOfPlayers,
                                                     .NumOfHugeObs = main_win.data[0],
                                                     .NumOfMediumObs = main_win.data[1],
                                                     .NumOfSmallObs = main_win.data[2]};
        ;

        obstacles.reserve(data.NumOfHugeObs + data.NumOfMediumObs + data.NumOfSmallObs);

        // наша очередь стрелять?
        // bool my_turn = true;
        // какой игрок стреляет

        size_t shoot_turn = 0; // TODO: МЕНЯТЬ СЧЕТЧИК

        Player real_player = Player{main_win.nick_input.get_string()};
        players.push_back(real_player); // наш игрок всегда первый в списке
        GenerateObstacles(data, obstacles);

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
                GeneratePlayer(data, players[i], obstacles, players);
                players[i].Revive();
            }
        }
        while (true)
        { // TODO: DELETE ME
            input_data equation = main_win.control_win.wait_for_game_button();
            game_draw(main_win, players, obstacles, equation, players[0]); // рендерим
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