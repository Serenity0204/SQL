#include "buttons.h"

Buttons::Buttons()
{
    this->_buttons = vector<Button>();
    this->_init_buttons();
}

Buttons::~Buttons() {}

// init buttons used in this project
void Buttons::_init_buttons()
{
    // example

    Button prev_btn("PREV COMMAND", {95, 30}, 9, sf::Color::White, sf::Color::Red);
    prev_btn.setFont(config.get_font(ARIAL));
    prev_btn.setPosition({985, 600});
    this->_buttons.push_back(prev_btn);

    Button next_btn("NEXT COMMAND", {95, 30}, 9, sf::Color::White, sf::Color::Red);
    next_btn.setFont(config.get_font(ARIAL));
    next_btn.setPosition({1090, 600});
    this->_buttons.push_back(next_btn);
}

// return action code
int Buttons::update_buttons(sf::RenderWindow& window, sf::Event& event)
{
    for (int i = 0; i < this->_buttons.size(); ++i)
    {
        if (this->_buttons[i].isMouseOver(window))
        {
            this->_buttons[i].setBackColor(sf::Color::Yellow);
            continue;
        }
        this->_buttons[i].setBackColor(sf::Color::White);
    }
    for (int i = 0; i < this->_buttons.size(); ++i)
    {
        bool clicked = this->_buttons[i].isMouseOver(window) && event.type == sf::Event::MouseButtonPressed;
        // if(clicked && i == ACTION CODE DEFINED IN SYSTEM) return ACTION CODE DEFINED IN SYSTEM;
        // ex:
        if (clicked && i == PREV) return PREV;
        if (clicked && i == NEXT) return NEXT;
    }
    return INVALID;
}

void Buttons::draw_buttons(sf::RenderWindow& window)
{
    for (int i = 0; i < this->_buttons.size(); ++i)
    {
        this->_buttons[i].setFont(config.get_font(ARIAL));
        this->_buttons[i].drawTo(window);
    }
}