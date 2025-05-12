#include <iostream>
#include <fstream>
#include "help.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

const int ROCKET_WIDTH = 40;
const int ROCKET_HEIGHT = 80;

const float BULLET_DIAMETER = 15;
const int MAX_BULLETS = 5;

const int backgroundX1 = 100, backgroundY1 = 100, backgroundX2 = 1555, backgroundY2 = 1000;
const int spaceX = 1405, spaceY = 150;

const int ALIEN_RADIUS = 15;
const int ALIEN_ROWS = 5;
const int ALIEN_COLS = 10;

const int INITIAL_LIVES = 5;

void displayScoreAndLives(int score, int lives, int prevScore, int prevLives) {
	//function to display score and lives

	drawText(15, 1405, 110, 255, 102, 102, "Score");
	if (prevScore != score)
	{
		//draw over previous score with background colour
		drawText(15, 1490, 110, 0, 0, 0, "", true, prevScore);
		prevScore = score;
	}
	drawText(15, 1490, 110, 255, 102, 102, "", true, score); //new score

	drawText(15, 1405, 150, 255, 102, 102, "Lives");
	if (prevLives != lives) {
		//draw over previous lives with background colour
		drawText(15, 1495, 150, 0, 0, 0, "", true, prevLives);
		prevLives = lives;
	}
	drawText(15, 1495, 150, 255, 102, 102, "", true, lives); //new lives
}

void rocketMovement(int& whichKey, int& rocketX, int& rocketY) {
	if (whichKey == 1) { //left
		if (rocketX - 5 > 100) {
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 0, 0, 0);
			rocketX -= 5;
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 255, 0, 127, 255, 193, 204);
		}
	}
	else if (whichKey == 2) { //up
		if ((rocketX + 40 < spaceX && rocketY - 5 > 100)
			|| (rocketX + 40 >= 1405 && rocketY - 5 > spaceY + 25)) {
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 0, 0, 0);
			rocketY -= 5;
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 255, 0, 127, 255, 193, 204);
		}
	}
	else if (whichKey == 3) { //right
		if ((rocketY < spaceY && rocketX + 40 + 5 < spaceX)
			|| (rocketY >= spaceY && rocketX + 40 + 5 < backgroundX2)) {
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 0, 0, 0);
			rocketX += 5;
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 255, 0, 127, 255, 193, 204);
		}
	}
	else if (whichKey == 4) { //down
		if (rocketY + 80 + 5 < backgroundY2) {
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 0, 0, 0);
			rocketY += 5;
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 255, 0, 127, 255, 193, 204);
		}
	}
}

void drawAliens(int aliens[ALIEN_ROWS][ALIEN_COLS], int alienX, int alienY, int prevAlienX, int prevAlienY, float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2])
{
	for (int row = 0; row < ALIEN_ROWS; row++)
	{
		for (int col = 0; col < ALIEN_COLS; col++)
		{
			if (aliens[row][col] == 1)
			{ // Active aliens
				int x = alienX + col * 70;
				int y = alienY + row * 150;

				//clear previous position with background colour
				myEllipse(prevAlienX + col * 70, prevAlienY + row * 150, prevAlienX + col * 70 + ALIEN_RADIUS * 4, prevAlienY + row * 150 + ALIEN_RADIUS * 4, 0, 0, 0);

				// Drawing alien at new position
				myEllipse(x, y, x + ALIEN_RADIUS * 4, y + ALIEN_RADIUS * 4, 168, 228, 160, 168, 228, 160);
				aliensCoordinates[row][col][0] = x + (2* ALIEN_RADIUS);
				aliensCoordinates[row][col][1] = y + (2*ALIEN_RADIUS);
			}
		}
	}
}

bool moveAliens(float& prevAlienX, float& prevAlienY, float& alienX, float& alienY, int& direction, int& steps, int aliens[ALIEN_ROWS][ALIEN_COLS], float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2], int nextCol)
{
	prevAlienX = alienX;
	prevAlienY = alienY;
	float leftX= aliensCoordinates[0][0][0];
	bool flag = false;
	for (int cols = 0; !flag && cols < ALIEN_COLS; cols++) {
		for (int rows = 0; !flag && rows < ALIEN_ROWS; rows++) {
			if (aliens[rows][cols] == 1) {
				nextCol = cols;
				leftX = aliensCoordinates[rows][nextCol][0] - (2*ALIEN_RADIUS);
				flag = true;
			}
		}
	}

	//movement
	if (leftX - 4 > backgroundX1) {
		alienX -= 4;
		steps++;
		alienY -= (2 * direction);
		if (steps == 50) {
			steps = 0;
			direction *= -1;
		}
		return false;
	}
	else {
		return true; //game over
	}
}

