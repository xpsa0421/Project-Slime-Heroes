#include "Enemy.h"

bool Enemy::Init()
{
    _sprite = S_SpriteManager.Get(L"FROG");
	_sprite->_updateTime = 0.5f;
    return true;
}

bool Enemy::Frame()
{
	if (_isDead) return true;
    _sprite->Frame();
    return true;
}

bool Enemy::Render()
{
	if (_isDead) return true;
    _sprite->Render();
    return true;
}

void Enemy::SetPosition(Vector2 pos)
{
	_pos = pos;
	_sprite->SetPosition(pos);
	_collider = _sprite->_collider;
}

void Enemy::SetPosition(Vector2 pos, Vector2 cameraPos)
{
	_pos = pos;
	_sprite->SetPosition(pos, cameraPos);
	_collider = _sprite->_collider;
}

void Enemy::SetCameraPos(Vector2 cameraPos)
{
	_cameraPos = cameraPos;
	_sprite->SetCameraPos(cameraPos);
}

void Enemy::SetCameraSize(Vector2 cameraSize)
{
	_cameraSize = cameraSize;
	_sprite->SetCameraSize(cameraSize);
}