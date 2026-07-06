#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>
using namespace std;

/*
Here, a terminal based shooting game is made.
A is aeroplane/player and the enemies are 'v'
which are shooted by bullets denoted by '|'
When score>=100, bombs denoted by 'o' keep dropping
increasing difficulty of game; which keeps getting faster n harder

The plane is navigated by 'a' and 'd' buttons
denoting 'left' and 'right' directions respectively.
space bar is used to shoot.
The game is never ending and thus can be quited
on pressing 'q'
*/

//Screen size
const int SCREEN_WIDTH = 40;
const int SCREEN_HEIGHT = 20;

//Max enemy grid
const int MAX_ENEMY_ROWS = 3;
const int MAX_ENEMY_COLS = 8;

//Current enemy count
int enemyRows = 2;
int enemyCols = 5;

//Player
int playerPos = SCREEN_WIDTH / 2;

//Score
int score = 0;

//Bullet
bool bulletExists = false;
int bulletX, bulletY;

//Bomb
bool bombExists = false;
int bombX, bombY;
int bombSpeedCounter = 0;

//Enemy grid
bool enemy[MAX_ENEMY_ROWS][MAX_ENEMY_COLS];

//Enemy movement

int moveDirection = 1;
int shiftX = 0;
int shiftY = 0;

int frameCounter = 0;
int enemySpeed = 12;
int gameSpeed = 60000;

struct termios originalSettings;

//terminal control

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &originalSettings);
    struct termios raw = originalSettings;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

//game logic

void createEnemies() {
    for (int i = 0; i < enemyRows; i++)
        for (int j = 0; j < enemyCols; j++)
            enemy[i][j] = true;

    shiftX = 0;
    shiftY = 0;
    moveDirection = 1;
}

int remainingEnemies() {
    int count = 0;
    for (int i = 0; i < enemyRows; i++)
        for (int j = 0; j < enemyCols; j++)
            if (enemy[i][j]) count++;
    return count;
}

void moveEnemies() {

    shiftX += moveDirection;

    for (int i = 0; i < enemyRows; i++) {
        for (int j = 0; j < enemyCols; j++) {

            if (!enemy[i][j]) continue;

            int posX = 5 + j * 4 + shiftX;

            if (posX <= 0 || posX >= SCREEN_WIDTH - 1) {
                moveDirection = -moveDirection;
                shiftY++;
                return;
            }
        }
    }
}

void moveBullet() {

    if (!bulletExists) return;

    bulletY--;

    if (bulletY < 0) {
        bulletExists = false;
        return;
    }

    for (int i = 0; i < enemyRows; i++) {
        for (int j = 0; j < enemyCols; j++) {

            if (!enemy[i][j]) continue;

            int enemyX = 5 + j * 4 + shiftX;
            int enemyY = 2 + i * 2 + shiftY;

            if (bulletX == enemyX && bulletY == enemyY) {
                enemy[i][j] = false;
                bulletExists = false;
                score += 10;
                return;
            }
        }
    }
}

//bomb function

void dropBomb() {

    if (score < 100) return;     
    //bombs unlock after 100 score
    if (bombExists) return;

    for (int i = 0; i < enemyRows; i++) {
        for (int j = 0; j < enemyCols; j++) {

            if (!enemy[i][j]) continue;

            int enemyX = 5 + j * 4 + shiftX;
            int enemyY = 2 + i * 2 + shiftY;

            if (rand() % 20 == 0) {
                bombExists = true;
                bombX = enemyX;
                bombY = enemyY + 1;
                return;
            }
        }
    }
}

void moveBomb() {

    if (!bombExists) return;

    bombSpeedCounter++;

    if (bombSpeedCounter >= 2) {
        bombY++;
        bombSpeedCounter = 0;
    }

    //If bomb hits player
    if (bombY == SCREEN_HEIGHT - 1 && bombX == playerPos) {
        disableRawMode();
        clearScreen();
        cout << "HIT BY BOMB!\n";
        cout << "Final Score: " << score << endl;
        exit(0);
    }

    if (bombY >= SCREEN_HEIGHT)
        bombExists = false;
}


