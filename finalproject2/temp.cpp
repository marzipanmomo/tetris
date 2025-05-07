#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>//used this library for srand function
class Piece {
protected:
    int shape[4][4];//4x4 matrix for each shapes representation.
    sf::Color color;
    int x, y;//co ordinats of the shapes.
public:
    Piece() : x(0), y(0) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
    }
    virtual ~Piece()
    {
    }
    virtual void rotate() = 0;//making a pure virtual function and making the base class abstract.
    void draw(sf::RenderWindow& window, int cellSize) {
        sf::RectangleShape block(sf::Vector2f(cellSize - 1, cellSize - 1));
        block.setFillColor(color);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (shape[i][j] == 1) {
                    block.setPosition((x + j) * cellSize, (y + i) * cellSize);
                    window.draw(block);
                }
            }
        }
    }//move functions
    void moveDown() {
        y++;
    }
    void moveLeft()
    {
        x--;
    }
    void moveRight()
    {
        x++;
    }
    void getPosition(int& posX, int& posY) const {
        posX = x;
        posY = y;
    }
    const int (*getShape() const)[4]
    {
        return shape;
    }
};//sub classes for each shape.(naming according to their name in the manual).
class IPiece : public Piece {
public:
    IPiece() {
        color = sf::Color::Magenta;
        shape[1][0] = 1;
        shape[1][1] = 1;
        shape[1][2] = 1;
        shape[1][3] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];//making a temporary variable naming it current shape,this variable is used for the sub classes of each shape to handle the rotation purpose.
        for (int i = 0; i < 4; ++i) {//loop to copy the same shape into temporary matrix.
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {//loop to clear the matrix so that we can store new values afterwards.
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {//the loop too rotate the shape into 90 degree,hoping this works fine when i run the code.
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};
class JPiece : public Piece {
public:
    JPiece() {
        color = sf::Color::Blue;
        shape[0][0] = 1;//the ones represent that what place of our base class matrix contains the blocks,simply that these boxes constitute the main shape(hard coding :p)
        shape[1][0] = 1;
        shape[1][1] = 1;
        shape[1][2] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];//same logic as above)
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};
class LPiece : public Piece {
public:
    LPiece() {
        color = sf::Color::Cyan;
        shape[0][2] = 1;
        shape[1][0] = 1;
        shape[1][1] = 1;
        shape[1][2] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};
class OPiece : public Piece
{
public:
    OPiece() {
        color = sf::Color::Yellow;
        shape[0][0] = 1;
        shape[0][1] = 1;
        shape[1][0] = 1;
        shape[1][1] = 1;
    }
    void rotate() override {//like rotating it would giv3 the same thing so yeah.
    }
};

class SPiece : public Piece {
public:
    SPiece() {
        color = sf::Color::Green;
        shape[0][1] = 1;
        shape[0][2] = 1;
        shape[1][0] = 1;
        shape[1][1] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};
class TPiece : public Piece {
public:
    TPiece() {
        color = sf::Color(128, 0, 128);
        shape[0][1] = 1;
        shape[1][0] = 1;
        shape[1][1] = 1;
        shape[1][2] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};
class ZPiece : public Piece {
public:
    ZPiece() {
        color = sf::Color::Red;
        shape[0][0] = 1;
        shape[0][1] = 1;
        shape[1][1] = 1;
        shape[1][2] = 1;
    }
    void rotate() override {
        int Crrshape[4][4];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Crrshape[i][j] = shape[i][j];
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                shape[i][j] = Crrshape[3 - j][i];
            }
        }
    }
};

Piece* generateRandomPiece() {
    //this function returns a pointer to the base class
    //returning the shape obtained as the result of the random remainder,each holds a shape from 0-7.
    int random = rand() % 7;
    if (random == 0) {
        return new IPiece();
    }
    else if (random == 1) {
        return new JPiece();
    }
    else if (random == 2) {
        return new LPiece();
    }
    else if (random == 3) {
        return new OPiece();
    }
    else if (random == 4) {
        return new SPiece();
    }
    else if (random == 5) {
        return new TPiece();
    }
    else if (random == 6) {
        return new ZPiece();
    }
    else {
        return new IPiece();
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));//used for srands usage.
    sf::RenderWindow window(sf::VideoMode(300, 600), "Tetris Game");
    const int cellSize = 20;//this is upto us actually,represents the shape size,the 20 pixels seeemed to be a suitable size so i went for it.
    Piece* currentPiece = nullptr;
    float timer = 0;
    float delay = 0.5;//you can change this to decrease or increase the speed of the falling of shape.
    currentPiece = generateRandomPiece();
    int startX = 4;
    int startY = 0;
    currentPiece->getPosition(startX, startY);
    sf::Clock clock;
    //This segment handles time management in game's main loop.
    while (window.isOpen()) {//main loop.
        float time = clock.getElapsedTime().asSeconds();//to get the time since last frame
        clock.restart();//to reset the clock.
        timer += time;//to get the time passed.
        //actually without this the pieces would fall really fast at times,we could have used sleep as well but it causes some sort of flickering issues sso yeah we need to avoid it.
        sf::Event event;//used for input handling like we have used it for key presses detection wagera and window resizing sort of things.
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }//key presses conditions for the current shape(which is given the current shape through the function).
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    currentPiece->moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    currentPiece->moveRight();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    currentPiece->moveDown();
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    currentPiece->rotate();
                }
            }
        }
        if (timer > delay) {//this function was actually added by gpt to keep a constant time for the natural moving down of shape,but its logic is quite understandable comparing the main timer with the declared delay.
            currentPiece->moveDown();
            timer = 0;
        }
        window.clear(sf::Color::Black);//clearing the screen
        currentPiece->draw(window, cellSize);//calling the function to draw the shape on screen finally..
        window.display();//displaying the frames.
    }
    if (currentPiece != nullptr) {
        delete currentPiece;
    }//memory de allocation for the last active piece.
    return 0;
} 