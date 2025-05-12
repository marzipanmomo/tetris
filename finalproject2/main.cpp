#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>//used this library for srand function
using namespace std;

//global variables or constants

//this represents the shape size (30 pixels)
const int cellSize = 30;
//board
const int columns = 10;
const int rows = 20;
const int backgroundStartX = 120;
const int backgroundStartY = 60;
sf::Color gameBoard[rows][columns];

// initializing the grid
int currentScore = 0;
int previousScore = 0;

//create window, user makes full screen to play, closes to end program
sf::Event event;
sf::RenderWindow window(sf::VideoMode({ 1000, 700 }), "tetris", sf::Style::Default);
sf::RectangleShape background(sf::Vector2f(300.f, 600.f)), grid(sf::Vector2f(cellSize - 1, cellSize - 1));
sf::Font font;
sf::Text tetris, play, highscores, instructions; //menu text objects
sf::Text paused, resume, quit; //pause screen text objects
sf::Text controlKeys; //instructions screen text object
sf::Text score, scoreVal; //current score text object
sf::Text highscoresList; //high scores screen text object
sf::Text gameover;

//function and class prototypes
class Tetromino;
class O;
class I;
class S;
class Z;
class L;
class J;
class T;

//base class
class Tetromino {
protected:
    int shape[4][4];//4x4 matrix for each shapes representation.
    sf::Color color;
    int x, y;//coordinates of the shapes.
public:
    Tetromino();
    virtual ~Tetromino();
    virtual void rotate() = 0;
    bool movable(int newX, int newY);
    bool isOccupied(int x, int y);
    virtual int getX();
    virtual int getY();
    void draw(sf::RenderWindow&, int);
    sf::Color getColor() const;

    //move functions
    void moveDown();
    void moveLeft();
    void moveRight();
    void setPosition(int posX, int posY);
    void getPosition(int& posX, int& posY) const;
    const int (*getShape() const)[4];
};
//derived classes
class O :public Tetromino {
public:
    O();
    void rotate() override;
};
class I :public Tetromino {
public:
    I();
    void rotate() override;
};
class S :public Tetromino {
public:
    S();
    void rotate() override;
};
class Z :public Tetromino {
public:
    Z();
    void rotate() override;
};
class L :public Tetromino {
public:
    L();
    void rotate();
};
class J :public Tetromino {
public:
    J();
    void rotate() override;
};
class T :public Tetromino {
public:
    T();
    void rotate() override;
};

//functions
Tetromino* generateRandomPiece();
void initialiseGraphicsObjects();
int menu();
void pause();
void changeScore();
void displayHighScores();
void addScore();
void displayInstructions();
bool valid_position(Tetromino&);
void holdPiece(Tetromino&);
void gameOver();

