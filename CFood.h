#pragma once
#include "CPosition.h"

class CFood : public CPosition
{
private:
	bool m_bIsBonus;

public:
	CFood();
	CFood(COORD);
	~CFood();

	void createFood();
	bool isBonusFood();
};