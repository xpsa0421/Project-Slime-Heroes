#pragma once
#include "StaticObject.h"
typedef std::vector<RECT> RECT_ARRAY;

// identifier of player colour
enum Colour
{
	GREY,
	BLUE,
	GREEN,
	YELLOW
};

class Sprite : public StaticObject
{
public:
	W_STR			_name;
	RECT_ARRAY		_rects;
	int				_index			= 0;
	float			_elapsedTime	= 0.0f;
	float			_updateTime		= 0.5f;
	bool			_isLooping		= true;
	bool			_isLoopFinished = false;

public:
	bool	CreateSprite(
		ID3D11Device* device, ID3D11DeviceContext* context,
		std::wstring texFileName);
	void	SetPosition(Vector2 pos)					override;
	void	SetPosition(Vector2 pos, Vector2 cameraPos)	override;

	void	ResetFrame();
	void	UpdateFrame();
};

