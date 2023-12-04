#include "Player.h"
#include "Sample.h"
#include "Enemy.h"
std::vector<Enemy*>	_enemies;
std::vector<StaticObject*>	_colliders;
std::vector<StaticObject*>	_props;

void Player::Create()
{
	// retrieve sprites
	_sprites.resize(8);
	_sprites[0] = S_SpriteManager.Get(L"IDLE");
	_sprites[1] = S_SpriteManager.Get(L"MOVE");
	_sprites[2] = S_SpriteManager.Get(L"JUMP");
	_sprites[3] = S_SpriteManager.Get(L"SWALLOW");
	_sprites[4] = S_SpriteManager.Get(L"HURT");
	_sprites[5] = S_SpriteManager.Get(L"DEAD");
	_sprites[6] = S_SpriteManager.Get(L"MOVEFALL");
	_sprites[7] = S_SpriteManager.Get(L"MOVEJUMP");

	// set sprite update times
	_sprites[IDLE]->_updateTime		= 0.4f;
	_sprites[MOVE]->_updateTime		= 0.3f;
	_sprites[JUMP]->_updateTime		= 0.21f;
	_sprites[SWALLOW]->_updateTime	= 0.07f;
	_sprites[HURT]->_updateTime		= 0.2f;
	_sprites[DEAD]->_updateTime		= 0.13f;
	_sprites[MOVEFALL]->_updateTime	= 0.4f;
	_sprites[MOVEJUMP]->_updateTime	= 0.4f;

	_sprites[DEAD]->_isLooping = false;
	_sprites[JUMP]->_isLooping = false;
	_sprites[SWALLOW]->_isLooping = false;
	_sprites[HURT]->_isLooping = false;

	// initialise sprite to the first sprite image (i.e. idle[0])
	SetState(IDLE);
	SetSpeed(70.0f);
	_gravity = 1.3f;
}

