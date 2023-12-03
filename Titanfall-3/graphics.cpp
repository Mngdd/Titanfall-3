#include "graphics.h"
#include <bits/stdc++.h>

using namespace Graph_lib;

std::vector<Player *> pl;
std::string name1;
std::string ip1;

Game_window::Game_window(Point xy, int w, int h, const std::string &title)
    : Window(xy, w, h, title),
      play_button{Point{50, 50}, 70, 20, "Play", cd_play},
      quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit}

{
  attach(quit_button);
  attach(play_button);
}

void Game_window::cb_quit(Address, Address widget)
{
  auto &btn = reference_to<Button>(widget);
  dynamic_cast<Game_window &>(btn.window()).quit();
}

void Game_window::cd_play(Address, Address widget)
{
  auto &btn = reference_to<Button>(widget);
  dynamic_cast<Game_window &>(btn.window()).play();
}

void Game_window::play()
{
  button_pushed = true;
  this->detach(play_button);
  Simple_window win{Point(500, 100), 200, 160, "data"};

  In_box Name{Point(80, 40), 50, 20, "Name: "}; // entering name
  win.attach(Name);

  In_box IP{Point(80, 90), 50, 20, "Ip: "}; // entering ip
  win.attach(IP);

  win.wait_for_button();

  name1 = Name.get_string(); // reading name
  ip1 = IP.get_string();     // reading ip
  Player *p = new Player{name1, 3, 0, 0};
  pl.push_back(p);
}

void Game_window::wait_for_button() // conservation window
{
  while (!button_pushed && Fl::wait())
    ;
  button_pushed = false;
  Fl::redraw();
}