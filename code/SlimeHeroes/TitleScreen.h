#pragma once
#include "Scene.h"
#include "StaticObject.h"
#include "Input.h"

struct Button :StaticObject
{
	float					_elapsedTime	= 0.0f;
	int						_index			= 1;
	std::vector<Texture*>	_texList;
	Texture*				_tex			= nullptr;
	bool					_clicked		= false;

	bool Render()
	{
		StaticObject::PreRender();
		_immediateContext->PSSetShaderResources(0, 1, &_tex->_textureSRV);
		StaticObject::PostRender();
		return true;
	}

	bool Frame()
	{
		POINT mousePos = S_Input._mousePos;
		if (Collision2D::RectToPoint(_collider, mousePos))
		{
			_tex = _texList[1];
			if (S_Input.GetKey(VK_LBUTTON) == KEY_UP) {
				_clicked = true;
			}
		}
		else _tex = _texList[0];
		
		return true;
	}
};

class TitleScreen : public Scene
{
public:
	BaseObject*		_title;
	Button*			_playButton;
	Button*			_quitButton;
	bool			_gameStart		= false;
	bool			_gameQuit		= false;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool IsNextScene()override  { return _gameStart; };
	bool IsGameQuit() override	{ return _gameQuit; };
};

