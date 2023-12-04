#pragma once
#include "Std.h"
#include "Sprite.h"
#define S_SpriteManager SpriteManager::GetInstance()

class SpriteManager : public Singleton<SpriteManager>
{
private:
	friend class Singleton<SpriteManager>;
	std::vector<W_STR>			_infoFiles;
	std::vector<W_STR>			_spriteNames;
	std::vector<RECT_ARRAY>		_spriteRects;
	std::vector<W_STR>			_texfileNames;
	std::map<W_STR, Sprite*>	_sprites;
public:
	ID3D11Device*				_device				= nullptr;
	ID3D11DeviceContext*		_immediateContext	= nullptr;
public:
	void	SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	bool	LoadSpriteInfoF(W_STR infoFileName);
	bool	LoadSprite(W_STR infoFileName);
	Sprite* Get(W_STR spriteName);
public:
	bool	Release();
private:
	SpriteManager();
public:
	~SpriteManager();
};

