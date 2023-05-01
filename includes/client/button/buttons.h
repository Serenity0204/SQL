#ifndef BUTTONS_H
#define BUTTONS_H

#include <vector>
#include "../config/config.h"
#include "button.h"
#include <SFML/Graphics.hpp>
#include <string>



class Buttons
{
private:
    vector<Button> _buttons;
    void _init_buttons();
public:
    Buttons();
    ~Buttons();
    int update_buttons(sf::RenderWindow &window, sf::Event& event);
    void draw_buttons(sf::RenderWindow &window);
};

#endif // !BUTTONS_H