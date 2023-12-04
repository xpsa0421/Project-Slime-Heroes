#pragma once
#include "StaticObject.h"
#include "SpriteManager.h"

class Enemy : public StaticObject
{
public:
	bool	_isDead = false;
	Sprite* _sprite = nullptr;

	float	_elapsedTime = 0.0f;
	float	_jumpTime = 0.7f;

public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	void	SetPosition(Vector2 pos);
	void	SetPosition(Vector2 pos, Vector2 cameraPos);
	void	SetCameraPos(Vector2 cameraPos);
	void	SetCameraSize(Vector2 cameraSize);
};

