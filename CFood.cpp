/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/

#include "CFood.h"
#include <stdlib.h>
#include <ctime>

CFood::CFood()
{
    srand((unsigned int)time(NULL));
    m_bIsBonus = false;
    m_pos = {2,2};
    m_previousPos = {2,2};
    m_screenSize = {0,0};
}
CFood::CFood(COORD screenSize)
{
    srand((unsigned int)time(NULL));
    m_bIsBonus = false;
    m_pos = {2,2};
    m_previousPos = {2,2};
    m_screenSize = screenSize;
}
CFood::~CFood()
{

}

void CFood::createFood()
{
    m_previousPos = m_pos;
    if (rand() % 5 == 1)
		m_bIsBonus = true;
	else
		m_bIsBonus = false;

	m_pos.X = (rand() % (m_screenSize.X - 2)) + 2;
	m_pos.Y = (rand() % (m_screenSize.Y - 2)) + 2;
}
bool CFood::isBonusFood()
{
    return m_bIsBonus;
}