#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "CFood.h"
#include "CSnake.h"

class CMain
{
private:

    COORD m_cursorPosition;
    COORD m_screenSize;
    char m_foodChar;
    char m_snakeChar;
    char m_deleteTrailingChar;
    Snake_Direction m_dir;
    int m_nGameSpeed;
    std::string m_nameLabel;

    inline void gotoxy(int x, int y)
    {
        printf("%c[%d;%df",0x1B,y,x);
        fflush(stdout);
    }
    inline void drawSnake(CSnake &snake)
    {
        gotoxy(snake.getTail().X, snake.getTail().Y);
        printf("%c", m_deleteTrailingChar);

        gotoxy((m_screenSize.X/2) - ((int)m_nameLabel.length()/2), m_screenSize.Y - 1);
        printf("%s", m_nameLabel.c_str());

        snake.setDirection(m_dir);
        snake.crawl();

        for (int i = 0; i < snake.getLength(); i++)
        {
            gotoxy(snake.getBodyLocation()[i].X, snake.getBodyLocation()[i].Y);
            printf("%c",  m_snakeChar);
        }
    }
    inline void drawFood(CFood &food)
    {
        if (food.isBonusFood())
            m_foodChar = '8';
        else
            m_foodChar = '0';

        gotoxy(food.getPosition().X, food.getPosition().Y);
        printf("%c",  m_foodChar);
    }
    inline void sleep(int n)
    {
        usleep(n*1000);
    }
    bool kbhit()
    {
        termios term;
        tcgetattr(0, &term);

        termios term2 = term;
        term2.c_lflag &= ~ICANON;
        tcsetattr(0, TCSANOW, &term2);

        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting);

        tcsetattr(0, TCSANOW, &term);

        return byteswaiting > 0;
    }

    inline bool listenForKeyPress()
    {
       int nKey = 0;
       if(kbhit())
       {
            nKey = getch();
            gotoxy(0,0);
            printf(" ");
            if (nKey == KEY_LEFT  && m_dir != Snake_Direction::RIGHT)
            {
                m_dir = Snake_Direction::LEFT;
            }
            else if (nKey == KEY_RIGHT && m_dir != Snake_Direction::LEFT)
            {
                m_dir = Snake_Direction::RIGHT;
            }
            else if (nKey == KEY_UP && m_dir != Snake_Direction::DOWN)
            {
                m_dir = Snake_Direction::UP;
            }
            else if (nKey == KEY_DOWN && m_dir != Snake_Direction::UP)
            {
                m_dir = Snake_Direction::DOWN;
            }
            else if (nKey == 0x1B)
            {
                return false;
            }
       }
       return true;
    }
    inline bool HasEaten(CSnake& snake, CFood& food)
    {
        return ((snake.getHead().X == food.getPosition().X) && snake.getHead().Y == food.getPosition().Y);
    }
public:
    CMain()
    {
        m_foodChar = '0';
        m_snakeChar = '@';
        m_deleteTrailingChar = ' ';
        m_dir = Snake_Direction::RIGHT;
        m_nGameSpeed = 50;
        m_cursorPosition = { 0,0 };
        m_screenSize = { 0,0 };
        m_nameLabel = "Lorenzo Leonardo's Snake Game Linux Console Using C++ (c) 2022";
    }
    void initializeArea()
    {
        system("clear");

        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        m_screenSize.X = 80;
        m_screenSize.Y = 25;
      
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
    }

    void run()
    {
        CFood food(m_screenSize);
        CSnake snake(m_screenSize);
       
        m_dir = Snake_Direction::RIGHT;

        food.createFood();
        drawFood(food);

        while (snake.isAlive() && listenForKeyPress())
        {
            drawSnake(snake);

            if (HasEaten(snake,food))
            {
                COORD posEaten;
                posEaten = food.getPosition();
               

                if (food.isBonusFood())
                {
                    snake.growBonus(posEaten, 5);
                }
                else
                {
                    snake.grow(posEaten);
                }
                food.createFood();
            }
            drawFood(food);
            sleep(m_nGameSpeed);
        }
        system("clear");
    }
};

int main()
{
    CMain main;

    main.initializeArea();
    main.run();


    return 0;
}