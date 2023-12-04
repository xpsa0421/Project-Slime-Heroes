#pragma once
#include "Scene.h"
#include "Player.h"
#include "ScrollObject.h"	
#include "Camera.h"
#include "SpriteManager.h"
#include "Portal.h"
#include "StaticObject.h"

struct Gem : StaticObject
{
	Colour _colour;
};

struct Emote : StaticObject
{
	std::vector<Rect>	_rects;
	Rect				_currRect;
	std::vector<bool> _show;
	float	_elapsedTime = 0.0f;
	void Update()
	{
		for (int i = 0; i < _show.size(); i++)
		{
			if (_show[i] == true)
			{
				if (_elapsedTime >= 1.5f)
				{
					_show[i] = false;
					_elapsedTime = 0.0f;
				}
				_elapsedTime += g_secondPerFrame;
			}
		}
	}
};

struct PopUpScreen : StaticObject
{
	int _texIdx = 0;
	std::vector<Texture*>			_textures;

	void Reset()
	{
		_texIdx = 0;
	};
	bool Render()
	{
		StaticObject::PreRender();
		_immediateContext->PSSetShaderResources(0, 1, &_textures[_texIdx]->_textureSRV);
		StaticObject::PostRender();
		return true;
	}
};

class InGameScreen : public Scene
{
public:
	W_STR	_defaultShader;
	Camera	_cam;
	Player* _player = nullptr;
public:
	std::vector<Texture*>			_colliderTextures;
	std::vector<Texture*>			_propsTextures;
	std::vector<std::vector<int>>	_colliderInfo;
	std::vector<std::vector<int>>	_propInfo;
	std::vector<Portal*>			_portals;
	std::vector<int>				_portalColorInfo; // Grey - Blue - Green - Yellow
	bool							_colourCollected[4] = { 1,0,0,0 };
	bool							_colourCollectedInit[4] = { 1,0,0,0 };
	Colour							_playerColourInit = GREY;
	std::vector<Vector2>			_portalTPInfo;
	std::vector<StaticObject*>		_obstacles;
	std::vector<ScrollObject*>		_backgrounds;
	std::vector<Gem*>				_gems;
	std::vector<PopUpScreen*>		_dialogs;
	std::vector<PopUpScreen*>		_guides;
	int				_guideIdx = -1;
	int				_dialogIdx = -1;
public:
	virtual void	GenerateBackground() {};
	virtual void	GenerateWorld() {};
	virtual void	GeneratePortals(STR fileName);
	virtual void	GeneratePlayer() {};
	virtual void	GenerateDialogs() {};
	virtual void	GenerateGuides() {};
	virtual void	GameOver();
	virtual void	RestartScene() override;
	virtual void	LoadInfoF(STR fileName, std::vector<std::vector<int>>& infoVector);
	virtual void	Generate(std::vector<std::vector<int>>& worldInfo,
		std::vector<StaticObject*>& world, W_STR texFileName);
};