int main() {
    srand(static_cast<unsigned>(time(nullptr)));//used for srands usage.

    //initialising text and background settings
    initialiseGraphicsObjects();

    //synchronise frame rate with vertical frequency of monitor
    window.setVerticalSyncEnabled(true);

    Tetromino* currentPiece = nullptr;
    float timer = 0;
    float delay = 0.5; //you can change this to decrease or increase the speed of the falling of shape.

    currentPiece = generateRandomPiece();
    int startX = 4;
    int startY = 1;
    currentPiece->setPosition(startX, startY);
    currentPiece->getPosition(startX, startY);
    sf::Clock clock;
    //This segment handles time management in game's main loop.

    // Initialize gameBoard with grid colour
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            gameBoard[i][j] = sf::Color(25, 25, 112);
        }
    }

    //menu loop
    int choice = 3;
    while (choice == 2 || choice == 3) {
        //if you view highscores or instructions, it will display menu again, if you press play it will exit menu loop
        choice = menu();

        if (choice == 2) { //highscores
            //view highscores
        }
        if (choice == 3) { //instructions
            displayInstructions();
        }
    }

    //game loop
    while (window.isOpen()){
        //to stop pieces falling too fast and prevent flickering
        float time = clock.getElapsedTime().asSeconds();//to get the time since last frame
        clock.restart();//to reset the clock.
        timer += time;//to get the time passed.
        
        //redrawing background
        window.clear(sf::Color::Black);//clearing the screen
        window.draw(background);//draw the background
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                grid.setPosition(backgroundStartX + (j * cellSize), backgroundStartY + (i * cellSize));
                window.draw(grid);
            }
        }
        currentPiece->draw(window, cellSize);
        window.draw(score);
        window.draw(scoreVal);
        window.display();

        //update high score
        changeScore();

        while (window.pollEvent(event)){
            // modified it a bit so it checks for collision first then moves

            if (event.type == sf::Event::KeyPressed) {
                // first checking if movement is valid then we can move according to the key pressed
                if (event.key.code == sf::Keyboard::Left) {
                    if (currentPiece->movable(currentPiece->getX() - 1, currentPiece->getY())) {
                        currentPiece->moveLeft();
                    }
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if (currentPiece->movable(currentPiece->getX() + 1, currentPiece->getY())) {
                        currentPiece->moveRight();
                    }
                }
                if (event.key.code == sf::Keyboard::Down) {
                    if (currentPiece->movable(currentPiece->getX(), currentPiece->getY() + 1)) {
                        currentPiece->moveDown();
                    }
                }
                if (event.key.code == sf::Keyboard::Up) {
                    currentPiece->rotate();
                    if (valid_position(*currentPiece)) {}
                    else {}
                }

                //pause screen
                if (event.key.code == sf::Keyboard::Escape) {
                    pause();
                }
            }

            else if (event.type == sf::Event::Closed) {
                gameOver();
                window.close();
            }

            //downwards movement of tetrominoes
            if (timer > delay) {
                //this function helps to keep a constant time for the natural moving down of shape,
                //it works by comparing the main timer with the declared delay.
                if (currentPiece->movable(currentPiece->getX(), currentPiece->getY() + 1)) {
                    currentPiece->moveDown();
                }
                else {
                    holdPiece(*currentPiece);
                    // Clear completed lines after placing the piece
                    for (int i = rows - 1; i >= 0; --i) {
                        bool fullLine = true;
                        for (int j = 0; j < columns; ++j) {
                            if (gameBoard[i][j] == sf::Color::Black) {
                                fullLine = false;
                                break;
                            }
                        }
                        if (fullLine) {
                            // Move all rows above down by one
                            for (int k = i; k > 0; --k) {
                                for (int j = 0; j < columns; ++j) {
                                    gameBoard[k][j] = gameBoard[k - 1][j];
                                }
                            }
                            // Clear the top row
                            for (int j = 0; j < columns; ++j) {
                                gameBoard[0][j] = sf::Color::Black;
                            }
                            i++; // Recheck the same row after shifting
                            currentScore += 100;
                        }
                    }

                    //de-allocation of memory
                    delete currentPiece;

                    // spawning new piece at top center
                    currentPiece = generateRandomPiece();
                    currentPiece->setPosition(columns / 2 - 2, 0);

                    if (!valid_position(*currentPiece)) {
                        window.clear();
                        window.draw(background);
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                grid.setPosition(backgroundStartX + (j * cellSize), backgroundStartY + (i * cellSize));
                                window.draw(grid);
                            }
                        }
                        window.display();
                        // game over
                        window.close(); 
                    }
                }
                timer = 0;
            }
        }
    }
    return 0;
}

