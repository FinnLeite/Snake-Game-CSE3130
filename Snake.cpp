#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail = 0;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

class GameObject {
public:
    virtual void draw() = 0; // Pure virtual function for drawing
};

class Snake : public GameObject {
public:
    void draw() override {
        cout << "O"; // Snake head
    }
};

class Fruit : public GameObject {
public:
    void draw() override {
        cout << "F"; // Fruit
    }
};

class Setup {
public:
    void initialize() {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
    }
};

class Draw {
public:
    void render(Snake& snake, Fruit& fruit) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
        
        // Draw the borders
        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << endl;

        // Draw the game area
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0)
                    cout << "#"; // Left border
                if (i == y && j == x)
                    snake.draw();
                else if (i == fruitY && j == fruitX)
                    fruit.draw();
                else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o"; // Snake tail
                            print = true;
                        }
                    }
                    if (!print)
                        cout << " ";
                }
                if (j == width - 1)
                    cout << "#"; // Right border
            }
            cout << endl;
        }

        // Draw the bottom border
        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << endl;
        cout << "Score: " << score << endl;
    }
};

class Input {
public:
    void getInput() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'w': if (dir != DOWN) dir = UP; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'x': gameOver = true; break; // End the game
            }
        }
    }
};

class Logic {
public:
    void process() {
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
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UP: y--; break;
            case DOWN: y++; break;
            default: break;
        }

        // Check for collisions
        if (x >= width || x < 0 || y >= height || y < 0) {
            gameOver = true;
        }
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameOver = true;
            }
        }

        // Check if the snake eats the fruit
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;
        }
    }
};

int main() {
    Setup setup;
    Draw draw;
    Input input;
    Logic logic;

    setup.initialize();
    Snake snake;
    Fruit fruit;

    while (!gameOver) {
        system("cls"); // Clear the console
        draw.render(snake, fruit);
        input.getInput();
        logic.process();
        Sleep(100); // Control the speed of the game
    }

    system("pause");
    return 0;
}
