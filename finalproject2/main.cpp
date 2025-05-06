#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

//global variables or constants
//create window, user makes full screen to play, closes to end program
sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "tetris", sf::Style::Default);
sf::Event event;
sf::RectangleShape background(sf::Vector2f(240.f, 440.f));

//function and class prototypes
void redrawBackground();
class Tetromino;
class O;
class I;
class S;
class Z;
class L;
class J;
class T;

class Tetromino {
protected:
public:
    virtual void rotate() = 0;
};
class O:public Tetromino {
private:
public:
    void rotate()override;
};
class I :public Tetromino {
private:
public:
    void rotate()override;
};
class S :public Tetromino {
private:
public:
    void rotate()override;
};
class Z :public Tetromino {
private:
public:
    void rotate()override;
};
class L :public Tetromino {
private:
public:
    void rotate()override;
};
class J :public Tetromino {
private:
public:
    void rotate()override;
};
class T :public Tetromino {
private:
public:
    void rotate()override;
};

int main(){
    //colours
    background.setOutlineColor(sf::Color(143, 188, 143));
    background.setOutlineThickness(2.f);
    background.setFillColor(sf::Color::Black);
    background.setPosition(150, 20);
    //synchronise frame rate with vertical frequency of monitor
    window.setVerticalSyncEnabled(true); 
    //window.setFramerateLimit(60); 
    //if above doens't work, change graphics card settings or use this instead

    //menu loop
    
    //game loop
    while (window.isOpen())
    {
        //downwards movement of tetrominoes
        //redraw background
        redrawBackground();
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

//functions
void redrawBackground() {
    window.clear(sf::Color::Black);
    window.draw(background);
    window.display();
}

//member functions
void O::rotate(){}
void I::rotate(){}
void S::rotate(){}
void Z::rotate(){}
void L::rotate(){}
void J::rotate(){}
void T::rotate(){}