/*
* Developed By : Lorenzo T.Leonardo
* Date : March 23, 2022
* Email : enzotechcomputersolutions@gmail.com
*/
#pragma once

#include <algorithm>
#include <vector>
#include "CPosition.h"

enum class Snake_Direction
{
	UP, DOWN, RIGHT, LEFT
};

class CSnake : public CPosition
{

private:
	std::vector<COORD> m_vBody;
	COORD m_head;
	COORD m_tail;
	Snake_Direction m_direction;
	int m_nlength;
	bool m_bIsAlive;
	inline void checkBodyCollision();

public:
	CSnake(COORD coord);
	~CSnake();

	void crawl();
	void grow(COORD pos);
	void growBonus(COORD pos, int nLength);
	void setDirection(Snake_Direction dir);
	std::vector<COORD> getBodyLocation()
	{
		return m_vBody;
	}
	bool isAlive()
	{
		return m_bIsAlive;
	}
	COORD getTail()
	{
		return m_tail;
	}
	COORD getHead()
	{
		return m_head;
	}
	Snake_Direction getDirection()
	{
		return m_direction;
	}
	int getLength()
	{
		return m_nlength;
	}
};