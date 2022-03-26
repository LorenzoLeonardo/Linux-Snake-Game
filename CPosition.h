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