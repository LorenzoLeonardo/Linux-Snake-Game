/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/
#pragma once

typedef struct _tCoor
{
     int X;
     int Y;
}COORD;

class CPosition
{
protected:
	COORD m_pos;
    COORD m_screenSize;
public:
    CPosition();
    CPosition(COORD coor);
    ~CPosition();
	COORD getPosition();
    COORD getScreenSize();
};