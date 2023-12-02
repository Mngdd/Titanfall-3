#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
// #include "std_lib_facilities.h" // Пожалуйста, без него
#include <Graph_lib/GUI.h>
#include <string>

using namespace Graph_lib;

class Game_window { // все же класс тут больше подойдет чем структура
    // xy - лев верхн угол окна; w,h - ширина и высота, title - имя окна
    Game_window(Point xy, int w, int h, const std::string &title);

private:
    Button play_button;
    Graph_lib::Button quit_button;

private:
    // Button Quit
    static void cb_quit(Address, Address widget);

    void quit() { hide(); }

    // Button Play
    static void cd_play(Address, Address widget);

    void play();
};

#endif