#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

int main(){
    //create window
    //DO NOT MAKE STYLE FULLSCREEN I LEARNT THE HARD WAY
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "tetris", sf::Style::Default);

    //create shape
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    //game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}