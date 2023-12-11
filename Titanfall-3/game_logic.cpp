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
        Screen main_win{Point(100, 100), FieldHeight, FieldWidth, "TF3"};
        main_win.callback(event_close);
        main_win.wait_for_button();
        if (wanna_exit) { return; }
        //================ GAME ================
        if (IM_A_HOST) {// DO IT PARRALLEL
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

        std::vector<Player> players(6);

        Player tmp;
        tmp = Player{"КОРНЕПЛОД", 130, 130};
        players.push_back(tmp);
        tmp = Player{"ПЕРУН", 863, 204};
        players.push_back(tmp);
        tmp = Player{"КИШЕМИР", 590, 550};
        players.push_back(tmp);
        tmp = Player{"СКАРЛАТИНА", 200, 170};
        players.push_back(tmp);

        Generate();
        game_draw(main_win, players);
        main_win.wait_for_button();
    }
}