void drawGame() {

    clearScreen();

    for (int y = 0; y < SCREEN_HEIGHT; y++) {

        for (int x = 0; x < SCREEN_WIDTH; x++) {

            if (y == SCREEN_HEIGHT - 1 && x == playerPos)
                cout << "A";

            else if (bulletExists && x == bulletX && y == bulletY)
                cout << "|";

            else if (bombExists && x == bombX && y == bombY)
                cout << "o";

            else {
                bool printed = false;

                for (int i = 0; i < enemyRows; i++) {
                    for (int j = 0; j < enemyCols; j++) {

                        if (!enemy[i][j]) continue;

                        int enemyX = 5 + j * 4 + shiftX;
                        int enemyY = 2 + i * 2 + shiftY;

                        if (x == enemyX && y == enemyY) {
                            cout << "v";
                            printed = true;
                        }
                    }
                }

                if (!printed) cout << " ";
            }
        }
        cout << endl;
    }

    cout << "Score: " << score << endl;

    if (score >= 100)
        cout << "WARNING: Bombs Active!\n";

    cout << "a = left | d = right | space = shoot | q = quit" << endl;
}


int main() {

    srand(time(0));

    clearScreen();
    cout << "\nWELCOME TO TERMINAL SHOOTER\n";
    cout << "\nControls:\n";
    cout << "a -> Move Left\n";
    cout << "d -> Move Right\n";
    cout << "space -> Shoot\n";
    cout << "q -> Quit\n\n";
    cout << "Survive as long as you can!!!\n";
    cout << "\nPress ENTER to start...";
    cin.get();
    
//Countdown
    clearScreen();
    cout << "\n\n       Get Ready!\n\n";
    sleep(1);

    cout << "           3...\n";
    sleep(1);

    clearScreen();
    cout << "\n\n           2...\n";
    sleep(1);

    clearScreen();
    cout << "\n\n           1...\n";
    sleep(1);

    clearScreen();
    cout << "\n\n         START!\n";
    sleep(1);

    enableRawMode();
    createEnemies();

    bool gameRunning = true;

    while (gameRunning) {

        char input;

        if (read(STDIN_FILENO, &input, 1) > 0) {

            if (input == 'a' && playerPos > 0)
                playerPos--;

            else if (input == 'd' && playerPos < SCREEN_WIDTH - 1)
                playerPos++;

            else if (input == ' ' && !bulletExists) {
                bulletExists = true;
                bulletX = playerPos;
                bulletY = SCREEN_HEIGHT - 2;
            }

            else if (input == 'q')
                gameRunning = false;
        }

        frameCounter++;

        if (frameCounter >= enemySpeed) {
            moveEnemies();
            frameCounter = 0;
        }

        moveBullet();
        dropBomb();
        moveBomb();

        drawGame();

        //Check enemy reach bottom
        for (int i = 0; i < enemyRows; i++) {
            for (int j = 0; j < enemyCols; j++) {

                if (!enemy[i][j]) continue;

                int enemyY = 2 + i * 2 + shiftY;

                if (enemyY >= SCREEN_HEIGHT - 1) {
                    disableRawMode();
                    clearScreen();
                    cout << "GAME OVER\n";
                    cout << "Final Score: " << score << endl;
                    return 0;
                }
            }
        }

        //Next level
        if (remainingEnemies() == 0) {

            if (enemyRows < MAX_ENEMY_ROWS)
                enemyRows++;

            if (enemyCols < MAX_ENEMY_COLS)
                enemyCols++;

            if (enemySpeed > 5)
                enemySpeed--;

            createEnemies();
        }

        usleep(gameSpeed);
    }

    disableRawMode();
    clearScreen();
    cout << "Game Ended\n";

    return 0;
}