bool checkCollision(int bulletX, int bulletY, int aliens[ALIEN_ROWS][ALIEN_COLS], float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2], float alienX, float alienY, float prevAlienX, float prevAlienY, int& score, int& prevScore)
{
	bool returnVal = false;
	for (int row = 0; row < ALIEN_ROWS; row++)
	{
		for (int col = 0; col < ALIEN_COLS; col++)
		{
			if (aliens[row][col] == 1)
			{

				int alienLeft = aliensCoordinates[row][col][0] - (ALIEN_RADIUS * 2);
				int alienRight = aliensCoordinates[row][col][0] + (ALIEN_RADIUS * 2);
				int alienTop = aliensCoordinates[row][col][1] - (ALIEN_RADIUS * 2);
				int alienBottom = aliensCoordinates[row][col][1] + (ALIEN_RADIUS * 2);


				int bulletLeft = bulletX - BULLET_DIAMETER / 2;
				int bulletRight = bulletX + BULLET_DIAMETER / 2;
				int bulletTop = bulletY - BULLET_DIAMETER / 2;
				int bulletBottom = bulletY + BULLET_DIAMETER / 2;


				if (bulletRight >= alienLeft && bulletLeft <= alienRight &&
					bulletBottom >= alienTop && bulletTop <= alienBottom)
				{
					aliens[row][col] = 0; // Alien destroyed BOOM 
					aliensCoordinates[row][col][0] = -1;
					aliensCoordinates[row][col][1] = -1;
					PlaySound(TEXT("destroyed"), NULL, SND_FILENAME | SND_ASYNC);

					myEllipse(prevAlienX + col * 70, prevAlienY + row * 150, prevAlienX + col * 70 + ALIEN_RADIUS * 4, prevAlienY + row * 150 + ALIEN_RADIUS * 4, 0, 0, 0);
					prevScore = score;
					score += 10;
					returnVal = true;
				}
			}
		}
	}
	return returnVal;
}

bool checkRocketAlienCollision(int rocketX, int rocketY, float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2], int aliens[ALIEN_ROWS][ALIEN_COLS])
{
	bool returnVal = false;
	for (int row = 0; row < ALIEN_ROWS; row++)
	{
		for (int col = 0; col < ALIEN_COLS; col++)
		{
			if (aliens[row][col] == 1)
			{

				// Alien ki boundaries
				int alienLeft = aliensCoordinates[row][col][0] - ALIEN_RADIUS * 2;
				int alienRight = aliensCoordinates[row][col][0] + ALIEN_RADIUS * 2;
				int alienTop = aliensCoordinates[row][col][1] - ALIEN_RADIUS * 2;
				int alienBottom = aliensCoordinates[row][col][1] + ALIEN_RADIUS * 2;


				int rocketLeft = rocketX;
				int rocketRight = rocketX + ROCKET_WIDTH;
				int rocketTop = rocketY;
				int rocketBottom = rocketY + ROCKET_HEIGHT;

				// overlap
				if (rocketRight >= alienLeft && rocketLeft <= alienRight &&
					rocketBottom >= alienTop && rocketTop <= alienBottom)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int pauseGame() {
	//draw background
	myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 229, 204, 201);
	//draw game paused
	drawText(50, 300, 300, 255, 102, 102, "PAUSED");
	// 146, 161, 207
	int resumeR = 80, resumeG = 125, resumeB = 42; //option colours
	int quitR = 102, quitG = 66, quitB = 77;
	int choice = 1;
	bool redraw = true;
	while (true) {
		int whichKey;
		bool keyPressed = isKeyPressed(whichKey);
		if (choice == 1 && whichKey == 4) //down key, update quit colours
		{
			resumeR = 102, resumeG = 66, resumeB = 77;
			quitR = 218; quitG = 44; quitB = 67;
			choice = 2;
			redraw = true;
		}
		if (choice == 2 && whichKey == 2) //up key, update resume colours
		{
			resumeR = 80, resumeG = 125, resumeB = 42;
			quitR = 102, quitG = 66, quitB = 77;
			choice = 1;
			redraw = true;
		}

		//draw options
		if (redraw) {
			drawText(30, 300, 450, resumeR, resumeG, resumeB, "RESUME");
			drawText(30, 300, 550, quitR, quitG, quitB, "QUIT");
			redraw = false;
		}

		if (whichKey == 5) { //enter key
			return choice;
		}
	}
}

void gameOverScreen(bool win) {
	bool keyPressed;
	int whichKey;
	bool drawOnceW = false, drawOnceL = false;
	myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 0, 0, 0);
	while (true) {
		if (win) {
			//win screen
			if (!drawOnceW) {
				myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 147, 197, 114);
				drawText(80, 500, 400, 147, 197, 114, "YOU WIN");
				drawText(40, 450, 600, 147, 197, 114, "Press Enter to Quit");
				PlaySound(TEXT("win"), NULL, SND_FILENAME | SND_ASYNC);
				drawOnceW = true;
			}
		}
		else
		{
			//lose screen
			if (!drawOnceL) {
				myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 218, 44, 67);
				drawText(80, 500, 400, 218, 44, 67, "YOU LOSE");
				drawText(40, 450, 600, 218, 44, 67, "Press Enter to Quit");
				PlaySound(TEXT("lose"), NULL, SND_FILENAME | SND_ASYNC);
				drawOnceL = true;
			}
		}

		keyPressed = isKeyPressed(whichKey);
		if (whichKey == 5) {
			system("cls");
			return;
		}
	}
}

