#include "graphics.h"
using namespace Graph_lib;

Game_window::Game_window(Point xy, int w, int h, const string &title)
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
    Simple_window win{Point(500, 100), 200, 150, "PLAY!!!"};

    In_box data{Point(40, 30), 50, 20, "DATA"};
    win.attach(data);

    win.wait_for_button();

    Out_box ot{Point(40, 60), 50, 20, "OUT"};

    win.attach(ot);
    this->attach(ot);
    win.wait_for_button();
    this->detach(ot);
  }