#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

struct coordinates
{
private:
    int HEIGHT_BOARD = 20, WIDTH_BOARD = 40;
public:
    int x, y;
    void printCoordinates()
    {
        std::cout << x << ", " << y << std::endl;
    }
    int getHeightBoard()
    {
        return HEIGHT_BOARD;
    }
    int getWidthBoard()
    {
        return WIDTH_BOARD;
    }
    coordinates getRandomCoord()
    {
        coordinates num;
        num.x = 1 + (std::rand() % ( (HEIGHT_BOARD - 2) - 1 + 1));
        num.y = 1 + (std::rand() % ( (WIDTH_BOARD - 2) - 1 + 1));
        return num;
    }
    coordinates add(const coordinates& other) const
    {
        return coordinates(x + other.x, y + other.y);
    }
    coordinates operator+(const coordinates& other) const
    {
        return add(other);
    }
    bool equalsWithNumbers(coordinates other, int i, int k) const
    {
        return other.x == i && other.y == k;
    }
    bool OR(coordinates other, int i, int k) const
    {
        return other.x == i || other.y == k;
    }
    bool equals(const coordinates& other) const
    {
        return other.x == x && other.y == y;
    }
    bool operator==(const coordinates& other) const
    {
        return equals(other);
    }
public:
    coordinates(int x, int y)
        : x(x), y(y)
    {}
    coordinates()
    {}
};

class Snake : coordinates
{
private:
    enum DIRECTIONS { UP, DOWN, LEFT, RIGHT };
    const coordinates direction[4] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    const coordinates rDirection[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    std::vector<coordinates> snakeBody;
public:
    void init_body()
    {
        snakeBody.push_back(getRandomCoord());
        addSectorBody();
    }
public:
    int setDirection(char inputDirection)
    {
        switch (inputDirection)
        {
        case 'w':
            return UP;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
        case 'd':
            return RIGHT;
        default:
            break;
        }
    }
    void takeStep(bool growth)
    {
        char inputDirection;
        inputDirection = _getch();
        switch (setDirection(inputDirection))
        {
        case UP:
            snakeBody.insert(snakeBody.begin(), snakeBody.front() + direction[UP]);
            if(!growth)
                snakeBody.pop_back();
            break;
        case DOWN:
            snakeBody.insert(snakeBody.begin(), snakeBody.front() + direction[DOWN]);
            if (!growth)
                snakeBody.pop_back();
            break;
        case LEFT:
            snakeBody.insert(snakeBody.begin(), snakeBody.front() + direction[LEFT]);
            if (!growth)
                snakeBody.pop_back();
            break;
        case RIGHT:
            snakeBody.insert(snakeBody.begin(), snakeBody.front() + direction[RIGHT]);
            if (!growth)
                snakeBody.pop_back();
            break;
        default:
            break;
        }
        
    }
    void addSectorBody()
    {
        snakeBody.push_back(snakeBody.back() + rDirection[UP]);
    }
    std::vector<coordinates> getSnakeBody()
    {
        return snakeBody;
    }
    coordinates getSnakeHead()
    {
        return snakeBody.front();
    }
public:
    Snake()
    {
        srand(time(NULL));
        init_body();
    }
};

class Apple : coordinates
{
private:
    coordinates apple;
public:
    coordinates getAppleCoordinates()
    {
        return apple;
    }
    void setNewApple()
    {
        apple = getRandomCoord();
    }
public:
    Apple()
    {
        setNewApple();
    }
};

class Game : coordinates
{
private:
    char** board;
    Snake* snake;
    Apple* apple;
    int score;
private:
    void board_matrix()
    {
        board = new char*[getHeightBoard()];
        for (int i = 0; i < getHeightBoard(); ++i)
            board[i] = new char[getWidthBoard()];

        for (int i = 0; i < getHeightBoard(); ++i)
            for (int k = 0; k < getWidthBoard(); ++k)
                board[i][k] = ' ';

        for (int i = 0; i < getHeightBoard(); ++i) {
            for (int k = 0; k < getWidthBoard(); ++k) {
                if (i == 0 || k == 0 || i == (getHeightBoard() - 1) || k == (getWidthBoard() - 1) ) {
                    board[i][k] = '*';
                }
            }
        }
    }
public:
    void renderBoard()
    {
        bool head = false;
        bool isApple = false;
        for (int i = 0; i < getHeightBoard(); ++i)
        {
            std::vector<coordinates> c_snakeBody{ snake->getSnakeBody() };
            for (int k = 0; k < getWidthBoard(); ++k)
            {
                int flagSpace = 0;
                if (!isApple && equalsWithNumbers(apple->getAppleCoordinates(), i, k))
                {
                    std::cout << '@';
                    isApple = true;
                    flagSpace = 1;
                }
                for (auto j = c_snakeBody.begin() + 1; j != c_snakeBody.end(); ++j)
                {
                    if (!head && equalsWithNumbers(snake->getSnakeHead(), i, k))
                    {
                        std::cout << 'X';
                        head = true;
                        flagSpace = 1;
                        break;
                    }
                    if (equalsWithNumbers(*j, i, k))
                    {
                        std::cout << 'o';
                        flagSpace = 1;
                        break;
                    }
                }

                if (flagSpace == 1)
                {
                    flagSpace = 0;
                    continue;
                }
                std::cout << board[i][k];
            }
            std::cout << std::endl;
        }
    }  
    bool eatApple()
    {
        bool ateApple = snake->getSnakeHead() == apple->getAppleCoordinates();
        if (ateApple)
        {
            score++;
            apple->setNewApple();
            return ateApple;
        }
        return false;
    }
    bool lose()
    {
        return OR(snake->getSnakeHead(), 0, 0) || OR(snake->getSnakeHead(), getHeightBoard() - 1, getWidthBoard() - 1);
    }
public:
    Game()
    {
        Snake snake;
        this->snake = &snake;
        Apple apple;
        this->apple = &apple;
        board_matrix();
        while(!lose())
        {
            renderBoard();
            snake.takeStep(eatApple());
            system("CLS");
        }
        std::cout << "You lose!" << std::endl;
        std::cout << "Your score: " << score << std::endl;
        std::cin.get();
    }
};

int main()
{
    Game game;
    return 0;
}