void Player::SetColour(Colour colour)
{
	_colour = colour;
	if (_colour == Colour::GREY)
	{
		_currSprite->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else if (_colour == Colour::BLUE)
	{
		_currSprite->SetColour({ 0.2f, 0.671f, 0.961f, 1.0f });
	}
	else if (_colour == Colour::GREEN)
	{
		_currSprite->SetColour({ 0.525f, 0.831f, 0.439f, 1.0f });
	}
	else if (_colour == Colour::YELLOW)
	{
		_currSprite->SetColour({ 0.9f, 0.657f, 0.157f, 1.0f });
	}
}

/*
*	주의 !!!
*	Player/Sprite SetPosition은 sprite의 하단 중앙 pos가 parameter로 넘어옴.
*/
void Player::SetPosition(Vector2 pos)
{
	_pos = pos;
	_currSprite->SetPosition(pos);
	_collider.SetFromBottomCentre(pos.x, pos.y, 15, 15);
}

void Player::SetPosition(Vector2 pos, Vector2 cameraPos)
{
	_pos = pos;
	_currSprite->SetPosition(pos, cameraPos);
	_collider.SetFromBottomCentre(pos.x, pos.y, 15, 15);
}

void Player::SetCameraPos(Vector2 cameraPos)
{
	_cameraPos = cameraPos;
	_currSprite->SetCameraPos(cameraPos);
}

void Player::SetCameraSize(Vector2 cameraSize)
{
	_cameraSize = cameraSize;
	for (auto sprite : _sprites)
	{
		sprite->SetCameraSize(cameraSize);
	}
}

void Player::SetState(PlayerState state)
{
	_state = state;
	_currSprite = _sprites[state];
	_currSprite->ResetFrame();
	SetColour(_colour);
	SetCameraPos(_cameraPos);
	SetPosition(_pos, _cameraPos);
}

bool Player::Frame()
{
	// Update action & position
	if (_moveAllowed == true &&_state != DEAD && _state != SWALLOW) HandleInput();

	// Apply gravity
	if (!_grounded && _state != DEAD)	_dir.y += _gravity * g_secondPerFrame;
	else _dir.y = 0;

	// Update player sprite
	UpdateSprite();

	_pos += _dir * _speed * g_secondPerFrame;
	SetPosition(_pos, _cameraPos);	

	// Collision detection and resolution
	ResolveCollisions();

	_currSprite->_immediateContext->UpdateSubresource(
		_currSprite->_vertexBuffer, NULL, nullptr, &_currSprite->_vertices.at(0), 0, 0);

	return true;
}

void Player::HandleInput()
{
	bool keyPressed = false;
	_xOffset = 0.0f;
	_dir.x = 0.0f;
	PlayerState newState = _state;

	if (S_Input.GetKey('A'))
	{
		if (_dir.y != 0)
		{
			_dir.y > 0 ? newState = MOVEFALL : newState = MOVEJUMP;
			_dir.x -= 0.7f;
			_xOffset -= 0.7f;
		}
		else
		{
			newState = MOVE;
			_dir.x -= 1.0f;
			_xOffset -= 1.0f;
		}		
		keyPressed = true;
	}
	if (S_Input.GetKey('D'))
	{
		if (_dir.y != 0)
		{
			_dir.y > 0 ? newState = MOVEFALL : newState = MOVEJUMP;
			_dir.x += 0.7f;
			_xOffset += 0.7f;
		}
		else
		{
			newState = MOVE;
			_dir.x += 1.0f;
			_xOffset += 1.0f;
		}
		keyPressed = true;
	}
	if (S_Input.GetKey('W') && _grounded)
	{
		newState = JUMP;
		_dir.y = -1.2f;
		keyPressed = true;
		_grounded = false;
	}
	if (S_Input.GetKey(VK_SHIFT) && _grounded)
	{
		newState = SWALLOW;
		_dir.x = 0;
		_xOffset = 0;
		keyPressed = true;
	}
	if (keyPressed == false && _dir.y == 0)
	{
		newState = IDLE;
	}

	if ((_dir.x != 0)) 
		_prevXDir = _dir.x;

	if (S_Input.GetKey(VK_F8))
	{
		SetPosition(_origin);
		_state = IDLE;
		_dir = { 0,0 };
	}

	if (newState != _state) SetState(newState);
}

void Player::ResolveCollisions()
{
	if (_dir.y != 0)
	{
		_grounded = false;
		for (auto tile : _colliders)
		{
			if (Collision2D::RectToRect(tile->_collider, _collider))
			{
				if (// 플레이어의 바깥 대각선 왼쪽 위치
					((tile->_collider.x1 < _collider.x1) && (tile->_collider.x2 <= _collider.x1)) ||
					// 플레이어의 바깥 대각선 오른쪽 위치
					((tile->_collider.x1 >= _collider.x2) && (tile->_collider.x2 > _collider.x2)))
				{
					continue;
				}

				if (_dir.y > 0)			// object falling
				{	
					if (tile->_collider.y1 >= _collider.y2-1)
					{
						_pos.y = tile->_collider.y1;
						_dir.y = 0;
						_grounded = true;
						break;
					}
				}
				else if (_dir.y < 0) {	// object jumping
					if (tile->_collider.y2 <= _collider.y1 + 1)
					{
						_pos.y = tile->_collider.y2 + _collider.h;
						_dir.y = 0;
						break;
					}
				}
			}
		}
	}

	if (_dir.x != 0)
	{
		_grounded = false;
		for (auto tile : _colliders)
		{
			if (Collision2D::RectToRect(tile->_collider, _collider))
			{
				if ((tile->_collider.y1 >= _collider.y2-1) &&
					!(	// 플레이어의 바깥 대각선 왼쪽 위치
						((tile->_collider.x1 < _collider.x1) && (tile->_collider.x2 <= _collider.x1)) ||
						// 플레이어의 바깥 대각선 오른쪽 위치
						((tile->_collider.x1 >= _collider.x2) && (tile->_collider.x2 > _collider.x2)))
					)
				{
					_grounded = true;
				}
				
				if (_dir.x > 0) {		// object moving right
					if (tile->_collider.y1 < _collider.y2) {
						_pos.x = tile->_collider.x1 - _collider.w / 2;
						_xOffset = 0;
					}
				}
				else if (_dir.x < 0) {	// object moving left
					if (tile->_collider.y1 < _collider.y2) {
						_pos.x = tile->_collider.x2 + _collider.w / 2;
						_xOffset = 0;
					}
					/*else if (tile->_collider.y2 > _collider.y2) {
						_grounded = true;
					}*/
				}
			}
		}
	}

	for (auto enemy : _enemies)
	{
		if (Collision2D::RectToRect(enemy->_sprite->_collider, _collider))
		{
			if (_state == SWALLOW)
			{
				enemy->_isDead = true;
				SetColour(GREEN);
			}
		}
	}
	
	SetPosition(_pos, _cameraPos);
}

bool Player::Render()
{
	_currSprite->PreRender();

	// if looking left, flip texture vertex
	if (_prevXDir < 0)
	{
		_currSprite->_vertices[0].t.x = _currSprite->_rectUV.x1 + _currSprite->_rectUV.w;
		_currSprite->_vertices[1].t.x = _currSprite->_rectUV.x1;
		_currSprite->_vertices[2].t.x = _currSprite->_rectUV.x1 + _currSprite->_rectUV.w;
		_currSprite->_vertices[3].t.x = _currSprite->_rectUV.x1;
		_currSprite->_immediateContext->UpdateSubresource(
			_currSprite->_vertexBuffer, NULL, nullptr, &_currSprite->_vertices.at(0), 0, 0);
	}
	_currSprite->PostRender();
	return true;
}

void Player::UpdateSprite()
{
	_currSprite->UpdateFrame();
	if (_state == DEAD && _currSprite->_isLoopFinished)
	{
		_currentScene->RestartScene();
	}
	else if (_state == SWALLOW && _currSprite->_isLoopFinished)
	{
		SetState(IDLE);
	}
}