//functions
Tetromino* generateRandomPiece() {
    //this function returns a pointer to the base class
    //returning the shape obtained as the result of the random remainder,each holds a shape from 0-7.
    int random = rand() % 7;
    if (random == 0) {
        return new I();
    }
    else if (random == 1) {
        return new J();
    }
    else if (random == 2) {
        return new L();
    }
    else if (random == 3) {
        return new O();
    }
    else if (random == 4) {
        return new S();
    }
    else if (random == 5) {
        return new T();
    }
    else if (random == 6) {
        return new Z();
    }
    else {
        return new I();
    }
}
void initialiseGraphicsObjects() {
    //background
    background.setOutlineColor(sf::Color(176, 224, 230));
    background.setOutlineColor(sf::Color::Cyan);
    background.setOutlineThickness(5.f);
    background.setFillColor(sf::Color::Black);
    background.setPosition(backgroundStartX, backgroundStartY);
    grid.setFillColor(sf::Color(25, 25, 112));
    grid.setOutlineThickness(1.f);
    grid.setOutlineColor(sf::Color::Black);

    //these settings wont change
    try {
        if (!font.loadFromFile("Pixellettersfull.ttf")) {
            throw 420;
        }
    }
    catch (int fileNotFound) {
        cout << "font file not found" << endl;
    }
    tetris.setFont(font);
    tetris.setString("Tetris");
    tetris.setCharacterSize(60);
    tetris.setFillColor(sf::Color::White);
    tetris.setPosition(400, 100);

    play.setFont(font);
    play.setString("Play");
    play.setCharacterSize(45);
    play.setPosition(400, 200);

    highscores.setFont(font);
    highscores.setString("High Scores");
    highscores.setCharacterSize(45);
    highscores.setPosition(400, 300);

    instructions.setFont(font);
    instructions.setString("Instructions");
    instructions.setCharacterSize(45);
    instructions.setPosition(400, 400);

    paused.setFont(font);
    paused.setString("Paused");
    paused.setCharacterSize(60);
    paused.setFillColor(sf::Color::White);
    paused.setPosition(400, 100);

    resume.setFont(font);
    resume.setString("Resume");
    resume.setCharacterSize(45);
    resume.setPosition(400, 250);

    quit.setFont(font);
    quit.setString("Quit");
    quit.setCharacterSize(45);
    quit.setPosition(400, 350);

    controlKeys.setFont(font);
    controlKeys.setString("Left arrow - Move left\nRight arrow - Move right\nUp arrow - Rotate right\nZ - Rotate left\nDown arrow - Soft drop\nSpace - Hard drop\nEsc - Pause\n\nPress Enter to continue");
    controlKeys.setCharacterSize(35);
    controlKeys.setFillColor(sf::Color::White);
    controlKeys.setPosition(350, 150);
    controlKeys.setLineSpacing(1.2);

    gameover.setFont(font);
    gameover.setString("GAMEOVER");
    gameover.setCharacterSize(45);
    gameover.setPosition(400, 300);
    gameover.setFillColor(sf::Color::Red);

    score.setFont(font);
    score.setString("Score");
    score.setCharacterSize(30);
    score.setPosition(650, 300);
    score.setFillColor(sf::Color::White);

    scoreVal.setFont(font);
    scoreVal.setString("0");
    scoreVal.setCharacterSize(30);
    scoreVal.setPosition(650, 350);
    scoreVal.setFillColor(sf::Color::White);
}
int menu() {
    int choice = 1;
    //initial colours, reset every time menu called
    play.setFillColor(sf::Color::Cyan);
    highscores.setFillColor(sf::Color::White);
    instructions.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) { //enter
                    return choice;
                }

                else if (choice == 1 && event.key.code == sf::Keyboard::Down) { //play -> highscores
                    play.setFillColor(sf::Color::White);
                    highscores.setFillColor(sf::Color::Cyan);
                    choice = 2;
                }

                else if (choice == 2 && event.key.code == sf::Keyboard::Down) { //highscores -> instructions
                    highscores.setFillColor(sf::Color::White);
                    instructions.setFillColor(sf::Color::Cyan);
                    choice = 3;
                }
                else if (choice == 3 && event.key.code == sf::Keyboard::Up) { //instructions -> highscores
                    instructions.setFillColor(sf::Color::White);
                    highscores.setFillColor(sf::Color::Cyan);
                    choice = 2;
                }

                else if (choice == 2 && event.key.code == sf::Keyboard::Up) { //highscores -> play
                    highscores.setFillColor(sf::Color::White);
                    play.setFillColor(sf::Color::Cyan);
                    choice = 1;
                }
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
            window.clear();
            window.draw(tetris);
            window.draw(play);
            window.draw(highscores);
            window.draw(instructions);
            window.display();
        }
    }
}
void pause() {
    int choice = 1;
    //initial colours
    resume.setFillColor(sf::Color::Cyan);
    quit.setFillColor(sf::Color::White);
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (choice == 1 && event.key.code == sf::Keyboard::Down) { //resume -> quit
                    resume.setFillColor(sf::Color::White);
                    quit.setFillColor(sf::Color::Cyan);
                    choice = 2;
                }
                if (choice == 2 && event.key.code == sf::Keyboard::Up) {
                    quit.setFillColor(sf::Color::White);
                    resume.setFillColor(sf::Color::Cyan);
                    choice = 1;
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (choice == 1) {
                        return;
                    }
                    else if (choice == 2) {
                        gameOver();
                        window.close();
                    }
                }
                else if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(paused);
        window.draw(resume);
        window.draw(quit);
        window.display();
    }
}
void displayInstructions() {
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    return;
                }
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(controlKeys);
        window.display();
    }
}
bool valid_position(Tetromino& piece) {
    //checking if current piece position is valid
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piece.getShape()[i][j]) {
                int boardX = piece.getX() + j;
                int boardY = piece.getY() + i;

                // checking for collision with boundaries
                if (boardX < 0 || boardX >= columns || boardY >= rows) {
                    return false;
                }

                // Checking for collision with alr present  pieces 
                if (boardY >= 0 && gameBoard[boardY][boardX] != sf::Color::Black) {
                    return false;
                }
            }
        }
    }
    return true;
}
void holdPiece(Tetromino& piece) {
    // holding the piece there once it gets to the bottom most position or if there's a collision
    int x, y;
    piece.getPosition(x, y);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            const int (*shape)[4] = piece.getShape();
            if (shape[i][j]) {
                gameBoard[y + i][x + j] = piece.getColor(); // Set the color of the block
            }
        }
    }
}
void gameOver() {
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    window.close();
                }
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(gameover);
        window.display();
    }
}
void changeScore() {
    if (previousScore != currentScore) {
        previousScore = currentScore;
        scoreVal.setString(to_string(currentScore));
    }
}

