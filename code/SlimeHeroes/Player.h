#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"
#include "SpriteManager.h"
#include "StaticObject.h"
#include "Input.h"
#include "Enemy.h"
extern std::vector<StaticObject*>		_colliders;
extern std::vector<StaticObject*>		_props;
extern std::vector<Enemy*>				_enemies;

// identifier of player motion
enum PlayerState
{
	IDLE,
	MOVE,
	JUMP,
	SWALLOW,
	HURT,
	DEAD,
	MOVEFALL,
	MOVEJUMP
};

class Player : public DynamicObject
{
public:
	bool					_moveAllowed = true;
	bool					_grounded	= false;
	float					_xOffset	= 0.0f;
	PlayerState				_state		= IDLE;
	Colour					_colour		= GREY;	
	int						_prevXDir	= 0.0f;
	Vector2					_origin		= { 0,0 };
	Sprite*					_currSprite = nullptr;
	std::vector<Sprite*>	_sprites;
public:
	bool	Frame()		override;
	bool	Render()	override;
	void	Create();
	void	UpdateSprite();
public:
	void	SetColour(Colour colour);
	void	SetPosition(Vector2 pos)			override;
	void	SetPosition(Vector2 pos, Vector2 cameraPos)	override;
	void	SetCameraPos(Vector2 pos)			override;
	void	SetCameraSize(Vector2 cameraSize)	override;
	void	SetState(PlayerState state);
public:
	void	HandleInput();
	void	ResolveCollisions();
};

