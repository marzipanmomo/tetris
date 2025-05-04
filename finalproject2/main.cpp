#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

int main(){
    //create window, user makes full screen to play, closes to end program
    sf::Window window(sf::VideoMode({ 800, 600 }), "tetris", sf::Style::Default);
    //synchronise frame rate with vertical frequency of monitor
    window.setVerticalSyncEnabled(true); 
    //window.setFramerateLimit(60); 
    //if above doens't work, change graphics card settings or use this instead
    
    sf::Event event;

    //menu loop
    
    //game loop
    while (window.isOpen())
    {
        //downwards movement of tetrominoes
        //redraw background
        //update high score
       
        //check all the window's events that were triggered since the last iteration of the loop
        //while there are pending events
        while (window.pollEvent(event))
        {

            //check type of event
            //"key pressed" event
            if (event.type == sf::Event::KeyPressed) {
                //redraw and move tetrominoes
                
                //check esc pressed- pause screen
                if (event.key.scancode == sf::Keyboard::Scan::Escape){
                    //pause screen
                }

            }

            // "close requested" event: we close the window
            else if (event.type == sf::Event::Closed) {
                window.close();
            }

            else{}
        }
    }
}