#ifndef ENGINE_H
#define ENGINE_H

#include "../../sql/sql.h"
#include "../button/buttons.h"
#include "../config/config.h"
#include "../header/header.h"
#include "../input_box/input_box.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

using namespace std;

class Engine
{
private:
    // sfml private members here
    sf::RenderWindow _window;

    // system setup here
    InputBox _input_box;
    Buttons _buttons;
    Header _header;
    // private member variables here
    SQL _sql;
    // private helper functions here
    // init
    void _init();
    // update
    void _update_buttons_event(sf::Event& event);
    void _update_keyboard_event();

public:
    // friend class
    friend class Middleware;
    // constructors and destructors
    Engine();
    ~Engine();
    // game processing
    void input();
    void run();
    void display();
};

#endif