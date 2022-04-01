/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include "CFood.h"
#include "CSnake.h"

namespace TIMER
{
    inline uint32_t getTickCount() 
    {
        struct timespec ts;
        unsigned theTick = 0U;
        clock_gettime( CLOCK_REALTIME, &ts );
        theTick  = ts.tv_nsec / 1000000;
        theTick += ts.tv_sec * 1000;
        return theTick;
    }

    inline void sleep(int n)
    {
        usleep(n*1000);
    }
}

class CMain
{
private:

    COORD m_cursorPosition;
    COORD m_screenSize;
    char m_foodChar;
    int m_snakeChar;
    char m_deleteTrailingChar;
    Snake_Direction m_dir;
    int m_nGameSpeed;
    std::string m_nameLabel;
    termios m_initialKBSettings;

    inline void gotoxy(int x, int y)
    {
        printf("%c[%d;%df",0x1B,y,x);

    }
    inline void drawSnake(CSnake &snake)
    {
        gotoxy(snake.getTail().X, snake.getTail().Y);
        printf("%c", m_deleteTrailingChar);

        gotoxy((m_screenSize.X/2) - ((int)m_nameLabel.length()/2), m_screenSize.Y + 1);
        printf("%s", m_nameLabel.c_str());

        gotoxy(1,1);
        printf("╔");
        gotoxy(m_screenSize.X,1);
        printf("╗");
        gotoxy(1,m_screenSize.Y);
        printf("╚");
        gotoxy(m_screenSize.X,m_screenSize.Y);
        printf("╝");
        
        for(int y = 2 ; y < (m_screenSize.Y);y++)
        {
            gotoxy(1,y);
            printf("║");

            gotoxy(m_screenSize.X,y);
            printf("║");
        }

        for(int x = 2 ; x < (m_screenSize.X); x++)
        {
            gotoxy(x,1);
            printf("═");

            gotoxy(x,m_screenSize.Y);
            printf("═");
        }
        snake.setDirection(m_dir);
        snake.crawl();

        for (int i = 0; i < snake.getLength(); i++)
        {
            gotoxy(snake.getBodyLocation()[i].X, snake.getBodyLocation()[i].Y);
            printf("%c",  m_snakeChar);
        }
        fflush(stdout);
    }
    inline void drawFood(CFood &food, bool isEaten)
    {
        if (food.isBonusFood())
            m_foodChar = 'B';
        else
            m_foodChar = 'O';
        
        gotoxy(food.getPosition().X, food.getPosition().Y);
        printf("%c",  m_foodChar);
        fflush(stdout);
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
    inline bool hasEaten(CSnake& snake, CFood& food)
    {
        return ((snake.getHead().X == food.getPosition().X) && snake.getHead().Y == food.getPosition().Y);
    }
public:
    CMain()
    {
        m_foodChar = 'O';
        m_snakeChar = '@';
        m_deleteTrailingChar = ' ';
        m_dir = Snake_Direction::RIGHT;
        m_nGameSpeed = 50;
        m_cursorPosition = { 0,0 };
        m_screenSize = { 0,0 };
        m_nameLabel = "Lorenzo Leonardo's Snake Game Linux Console Using C++ (c) 2022";
       
        system("clear");
        printf("\e[?25l");
        tcgetattr(0,&m_initialKBSettings);
    }
    ~CMain()
    {
        
        printf("\e[?25h");
        tcsetattr(0, TCSANOW, &m_initialKBSettings);
        system("clear");
    }
    void initializeArea()
    {
        m_screenSize.X = 80;
        m_screenSize.Y = 25;

        printf("\e[8;%d;%dt",m_screenSize.Y + 1, m_screenSize.X);
       
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
           
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
    }

    void run()
    {
        CFood food(m_screenSize);
        CSnake snake(m_screenSize);
        uint32_t unTimePrev = 0, unTimeCurrent = 0;
        m_dir = Snake_Direction::RIGHT;

        food.createFood();
        
        unTimePrev = unTimeCurrent = TIMER::getTickCount();

        while (snake.isAlive() && listenForKeyPress())
        {
            drawSnake(snake);
            if(food.isBonusFood())
            {
                //Bonus food will only appear for 5 seconds and disappear after.
                unTimeCurrent = TIMER::getTickCount();
                if((unTimeCurrent - unTimePrev) > 5000)
                {
                    gotoxy(food.getPosition().X, food.getPosition().Y);
                    printf(" ");
                    food.createFood();
                    unTimePrev = unTimeCurrent;
                }
            }
            else
                unTimePrev = unTimeCurrent = TIMER::getTickCount();

            if (hasEaten(snake,food))
            {
                COORD posEaten;
                posEaten = food.getPosition();
               
                if (food.isBonusFood())
                    snake.growBonus(posEaten, 5);
                else
                    snake.grow(posEaten);
  
                food.createFood();
            }
            drawFood(food, false);
            TIMER::sleep(m_nGameSpeed);
        }

    }
};

int main()
{
    CMain main;

    main.initializeArea();
    main.run();


    return 0;
}