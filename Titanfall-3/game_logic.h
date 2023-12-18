#ifndef TITANFALL_3_LOGIC_H
#define TITANFALL_3_LOGIC_H
#include "settings.h"
#include "graphics.h"
#include <FL/Fl.H>
#include <iostream>
#include <map>

void event_close(Fl_Widget *widget, void *);

void game_loop();

void game_master();

void button_updater(Screen* obj, input_data* inp_inf, bool* playin, bool* upd);

struct PlayerOrder {
    PlayerOrder() = default;

    const std::string& get_current_shooter() { return order_list[index]; }
    
    PlayerOrder& operator++();
    
    PlayerOrder& operator--();

    void remove_name(const std::string& name);

    void add_name(const std::string& name);

    int get_total() const { return total_players; }

    void print_order() const;
private:  
    std::map<int, std::string> order_list;
    int total_players;
    int index;

    void update();
};  
#endif//TITANFALL_3_LOGIC_H
