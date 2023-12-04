#pragma once
#include "StaticObject.h"
#include "SpriteManager.h"

class Portal : public StaticObject
{
public:
	Sprite* _sprite;
	Vector2 _tpPos;
	int		_color;
	int		_index = 0;
	float	_elapsedTime = 0.0f;
public:
	void	Update();
};

