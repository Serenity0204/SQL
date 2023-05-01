#include "engine.h"

// Engine main
// *****************************************************************************************************************
Engine::Engine()
{
    //                                                  Change the name of the project here
    this->_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project Name");
    this->_window.setKeyRepeatEnabled(true);

    // ADD MORE IN CTOR
    this->_init();
}

// RELEASE MEMORY IN DESTRUCTOR IF USED NEW
Engine::~Engine() {}

// taking input
void Engine::input()
{
    sf::Event event;
    while (this->_window.pollEvent(event))
    {

        // User quit
        if (event.type == sf::Event::Closed)
        {
            cout << "User Quit" << endl;
            this->_window.close();
            break;
        }

        if (event.type == sf::Event::TextEntered)
        {
            bool ctrlC = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::C));
            bool ctrlX = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::X));
            bool ctrlV = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::V));
            bool ctrlA = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::A));
            if (!ctrlC && !ctrlX && !ctrlV && !ctrlA) this->_input_box.typedOn(event);
            break;
        }

        // User press KEY
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            std::stringstream ss;

            string cmd = this->_input_box.getText();
            cout << cmd << endl;
            Table tb = this->_sql.command(cmd);
            ss << tb << endl << endl;
            ss << "selected record numbers:" << tb.select_recnos() << endl;

            string table = ss.str();

            if (this->_sql.is_error())
            {
                cout << "error" << endl;
                this->_header.setHeaderText("error");
            }
            else
            {
                cout << table << endl;
                this->_header.setHeaderText(table);
            }
            this->_input_box.clear_text();
            break;
        }

        // calling update event helper functions
        this->_update_buttons_event(event);
        this->_update_keyboard_event();
        this->_input_box.update_input_box(this->_window, event);
    }
}

// main draw method, update screen
void Engine::display()
{
    // ADD MORE THINGS TO DRAW

    // display the input box for user to enter bet
    this->_input_box.drawTo(this->_window);

    this->_header.drawTo(this->_window);

    // display buttons
    this->_buttons.draw_buttons(this->_window);
}

// run method for game
void Engine::run()
{
    // set the position and font before running
    sf::Font arial = config.get_font(ARIAL);
    this->_input_box.setFont(arial);
    this->_input_box.setLimit(true, CHAR_LIMIT);
    this->_header.setFont(arial);

    // main loop
    while (this->_window.isOpen())
    {
        // taking input
        this->input();
        // clear the screen
        this->_window.clear(sf::Color(0, 102, 0));
        // draw the updated events
        this->display();
        // sfml method to display to the screen
        this->_window.display();
    }
}
// *****************************************************************************************************************

// Init
// *****************************************************************************************************************
// init the attributes
void Engine::_init()
{

    this->_buttons = Buttons();
    this->_header = Header("", HEADER_SIZE, HEADER_POS, HEADER_FONT_SIZE, sf::Color::White, sf::Color::Red);
    this->_input_box = InputBox(INPUT_BOX_FONT_SIZE, INPUT_BOX_SIZE, INPUT_BOX_POS, sf::Color::Red, sf::Color::White, false, "", false);
    this->_sql = SQL();
}
// *****************************************************************************************************************

// update events
// *****************************************************************************************************************

// update keyboard
void Engine::_update_keyboard_event()
{
    // copy
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        if (!this->_input_box.is_selected()) return;
        cout << "copy:" << endl;
        return;
    }
    // cut
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        if (!this->_input_box.is_selected()) return;
        cout << "cut:" << endl;
        return;
    }
    // paste
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::V))
    {
        if (!this->_input_box.is_selected()) return;
        cout << "paste:" << endl;
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (!this->_input_box.is_selected()) return;
        cout << "select" << endl;
        return;
    }
}

// update buttons event
void Engine::_update_buttons_event(sf::Event& event)
{
    // to get which button the player clicks
    int action = this->_buttons.update_buttons(this->_window, event);

    // template for action code
    // if(action == ACTION CODE DEFINED IN SYSTEM)
    // {

    // }

    // ex:
    if (action == PREV)
    {
        cout << "PREV" << endl;
        return;
    }
    if (action == NEXT)
    {
        cout << "NEXT" << endl;
        return;
    }
}

// *****************************************************************************************************************