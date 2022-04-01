/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/
#include "CPosition.h"
CPosition::CPosition()
{
    m_pos = {0,0};
}
CPosition::CPosition(COORD screenSize)
{
    m_pos = {0,0};
    m_screenSize = screenSize;
}
CPosition::~CPosition()
{

}
COORD CPosition::getPosition()
{
    return m_pos;
}

COORD CPosition::getScreenSize()
{
   return m_screenSize; 
}