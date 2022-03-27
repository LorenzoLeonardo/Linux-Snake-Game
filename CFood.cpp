#include "CFood.h"
 #include <stdlib.h>
#include <ctime>

CFood::CFood()
{
    srand((unsigned int)time(NULL));
    m_bIsBonus = false;
}
CFood::CFood(COORD coor): CPosition(coor)
{
    srand((unsigned int)time(NULL));
    m_bIsBonus = false;
}
CFood::~CFood()
{

}

void CFood::createFood()
{
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