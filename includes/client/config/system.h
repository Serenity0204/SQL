#ifndef SYSTEM_H
#define SYSTEM_H

#include <SFML/Graphics.hpp>
#include <string>
using namespace std;

enum FONTS
{
    // ADD MORE FONTS HERE
    ARIAL = 0,
};

// Action code for buttons go here
enum ACTION_CODE
{
    INVALID = -1,
    // add more action code for button here
    PREV = 0,
    NEXT = 1,
};

// add more enum here

// windows
// ADD WINDOWS CONFIG VARIABLE HERE
const int WINDOW_HEIGHT = 650;
const int WINDOW_WIDTH = 1200;
const int INPUT_BOX_FONT_SIZE = 15;
const int HEADER_FONT_SIZE = 14;
const int CHAR_LIMIT = 115;
const int CHAR_DIVIDE = 20;

// system variable config
// ADD SYSTEM  VARIABLE HERE
const sf::Vector2f INPUT_BOX_SIZE = sf::Vector2f(950, 30);
const sf::Vector2f INPUT_BOX_POS = sf::Vector2f(25, 600);
const sf::Vector2f HEADER_SIZE = sf::Vector2f(1150, 550);
const sf::Vector2f HEADER_POS = sf::Vector2f(25, 25);

#endif