void saveGame(int score, int lives, float alienX, float alienY, int direction, int steps, float bulletX[MAX_BULLETS], float bulletY[MAX_BULLETS], int rocketX, int rocketY, int aliens[ALIEN_ROWS][ALIEN_COLS], float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2])
{
	ofstream outFile("game_save.txt");
	//score lives alienX alienY rocketX, rocketY
	outFile << score << " " << lives << " " << alienX << " " << alienY << " " << direction << " " << steps << " " << rocketX << " " << rocketY << endl;

	//store whether or not aliens are active
	for (int row = 0; row < ALIEN_ROWS; row++)
	{
		for (int col = 0; col < ALIEN_COLS; col++)
		{
			outFile << aliens[row][col] << " ";
		}
		outFile << endl;
	}

	//store alien coordinates
	for (int row = 0; row < ALIEN_ROWS; row++) {
		for (int col = 0; col < ALIEN_COLS; col++) {
			for (int i = 0; i < 2; i++) {
				outFile << aliensCoordinates[row][col][i] << " ";
			}
		}
		outFile << endl;
	}

	//store bullets
	for (int i = 0; i < MAX_BULLETS; i++) {
		outFile << bulletX[i] << " ";
	}

	outFile << endl;

	for (int i = 0; i < MAX_BULLETS; i++) {
		outFile << bulletY[i] << " ";
	}

	outFile.close();
}

bool loadGame(int& score, int& lives, float& alienX, float& alienY, int& direction, int& steps, float bulletX[MAX_BULLETS], float bulletY[MAX_BULLETS], 
	int& rocketX, int& rocketY, int aliens[ALIEN_ROWS][ALIEN_COLS], float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2])
{
	ifstream inFile("game_save.txt");
	if (!inFile) {
		return false; //if file empty
	}

	inFile >> score >> lives >> alienX >> alienY >> direction >> steps >>rocketX >> rocketY;

	for (int row = 0; row < ALIEN_ROWS; row++)
	{
		for (int col = 0; col < ALIEN_COLS; col++)
		{
			inFile >> aliens[row][col];
		}
	}

	for (int row = 0; row < ALIEN_ROWS; row++) {
		for (int col = 0; col < ALIEN_COLS; col++) {
			for (int i = 0; i < 2; i++) {
				inFile >> aliensCoordinates[row][col][i];
			}
		}
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		inFile >> bulletX[i];
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		inFile >> bulletY[i];
	}
	inFile.close();
	return true;
}