//member functions
//base class
Tetromino::Tetromino() : x(0), y(0), color(sf::Color::Green) {  // Or any color
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            shape[i][j] = 0;
        }
    }
}

Tetromino::~Tetromino() {}
void Tetromino::draw(sf::RenderWindow& window, int cellSize) {
    sf::RectangleShape block(sf::Vector2f(cellSize - 1, cellSize - 1));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (gameBoard[i][j] != sf::Color(25, 25, 112)) {
                    grid.setPosition(backgroundStartX + (j * cellSize),
                        backgroundStartY + (i * cellSize));

                    grid.setFillColor(gameBoard[i][j]);
                    window.draw(grid);
                }
            }
        }

        block.setFillColor(color);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) { 
                if (shape[i][j] == 1) {
                    int drawX = backgroundStartX + (x + j) * cellSize;
                    int drawY = backgroundStartY + (y + i) * cellSize;

                    if (drawX >= 0 && drawX < window.getSize().x && drawY >= 0 && drawY < window.getSize().y) {
                        block.setPosition(drawX, drawY);
                        window.draw(block);
                    }
                }
            }
        }
}

//move functions
void Tetromino::moveRight() {
    if (movable(x + 1, y))
        x++;
}
void Tetromino::moveLeft() {
    if (movable(x - 1, y))
        x--;
}
void Tetromino::moveDown() {
    if (movable(x, y + 1))
        y++;
}

void Tetromino::setPosition(int posX, int posY) {
    x = posX;
    y = posY;
}
void Tetromino::getPosition(int& posX, int& posY) const {
    posX = x;
    posY = y;
}
const int (*Tetromino::getShape() const)[4] {
    return shape;
}
bool Tetromino::isOccupied(int x, int y) {
    // checking whether there's alr a piece at the position (x, y)

    return gameBoard[y][x] != sf::Color::Black;

}
bool Tetromino::movable(int newX, int newY) {
    // function to check whether movement is possible without collision with the walls
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                int boardX = newX + j;
                int boardY = newY + i;

                // check collision with boundaries
                if (boardX < 0 || boardX >= columns || boardY >= rows || boardY < 0) {
                    return false;
                }

                // Check collision with alr present pieces
                if (boardY >= 0 && gameBoard[boardY][boardX] == sf::Color::Cyan) {
                    return false;
                }
            }
        }
    }
    return true;
}
sf::Color Tetromino::getColor() const {
    return color;
}
int Tetromino::getX(){
    return x;
}
int Tetromino::getY(){
    return y;
}

//derived classes
O::O() {
    color = sf::Color::Yellow;
    shape[0][0] = 1;
    shape[0][1] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
}
void O::rotate() {
    //rotating this shape would not make a difference
}

I::I() {
    color = sf::Color::Magenta;
    shape[1][0] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
    shape[1][3] = 1;
}
void I::rotate() {
    //making a temporary variable naming it current shape,this variable is used for the sub classes of each shape to handle the rotation purpose.
    int Crrshape[4][4];

    //loop to copy the same shape into temporary matrix.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            Crrshape[i][j] = shape[i][j];
        }
    }

    //loop to clear the matrix so that we can store new values afterwards.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            shape[i][j] = 0;
        }
    }

    //the loop too rotate the shape into 90 degree,hoping this works fine when i run the code.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            shape[i][j] = Crrshape[3 - j][i];
        }
    }
}

S::S() {
    color = sf::Color::Green;
    shape[0][1] = 1;
    shape[0][2] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
}
void S::rotate() {
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

Z::Z() {
    color = sf::Color::Red;
    shape[0][0] = 1;
    shape[0][1] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
}
void Z::rotate() {
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

L::L() {
    color = sf::Color::Cyan;
    shape[0][2] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
}
void L::rotate() {
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

J::J() {
    color = sf::Color::Blue;
    //the ones represent that what place of our base class matrix contains the blocks,simply that these boxes constitute the main shape(hard coding :p)
    shape[0][0] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
}
void J::rotate() {
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

T::T() {
    color = sf::Color(128, 0, 128);
    shape[0][1] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
}
void T::rotate() {
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