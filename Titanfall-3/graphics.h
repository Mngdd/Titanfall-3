#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include "std_lib_facilities.h"
#include <Graph_lib/GUI.h>
using namespace Graph_lib;

struct Game_window : Graph_lib::Window
{
  Game_window(Point xy, int w, int h, const string &title);

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