int menu() {
	int choice = 1;
	int whichKey;
	bool keyPressed;
	bool redraw = true;
	myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 70, 130, 180);
	drawText(70, 230, 200, 255, 102, 102, "Space Invaders");

	int startR = 80, startG = 125, startB = 42; //green
	int resumeR = 102, resumeG = 66, resumeB = 77;
	int highScoreR = 102, highScoreG = 66, highScoreB = 77;
	int instructionsR = 102, instructionsG = 66, instructionsB = 77;
	int quitR = 102, quitG = 66, quitB = 77;

	while (true) {
		whichKey = 0;
		keyPressed = isKeyPressed(whichKey);

		if (whichKey == 5) { //enter
			return choice;
		}

		else if (choice == 1 && whichKey == 4) {
			startR = 102; startG = 66; startB = 77;
			resumeR = 80; resumeG = 125; resumeB = 42;
			choice = 2;
			redraw = true;
		}

		else if (choice == 2 && whichKey == 4) {
			resumeR = 102; resumeG = 66; resumeB = 77;
			highScoreR = 80; highScoreG = 125; highScoreB = 42;
			choice = 3;
			redraw = true;
		}
		else if (choice == 3 && whichKey == 4) {
			highScoreR = 102; highScoreG = 66; highScoreB = 77;
			instructionsR = 146; instructionsG = 161; instructionsB = 207;
			choice = 4;
			redraw = true;
		}
		else if (choice == 4 && whichKey == 4) {
			instructionsR = 102; instructionsG = 66; instructionsB = 77;
			quitR = 218; quitG = 44; quitB = 67;
			choice = 5;
			redraw = true;
		}

		else if (choice == 5 && whichKey == 2) {
			quitR = 102; quitG = 66; quitB = 77;
			instructionsR = 146; instructionsG = 161; instructionsB = 207;
			choice = 4;
			redraw = true;
		}
		else if (choice == 4 && whichKey == 2) {
			instructionsR = 102; instructionsG = 66; instructionsB = 77;
			highScoreR = 80; highScoreG = 125; highScoreB = 42;
			choice = 3;
			redraw = true;
		}

		else if (choice == 3 && whichKey == 2) {
			highScoreR = 102; highScoreG = 66; highScoreB = 77;
			resumeR = 80; resumeG = 125; resumeB = 42;
			choice = 2;
			redraw = true;
		}

		else if (choice == 2 && whichKey == 2) {
			resumeR = 102; resumeG = 66; resumeB = 77;
			startR = 80; startG = 125; startB = 42;
			choice = 1;
			redraw = true;
		}

		if (redraw) {
			drawText(30, 250, 400, startR, startG, startB, "Start New Game");
			drawText(30, 250, 500, resumeR, resumeG, resumeB, "Resume Game");
			drawText(30, 250, 600, highScoreR, highScoreG, highScoreB, "High Scores");
			drawText(30, 250, 700, instructionsR, instructionsG, instructionsB, "Instructions");
			drawText(30, 250, 800, quitR, quitG, quitB, "Quit");
			redraw = false;
		}
	}
}

void displayHighScores(int highScores[5]) {
	ifstream inFile("HighScores.txt");
	bool keyPressed;
	int whichKey;
	bool drawOnce = false;

	if (!inFile) {
		return;
	}
	else {
		for (int i = 0; i < 5; i++) {
			inFile >> highScores[i];
		}
		inFile.close();

		while (true) {
			if (!drawOnce) {
				myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 141, 182, 0);
				drawText(60, 500, 200, 141, 182, 0, "High Scores");

				drawText(30, 750, 400, 141, 182, 0, "", true, highScores[0]);
				drawText(30, 750, 480, 141, 182, 0, "", true, highScores[1]);
				drawText(30, 750, 560, 141, 182, 0, "", true, highScores[2]);
				drawText(30, 750, 640, 141, 182, 0, "", true, highScores[3]);
				drawText(30, 750, 720, 141, 182, 0, "", true, highScores[4]);

				drawText(20, 500, 900, 255, 255, 255, "Press Enter To Return To Menu");
				drawOnce = true;
			}
			keyPressed = isKeyPressed(whichKey);
			if (whichKey == 5) {
				return;
			}
		}
	}
}

