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
    // получаем настройки
    GenerationSettings data = GenerationSettings{.NumOfPlayers = NumOfPlayers,
                                                 .NumOfHugeObs = main_win.data[0],
                                                 .NumOfMediumObs = main_win.data[1],
                                                 .NumOfSmallObs = main_win.data[2]};
    ;

    obstacles.reserve(data.NumOfHugeObs + data.NumOfMediumObs + data.NumOfSmallObs);

    Player real_player = Player{main_win.nick_input.get_string()}; // получаем наше имя
    players.push_back(real_player);                                // наш игрок всегда первый в списке
    GenerateObstacles(data, obstacles);                            // генерируем препятствия
    // добавляем мишени
    Player tmp;
    tmp = Player{"КОРНЕПЛОД"};
    players.push_back(tmp);
    tmp = Player{"ПЕРУН"};
    players.push_back(tmp);
    tmp = Player{"КИШЕМИР"};
    players.push_back(tmp);
    tmp = Player{"СКАРЛАТИНА"};
    players.push_back(tmp);

    std::string str = "Your Score: ";
    std::string cur_record = "  Current Record: ";
    int score{0}; // начальный счет
    input_data equation{.g_s = game_state::RESPAWN};
    game_draw(main_win, players, obstacles, equation, players[0]);
    for (int i{0}; i < 5; ++i)
    {
        score = 0;
        input_data equation{.g_s = game_state::RESPAWN};
        game_draw(main_win, players, obstacles, equation, players[0]); // перерисовываем
        for (int i{0}; i < 10; ++i)
        {
            input_data equation = main_win.control_win.wait_for_game_button(); // играем
            game_draw(main_win, players, obstacles, equation, players[0]);
            if (equation.g_s == game_state::RESPAWN)
            { // что кнопка respawn не забирала наши попытки
                i = 0;
                score = 0;
            }
        }
        for (size_t i{0}; i < players.size(); ++i)
            if (!players[i].IsAlive())
            {
                players[i].SetAlive();
                std::cout << '1'; // увеличиваем счет
                ++score;
            }
        main_win.control_win.record = std::max(main_win.control_win.record, score);
        main_win.control_win.print_text(str + std::to_string(score) +
                                        cur_record + std::to_string(main_win.control_win.record)); // выводим счет и рекорд
        main_win.control_win.wait_for_game_button();
        main_win.callback(event_close);

        // main_win.control_win.wait_for_game_button();

        // main_win.control_show();    // даем управление
        // main_win.wait_for_button(); // если жмет чета

        // *server send
        // *server recv

        // *client shoots
        // *client send
        // *client recv ans
        //------
        // }
    }
    // main_win.contro;
}
