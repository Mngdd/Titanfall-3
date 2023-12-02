// #include "level_gen.h" // FIXME: <---- В ТВОЕМ БЛЯ КОДЕ ПОЛНЫЙ П И З Д Е Ц
//#include "main.h" // бесполезен?
#include "server.h"
#include "client.h"
#include "graphics.h"
#include <string>
#include <exception>
#include <iostream>
#include <thread>

#define DEBUG 0 // переменная дэ ба gue, для всяких сиаутов лишних и т.п.


int main() {
    try {
        #if DEBUG != 0
        // тестовый код сервера начало
        std::string ip = "127.0.0.1";
        int port = 55355;

        std::thread srv(server_test, port); // запускаем тут их сразу же
        std::thread cli(client_test, ip.c_str(), port);

        // ждем завершения их исполнения
        srv.join();
        cli.join();
        // ---------- конец сервер теста ---------
        #endif

        int win_width = 1366, win_height = 768;
        std::string title = "TITANOPAD 3";
        Point zero_point{};
        Game_window : Graph_lib::Window window{zero_point, win_width, win_height, title};

        // #300icq plan
        // init area
        // generate players, obstacles and etc
        // start main menu
        // create a nickname
        // choice --> find or host a game
        //     IF host
        //          init the server
        //          start the server   USE THREADING!!!!!!!!!!!!!!!!!
        //          get the address you need to connect to
        //     ELSE
        //          find the server through the menu
        //          get the address you need to connect to
        // connect_to_socket(address)  (or something like that idk tbh)
        // while (_CONNECTED_)
        //      ?game_process_here?
        // CLEANUP
        // back to menu
        // ??????
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        // FIXME: этот кэтч бесполезный же, не? мы ловим ошибку, НИЧЕГО не делаем и отпускаем
        // FIXME: cerr не в счет - ошибка-то не обработана...
        std::cerr << "Unknown error " << std::endl;
        return 2;
    }
}