void addScore(int highScores[5], int newScore) {
	int scores[6];
	int temp;

	for (int i = 0; i < 5; i++) {
		scores[i] = highScores[i];
	}
	scores[5] = newScore;
	//copy high scores array and new score to new array
	
	//bubble sort
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5 - i; j++) {
			if (scores[j] < scores[j + 1]) {
				temp = scores[j];
				scores[j] = scores[j + 1];
				scores[j + 1] = temp;
			}
		}
	}

	//write first 5 scores to file
	ofstream outFile("HighScores.txt");
	for (int i = 0; i < 5; i++) {
		outFile << scores[i] << " ";
	}
	outFile.close();
}

void showInstructions() {
	bool drawOnce = false;
	bool keyPressed;
	int whichKey;

	while (true) {
		keyPressed = isKeyPressed(whichKey);
		if (whichKey == 5) {
			return;
		}
		if (!drawOnce) {
			myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 229, 204, 201);
			drawText(60, 470, 200, 255, 102, 102, "Instructions");
			drawText(20, 520, 400, 255, 102, 102, "Use arrow keys to move rocket");
			drawText(20, 520, 500, 255, 102, 102, "Use space bar to shoot bullets");
			drawText(20, 520, 600, 255, 102, 102, "Press Esc to pause game");
			drawText(20, 520, 700, 255, 102, 102, "Use arrow keys to navigate options");
			drawText(20, 520, 800, 255, 102, 102, "Use Enter key to select");
			drawText(15, 520, 910, 141, 182, 0, "Press Enter to return to Menu");
			drawOnce = true;
		}
	}
}

