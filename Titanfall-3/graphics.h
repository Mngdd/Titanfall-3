#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <Graph_lib/GUI.h>

#include "enviroment.h"
#include <bits/stdc++.h>
#include <vector>

using namespace Graph_lib;

extern std::vector<Player *> pl; // our players

struct Game_window : Graph_lib::Window // create a new window
{
  Game_window(Point xy, int w, int h, const std::string &title);
  void wait_for_button();

private:
  Button play_button;
  Button quit_button;
  // Button Quit
  bool button_pushed = false;
  static void cb_quit(Address, Address widget);
  void quit() { hide(); }
  // Button Play
  static void cd_play(Address, Address widget);
  void play();
};

#endif