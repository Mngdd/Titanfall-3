#include "graphics.h"
#include "settings.h"
#include <Fl/Enumerations.H>

#include <Graph_lib/GUI.h>
#include <bits/stdc++.h>

using namespace Graph_lib;

std::string *equation;

void SliderInput::Slider_CB2() {
    static int recurse = 0;
    if (recurse) {
        return;
    } else {
        recurse = 1;
        char s[80];
        sprintf(s, "%d", (int) (slider->value() + .5));
        // fprintf(stderr, "SPRINTF(%d) -> '%s'\n", (int)(slider->value()+.5), s);
        input->value(s);// pass slider's value to input
        recurse = 0;
    }
}

void SliderInput::Input_CB2() {
    static int recurse = 0;
    if (recurse) {
        return;
    } else {
        recurse = 1;
        int val = 0;
        if (sscanf(input->value(), "%d", &val) != 1) {
            val = 0;
        }
        // fprintf(stderr, "SCANF('%s') -> %d\n", input->value(), val);
        if (val > max_val) {
            input->value(std::to_string(max_val).c_str());
            val = max_val;
        } else if (val < min_val) {
            input->value(std::to_string(min_val).c_str());
            val = min_val;
        }
        slider->value(val);// pass input's value to slider
        recurse = 0;
    }
}

SliderInput::SliderInput(int x, int y, int w, int h,
                         const char *l, int min_, int max_)
    : Fl_Group(x, y, w, h, l), min_val{min_}, max_val{max_} {
    int in_w = 40;
    input = new Fl_Int_Input(x, y, in_w, h);
    input->callback(Input_CB, (void *) this);
    input->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);

    slider = new Fl_Slider(x + in_w, y, w - in_w, h);
    slider->type(1);
    slider->callback(Slider_CB, (void *) this);

    bounds(min_, max_);// some usable default
    value(min_);       // some usable default
    end();             // close the group
}

MainMenu::MainMenu(Point xy, int w, int h, const std::string &title)
    : Window(xy, w, h, title),
      host_button{Point{50, 50}, 70, 20, "Host", cd_host},
      join_button{Point{50, 80}, 70, 20, "Join", cd_join},
      quit_button{Point{50, 110}, 70, 20, "Quit", cd_quit} {
    attach(quit_button);
    attach(join_button);
    attach(host_button);
}

void MainMenu::cd_quit(Address, Address widget) {
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<MainMenu &>(btn.window()).quit();
}

void MainMenu::quit() {
    wanna_exit = true;
    hide();
}

void MainMenu::cd_host(Address, Address widget) {
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<MainMenu &>(btn.window()).host();
}

void MainMenu::host() {
    button_pushed = true;
    Simple_window mini_menu{Point(500, 100),
                            HostWin_x, HostWin_y,
                            "Create a server..."};
    mini_menu.size_range(HostWin_x, HostWin_y, HostWin_x, HostWin_y);// lock size
    mini_menu.next_button.move(-HostWin_x + bg_offset + mini_menu.next_button.width, HostWin_y - bg_offset + 5);

    // show ip
    Out_box ip{Point(40, 20), 100, 20, "Ip:"};
    mini_menu.attach(ip);
    ip.put(MY_IP);

    // input port
    In_box port{Point(200, 20), 50, 20, "Port: "};
    mini_menu.attach(port);

    //change some global vars ---------------------------------------
    mini_menu.Graph_lib::Window::begin();
    //background
    // кривое, неудобное, обрезаное poop, не использовать
    // Graph_lib::Rectangle rect{Point{bg_offset, bg_offset + 40},
    //                           HostWin_x - 2 * bg_offset, HostWin_y - 2 * bg_offset - 40};
    // mini_menu.attach(rect);
    // // poly.set_style(Line_style{Line_style::dash, 4});
    // rect.set_fill_color(FL_LIGHT1);
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
    t.set_font_size(20);

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
    //end -----------------------------------------------------------

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

    //hide_all();
    delete box;
    delete sl_0;
    delete sl_1;
    delete sl_2;
    delete sl_3;

    mini_menu.detach(ip);
    mini_menu.detach(port);
}

void MainMenu::cd_join(Address, Address widget) {
    auto &btn = reference_to<Button>(widget);
    dynamic_cast<MainMenu &>(btn.window()).join();
}

void MainMenu::join() {
    button_pushed = true;
    Simple_window mini_menu{Point(500, 100),
                            ConnWin_x, ConnWin_y,
                            "Connect to the game..."};
    mini_menu.size_range(ConnWin_x, ConnWin_y, ConnWin_x, ConnWin_y);// lock size
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
    IM_A_HOST = true;

    //hide_all();
    mini_menu.detach(ip);
    mini_menu.detach(port);
}

void MainMenu::wait_for_button()// conservation window
{
    while (!button_pushed && Fl::wait());
    button_pushed = false;
    Fl::redraw();
}