int main() {
	bool keyPressed;
	//checks if key is pressed
	bool gameOver = false;
	//checks if game over
	bool paused = false;
	//pause game
	bool reachedLeftSide = false;
	int whichKey;
	//stores the key pressed as a number
	int score = 0; int lives = 5; int prevScore = score; int prevLives = lives;
	//score and lives, also variables to store previous values

	int rocketX = 300; int rocketY = 500;
	//initial rocket values

	float bulletX[MAX_BULLETS] = { -1, -1, -1, -1, -1 };  // Array to hold up to 5 bullets
	float bulletY[MAX_BULLETS] = { -1, -1, -1, -1, -1 };

	int aliens[ALIEN_ROWS][ALIEN_COLS];
	//array to check if alien has been destroyed

	float aliensCoordinates[ALIEN_ROWS][ALIEN_COLS][2];
	//array to store centre coordinates of each alien for movement

	float alienX = 850, alienY = 250, prevAlienX = 850, prevAlienY = 250;
	//current and previous alien coordinates
	int alienDirection = 1, alienSteps = 0;
	//to change the vertical direction of aliens
	int nextCol = 0;
	//to store the leftmost column (that contains at least one active alien)
	int iterations = 0;
	//reduce flickering of aliens, move after certain number of iterations

	int highScores[5] = { 0,0,0,0,0 };

	for (int i = 0; i < ALIEN_ROWS; i++) {
		for (int j = 0; j < ALIEN_COLS; j++) {
			aliens[i][j] = 1;
		}
	}
	//all aliens active

	Sleep(1000); //1 second to make full screen

	PlaySound(TEXT("espresso.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	int choice = 3; 
	while (choice == 3 || choice == 4) {
		choice = menu();

		if (choice == 2) { //resume
			loadGame(score, lives, alienX, alienY, alienDirection, alienSteps, bulletX, bulletY, rocketX, rocketY, aliens, aliensCoordinates);
		}
		if (choice == 3) {
			displayHighScores(highScores);
		}
		if (choice == 4) {
			showInstructions();
		}
		if (choice == 5) { //quit
			system("cls");
			gameOver = true;
		}
	}

	if (choice != 5) {
		myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 146, 161, 207); //background- draw once
		drawAliens(aliens, alienX, alienY, prevAlienX, prevAlienY, aliensCoordinates);
	}
	PlaySound(0, 0, 0);
	while (!gameOver) { //game loop- infinite
		whichKey = 0;
		myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 255, 193, 204, 255, 193, 204); //draw rocket
		displayScoreAndLives(score, lives, prevScore, prevLives); //draw score and lives

		if (iterations++ % 80 == 0) {
			drawAliens(aliens, alienX, alienY, prevAlienX, prevAlienY, aliensCoordinates);
			//draw and erase aliens
			reachedLeftSide = moveAliens(prevAlienX, prevAlienY, alienX, alienY, alienDirection, alienSteps, aliens, aliensCoordinates, nextCol);
			//move aliens, checks if aliens have reached left side (lose condition)
		}


		//bullet movement
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bulletX[i] != -1 && bulletY[i] != -1) {
				//if bullet exists

				//draw over previous bullet with background colour
				myEllipse(bulletX[i], bulletY[i], bulletX[i] + BULLET_DIAMETER, bulletY[i] + BULLET_DIAMETER, 0, 0, 0);

				if ((bulletY[i] > 150 && bulletX[i] + BULLET_DIAMETER + 1.75 < backgroundX2)
					|| (bulletY[i] <= 150 && bulletX[i] + BULLET_DIAMETER + 1.75 < 1405)) {
					//check bullet not reached boundaries, 1.75 to prevent eating border

					bulletX[i] += 1.2; //move right

					//draw new bullet
					myEllipse(bulletX[i], bulletY[i], bulletX[i] + BULLET_DIAMETER, bulletY[i] + BULLET_DIAMETER, 255, 102, 102, 255, 102, 102);

					if (checkCollision(bulletX[i], bulletY[i], aliens, aliensCoordinates, alienX, alienY, prevAlienX, prevAlienY, score, prevScore)) { //check if collision with alien
						//draw over bullet with background colour
						myEllipse(bulletX[i], bulletY[i], bulletX[i] + BULLET_DIAMETER, bulletY[i] + BULLET_DIAMETER, 0, 0, 0);
						bulletX[i] = -1;
						bulletY[i] = -1;
					}
				}
				else {
					bulletX[i] = -1;
					bulletY[i] = -1;
				}
			}
		}

		keyPressed = isKeyPressed(whichKey); //stores key pressed

		if (keyPressed)
		{
			if (whichKey >= 1 && whichKey <= 4) {
				rocketMovement(whichKey, rocketX, rocketY);
			}

			else if (whichKey == 8) { //space
				//shoot bullet- start position
				PlaySound(TEXT("bullet.wav"), NULL, SND_FILENAME | SND_ASYNC);
				for (int i = 0; i < MAX_BULLETS; i++) {
					if (bulletX[i] == -1)
					{
						bulletX[i] = rocketX + 12;
						bulletY[i] = rocketY + (ROCKET_HEIGHT / 2); //shoot from middle of rocket
						break;
					}
				}
			}

			if (whichKey == 7) { //escape key, pause game
				paused = true;
				while (paused) {
					int choice = pauseGame();
					if (choice == 1) { //resume
						paused = false;
						myRect(backgroundX1, backgroundY1, backgroundX2, backgroundY2, 146, 161, 207);
						//draw background again
					}
					else if (choice == 2) { //save and quit
						gameOver = true;
						addScore(highScores, score);
						saveGame(score, lives, alienX, alienY, alienDirection, alienSteps, bulletX, bulletY, rocketX, rocketY, aliens, aliensCoordinates);
						system("cls");
						break;
					}
				}
			}
		}

		//alien touch rocket
		if (checkRocketAlienCollision(rocketX, rocketY, aliensCoordinates, aliens))
		{
			prevLives = lives;
			lives--;
			PlaySound(TEXT("lose_life.wav"), NULL, SND_FILENAME | SND_ASYNC);
			myRect(rocketX, rocketY, rocketX + ROCKET_WIDTH, rocketY + ROCKET_HEIGHT, 0, 0, 0);
			rocketX = 200;
			rocketY = backgroundY2 / 2;
		}

		// Check all aliens destroyed
		bool allAliensDestroyed = true;
		for (int row = 0; row < ALIEN_ROWS; row++)
		{
			for (int col = 0; col < ALIEN_COLS; col++)
			{
				if (aliens[row][col] == 1)
				{
					allAliensDestroyed = false;
					break;
				}
			}
			if (!allAliensDestroyed) break;
		}

		//check lose condition
		if (lives == 0 || reachedLeftSide) {
			//lose screen
			gameOver = true;
			gameOverScreen(false);
		}

		//check win condition
		if (allAliensDestroyed) {
			//win screen
			gameOver = true;
			gameOverScreen(true);
		}
		addScore(highScores, score);
	}
	return 0;
}
