#include "graphics.h"
#include "settings.h"
#include <Fl/Enumerations.H>

#include <bits/stdc++.h>

using namespace Graph_lib;

void control_close(Fl_Widget *widget, void *)
{
    wanna_exit = true;
    Fl_Window *window = dynamic_cast<Fl_Window *>(widget);
    window->hide();
    while (Fl::first_window())
    {
        delete Fl::first_window();
    }
}

void control_not_close(Fl_Widget *widget, void *) {}

void SliderInput::Slider_CB2()
{
    static int recurse = 0;
    if (recurse)
    {
        return;
    }
    else
    {
        recurse = 1;
        char s[80];
        sprintf(s, "%d", (int)(slider->value() + .5));
        // fprintf(stderr, "SPRINTF(%d) -> '%s'\n", (int)(slider->value()+.5), s);
        input->value(s); // pass slider's value to input
        recurse = 0;
    }
}

void SliderInput::Input_CB2()
{
    static int recurse = 0;
    if (recurse)
    {
        return;
    }
    else
    {
        recurse = 1;
        int val = 0;
        if (sscanf(input->value(), "%d", &val) != 1)
        {
            val = 0;
        }
        // fprintf(stderr, "SCANF('%s') -> %d\n", input->value(), val);
        if (val > max_val)
        {
            input->value(std::to_string(max_val).c_str());
            val = max_val;
        }
        else if (val < min_val)
        {
            input->value(std::to_string(min_val).c_str());
            val = min_val;
        }
        slider->value(val); // pass input's value to slider
        recurse = 0;
    }
}

SliderInput::SliderInput(int x, int y, int w, int h,
                         const char *l, int min_, int max_)
    : Fl_Group(x, y, w, h, l), min_val{min_}, max_val{max_}
{
    int in_w = 40;
    input = new Fl_Int_Input(x, y, in_w, h);
    input->callback(Input_CB, (void *)this);
    input->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);

    slider = new Fl_Slider(x + in_w, y, w - in_w, h);
    slider->type(1);
    slider->callback(Slider_CB, (void *)this);

    bounds(min_, max_); // some usable default
    value(min_);        // some usable default
    end();              // close the group
}

Screen::Screen(Point xy, int w, int h, const std::string &title, bool &my_turn)
    : Window(xy, w, h, title),
      host_button{Point{50, 50}, 70, 20, "Host", cd_host},
      join_button{Point{50, 80}, 70, 20, "Join", cd_join},
      quit_button{Point{50, 110}, 70, 20, "Quit", cd_quit},
      nick_input{Point(200, 50), 150, 20, "ur nick: "},
      game_name{Point{30, 30}, GameName}
{
    attach(quit_button);
    attach(join_button);
    attach(host_button);
    attach(nick_input);
    attach(game_name);
    game_name.set_font(Graph_lib::Font::helvetica);
    game_name.set_font_size(20);
    size_range(FieldWidth, FieldHeight, FieldWidth, FieldHeight);
    control_win.hide();
    // control_win.callback(control_close);
    gamin_now = false;

    // this->redraw();
    // status = control_win.wait_for_game_button();
}

Screen::~Screen()
{
    detach(host_button);
    detach(join_button);
    detach(quit_button);
    detach(game_name);
    detach(nick_input);
}

void Screen::cd_quit(Address, Address widget)
{
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<Screen &>(btn.window()).event_quit();
}

void Screen::event_quit()
{
    wanna_exit = true;
    hide();
}

void Screen::cd_host(Address, Address widget)
{
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<Screen &>(btn.window()).event_host();
}

void Screen::event_host()
{
    button_pushed = true;
    Simple_window mini_menu{Point(500, 100),
                            HostWin_x, HostWin_y,
                            "Create a server..."};
    mini_menu.size_range(HostWin_x, HostWin_y, HostWin_x, HostWin_y); // lock size
    mini_menu.next_button.move(-HostWin_x + bg_offset + mini_menu.next_button.width, HostWin_y - bg_offset + 5);

    // show ip
    Out_box ip{Point(40, 20), 100, 20, "Ip:"};
    mini_menu.attach(ip);
    ip.put(MY_IP);

    // input port
    In_box port{Point(200, 20), 50, 20, "Port: "};
    mini_menu.attach(port);

    // change some global vars ---------------------------------------
    mini_menu.Graph_lib::Window::begin();
    // background
    //  кривое, неудобное, обрезаное poop, не использовать
    //  Graph_lib::Rectangle rect{Point{bg_offset, bg_offset + 40},
    //                            HostWin_x - 2 * bg_offset, HostWin_y - 2 * bg_offset - 40};
    //  mini_menu.attach(rect);
    //  // poly.set_style(Line_style{Line_style::dash, 4});
    //  rect.set_fill_color(FL_LIGHT1);
    Fl_Box *box = new Fl_Box(
        bg_offset, bg_offset + 10,
        HostWin_x - 2 * bg_offset, HostWin_y - 2 * bg_offset - 10);
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);

    int x_offset = 10, txt_offset = 45, slide_w = 200, slide_h = 25;

    Text t{Point{bg_offset + 60, btn_begin + 10}, "GAME SETTINGS"};
    mini_menu.attach(t);
    t.set_font(Graph_lib::Font::helvetica);
    t.set_font_size(NickSize);

    SliderInput *sl_0 = new SliderInput(bg_offset + x_offset, btn_begin + txt_offset,
                                        slide_w, slide_h,
                                        "Player amount", NumOfPlayers, 15);

    SliderInput *sl_1 = new SliderInput(bg_offset + x_offset, btn_begin + txt_offset + btn_betw,
                                        slide_w, slide_h,
                                        "Amount of huge obstacles", NumOfHugeObs);

    SliderInput *sl_2 = new SliderInput(bg_offset + x_offset, btn_begin + txt_offset + 2 * btn_betw,
                                        slide_w, slide_h,
                                        "Amount of medium obstacles", NumOfMediumObs);

    SliderInput *sl_3 = new SliderInput(bg_offset + x_offset, btn_begin + txt_offset + 3 * btn_betw,
                                        slide_w, slide_h,
                                        "Amount of small obstacles", NumOfSmallObs);

    mini_menu.Graph_lib::Window::end();
    // end -----------------------------------------------------------

    mini_menu.wait_for_button();
    // get values ---------------------------------------------------

    // получаем значения с ползунков
    NumOfPlayers = sl_0->value();
    NumOfHugeObs = sl_1->value();
    NumOfMediumObs = sl_2->value();
    NumOfSmallObs = sl_3->value();

    // считываем порт
    PORT = port.get_int();
    IM_A_HOST = true;
    // pl_name.push_back(name1);

    // hide_all();
    delete box;
    delete sl_0;
    delete sl_1;
    delete sl_2;
    delete sl_3;

    mini_menu.detach(ip);
    mini_menu.detach(port);
}

