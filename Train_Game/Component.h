#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <SDL.h>
#include <string>

struct Vector2
{
public:
	Vector2(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	int x;
	int y;
private:
	bool operator==(const Vector2& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}
	Vector2& operator=(const Vector2& rhs) //not needed yet
	{
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}
};

class Component
{
public:
	virtual Vector2& GetPosition() = 0;
private:
	virtual void SetPosition(int x, int y) = 0;
};

#endif