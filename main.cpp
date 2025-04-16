#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 17;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int fruitColor, snakeColor;
int numObstacles = 2;
int obstacleX[100], obstacleY[100];
bool speedBoostActive = false;
int speedBoostX, speedBoostY;
DWORD speedBoostEndTime = 0;
bool ghostModeActive = false;
int ghostModeX, ghostModeY;
DWORD ghostModeEndTime = 0;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    fruitColor = 1 + rand() % 6;
    snakeColor = 1 + rand() % 6;
    score = 0;
    speedBoostX = rand() % width;
    speedBoostY = rand() % height;
    ghostModeX = rand() % width;
    ghostModeY = rand() % height;
    
    for (int i = 0; i < numObstacles; i++) {
        obstacleX[i] = rand() % width;
        obstacleY[i] = rand() % height;
    }
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Draw() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x) {
                SetColor(snakeColor);
                cout << "O";
                SetColor(7);
            } else if (i == fruitY && j == fruitX) {
                SetColor(fruitColor);
                cout << "F";
                SetColor(7);
            } else if (i == speedBoostY && j == speedBoostX) {
                SetColor(3);
                cout << "B";
                SetColor(7);
            } else if (i == ghostModeY && j == ghostModeX) {
                SetColor(5);
                cout << "G";
                SetColor(7);
            } else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(snakeColor);
                        cout << "o";
                        SetColor(7);
                        print = true;
                    }
                }
                for (int k = 0; k < numObstacles; k++) {
                    if (obstacleX[k] == j && obstacleY[k] == i) {
                        SetColor(4);
                        cout << "#";
                        SetColor(7);
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    if (!ghostModeActive) {
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;
    }
    
    for (int i = 0; i < numObstacles; i++)
        if (x == obstacleX[i] && y == obstacleY[i])
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += speedBoostActive ? 20 : 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        fruitColor = 1 + rand() % 6;
        snakeColor = 1 + rand() % 6;
        nTail++;
    }
    if (x == speedBoostX && y == speedBoostY) {
        speedBoostActive = true;
        speedBoostEndTime = GetTickCount() + 10000;
        speedBoostX = rand() % width;
        speedBoostY = rand() % height;
    }
    if (x == ghostModeX && y == ghostModeY) {
        ghostModeActive = true;
        ghostModeEndTime = GetTickCount() + 10000;
        ghostModeX = rand() % width;
        ghostModeY = rand() % height;
    }
    if (speedBoostActive && GetTickCount() > speedBoostEndTime)
        speedBoostActive = false;
    if (ghostModeActive && GetTickCount() > ghostModeEndTime)
        ghostModeActive = false;
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speedBoostActive ? 50 : 100);
    }
    return 0;
}
