#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

#include "settings.h"
#include "environment.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Slider.H>
#include <Graph_lib/GUI.h>
#include <bits/stdc++.h>
#include <vector>

using namespace Graph_lib;

struct input_data // данные передаваемые кнопкой
{
    game_state g_s;
    std::string equation;
    bool right;
};

// TODO: ДОДЕЛАТЬ
struct InputMenu : Graph_lib::Window
{ // create a new window
    InputMenu() = default;
    InputMenu(Point xy, int w, int h, const std::string &title);
    void wait_for_button();
    ~InputMenu();
    void control_show();
    input_data wait_for_game_button();

private:
    // MENU
    bool close_pressed = false;
    bool to_the_right;
    Out_box data_output;
    In_box func_input;
    Button left;
    Button right;
    Button disconnect;
    Button respawn;
    Button restart;
    Button fire;
    bool game_button_pushed;
    game_state state;
    std::string line;
    game_state GetState() { return state; }

    // RESTART
    static void restart_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).restart_();
    }
    void restart_();
    // RESPAWN
    static void respawn_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).resp();
    }
    void resp();
    // LEAVE
    static void leave_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).leave();
    }
    void leave();
    // left
    static void left_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).lt();
    }
    void lt();
    // right
    static void right_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).rt();
    }
    void rt();
    // fire
    static void fire_bt(Address, Address widget)
    {
        auto &btn = reference_to<Button>(widget);
        dynamic_cast<InputMenu &>(btn.window()).fr();
    }
    void fr();
};

// TODO: ПРИКРУТИТЬ К ИГРЕ НАЗВАНИЕ + ИКОНКУ
struct Screen : Graph_lib::Window
{ // create a new window
    Screen() = default;
    Screen(Point xy, int w, int h, const std::string &title, bool &my_turn);
    void wait_for_button();
    ~Screen();
    void control_show();
    void control_hide();
    bool playing() { return gamin_now; }
    input_data status;
    input_data GetState() { return status; }
    InputMenu control_win{Point(100, 100), InputWidth, InputHeight, "game contol"};

private:
    void hide_menu()
    { // мне лень делать красиво
        host_button.hide();
        join_button.hide();
        quit_button.hide();
        detach(game_name);
        nick_input.hide();
    }

    // MENU
    Text game_name;
    In_box nick_input;
    Button host_button;
    Button join_button;
    Button quit_button;
    bool gamin_now;
    // IN-GAME
    // cd просто для привязки
    // Button Quit
    bool button_pushed = false;
    static void cd_quit(Address, Address widget);
    void event_quit();
    // Button Host
    static void cd_host(Address, Address widget);
    void event_host();
    // Button Join
    static void cd_join(Address, Address widget);
    void event_join();
};

class SliderInput : public Fl_Group
{ // TODO: описать код и в хедер выкинуть!!!
    Fl_Int_Input *input;
    Fl_Slider *slider;
    int min_val = 0;
    int max_val = 10;
    // CALLBACK HANDLERS
    //    These 'attach' the input and slider's values together.
    //
    void Slider_CB2();

    static void Slider_CB(Fl_Widget *w, void *data) { ((SliderInput *)data)->Slider_CB2(); }

    void Input_CB2();
    static void Input_CB(Fl_Widget *w, void *data) { ((SliderInput *)data)->Input_CB2(); }

public:
    // CTOR
    SliderInput(int x, int y, int w, int h, const char *l = 0,
                int min_ = 0, int max_ = 25);
    ~SliderInput()
    {
        delete input;
        delete slider;
    }
    // MINIMAL ACCESSORS --  Add your own as needed
    int value() const { return ((int)(slider->value() + 0.5)); }
    void value(int val)
    {
        slider->value(val);
        Slider_CB2();
    }
    void minumum(int val) { slider->minimum(val); }
    int minumum() const { return ((int)slider->minimum()); }
    void maximum(int val) { slider->maximum(val); }
    int maximum() const { return ((int)slider->maximum()); }
    void bounds(int low, int high) { slider->bounds(low, high); }
};
#endif
