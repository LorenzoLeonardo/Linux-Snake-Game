/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/
#pragma once
#include "CPosition.h"

class CFood : public CPosition
{
private:
	bool m_bIsBonus;
	COORD m_previousPos;

public:
	CFood();
	CFood(COORD);
	~CFood();

	void createFood();
	bool isBonusFood();
	COORD getPreviousPosition()
	{
		return m_previousPos;
	}

};