void Screen::cd_join(Address, Address widget)
{
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<Screen &>(btn.window()).event_join();
}

void Screen::event_join()
{
    button_pushed = true;
    Simple_window mini_menu{Point(500, 100),
                            ConnWin_x, ConnWin_y,
                            "Connect to the game..."};
    mini_menu.size_range(ConnWin_x, ConnWin_y, ConnWin_x, ConnWin_y); // lock size
    mini_menu.next_button.move(-ConnWin_x + bg_offset + mini_menu.next_button.width,
                               ConnWin_y - bg_offset + 5);

    // show ip
    In_box ip{Point(40, 20), 100, 20, "Ip:"};
    mini_menu.attach(ip);
    // input port
    In_box port{Point(200, 20), 50, 20, "Port: "};
    mini_menu.attach(port);
    mini_menu.wait_for_button();
    // get values ---------------------------------------------------
    CONN_IP = ip.get_string();
    PORT = port.get_int();
    IM_A_HOST = false;

    // hide_all();
    mini_menu.detach(ip);
    mini_menu.detach(port);
}

void Screen::wait_for_button() // conservation window
{
    while (!button_pushed && Fl::wait())
    {
    };
    hide_menu();
    UserNick = nick_input.get_string();

    button_pushed = false;
    Fl::redraw();
}

void Screen::control_show()
{
    control_win.show();
    control_win.set_label("game contol");
    gamin_now = true;
}
void Screen::control_hide()
{
    control_win.hide();
    gamin_now = false;
}

// TODO: ДОДЕЛАТЬ
InputMenu::InputMenu(Point xy, int w, int h, const std::string &title)
    : Window(xy, w, h, title),
      data_output{Point{40, 116}, 305, 20, "info:"},
      func_input{Point{35, 20}, 310, 20, "y = "},
      left{Point{5, 50}, 85, 20,
           "shoot left", left_bt},
      right{Point(105, 50), 85, 20,
            "shoot right", right_bt},
      disconnect{Point(245, 80), 100, 20,
                 "leave game...", leave_bt},
      respawn{Point(105, 80), 85, 20,
              "RESPAWN", respawn_bt},
      restart{Point(195, 50), 150, 20,
              "RESTART GAME", restart_bt},
      fire{Point(5, 80), 85, 20,
           "FIRE", fire_bt}
{
    this->size_range(InputWidth, InputHeight, InputWidth, InputHeight); // lock size
    callback(control_not_close);

    attach(data_output);
    attach(func_input);
    attach(left);
    attach(right);
    attach(disconnect);
    attach(respawn);
    attach(restart);
    attach(fire);
    to_the_right = true;
    game_button_pushed = false;
    // game_state state;
    std::string line;
}
// WAIT_FOR_BUTTON
input_data InputMenu::wait_for_game_button()
{
    this->show();
    while (Fl::wait() && !game_button_pushed)
    {
    };
    game_button_pushed = false;
    Fl::redraw();
    input_data *s = new input_data{
        state, line, to_the_right};
    return *s;
}
// RESTART
void InputMenu::restart_()
{
    state = game_state::RESTART;
    game_button_pushed = true;
}
// RESPAWN
void InputMenu::resp()
{
    state = game_state::RESPAWN;
    game_button_pushed = true;
}
// FIRE
void InputMenu::fr()
{
    line = func_input.get_string();
    state = game_state::FIRE;
    game_button_pushed = true;
}
// LEAVE
void InputMenu::leave()
{
    state = game_state::LEAVE;
    game_button_pushed = true;
}
// RIGHT
void InputMenu::rt()
{
    to_the_right = true;
}
// LEFT
void InputMenu::lt()
{
    to_the_right = false;
    // game_button_pushed = true;
}
InputMenu::~InputMenu()
{
    left.hide();
    right.hide();
    disconnect.hide();
    respawn.hide();
    restart.hide();
    data_output.hide();
    func_input.hide();
}
