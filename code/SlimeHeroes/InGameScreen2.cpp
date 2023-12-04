#include "InGameScreen2.h"
#include "Enemy.h"

bool InGameScreen2::Init()
{
	_defaultShader = L"../../resource/shader/defaultShader.txt";
	_colourCollected[1] = 1;

	// camera initialisation
	_cam.SetSize({ 1000 / 2, 625 / 2 });
	_cam.SetLimit(-40.0f, 70.0f, -480.0f, 60.f);

	// world initialisation
	GenerateBackground();
	GenerateWorld();
	GeneratePlayer();

	_alertPopUp = new StaticObject;
	_alertPopUp->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/alert.png",
		_defaultShader
	);
	_alertPopUp->SetRect({ 0, 0, 320, 202 });
	_alertPopUp->SetPosition({ (992 - 320) / 2, (624 - 202)/2 });

	return true;
}

void InGameScreen2::GenerateBackground()
{
	ScrollObject* bg = nullptr;
	W_STR filePath;
	const WCHAR* bgNames[4] = { L"bg", L"far_trees", L"mid_trees", L"close_trees" };
	float bgSpeed[4] = { 0.00f, 0.005f, 0.001f, 0.01f };

	for (int i = 0; i < 4; i++)
	{
		bg = new ScrollObject;
		filePath = mtw("../../resource/background/DemonWoods_") + bgNames[i] + mtw(".png");
		bg->Create(_device, _immediateContext, filePath, _defaultShader);
		bg->SetRect({ 0, 0, 592, 272 });
		bg->SetScrollSpeed(bgSpeed[i]);
		_backgrounds.push_back(bg);
	}
}


void InGameScreen2::GenerateWorld()
{
	// Generate world 
	LoadInfoF("../../resource/sprite/mapColliderInfo2.txt", _colliderInfo);
	LoadInfoF("../../resource/sprite/mapPropsInfo2.txt", _propInfo);
	Generate(_colliderInfo, _colliders, L"../../resource/sprite/map/mapTileSetBlue.png");
	Generate(_propInfo, _props, L"../../resource/sprite/map/propsTileSetBlue.png");

	// Save textures for world
	_colliderTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/mapTileSetBlue.png"));
	_colliderTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/mapTileSetGreen.png"));
	_propsTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/propsTileSetBlue.png"));
	_propsTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/propsTileSetGreen.png"));

	// Generate portals
	GeneratePortals("../../resource/sprite/portalInfo2.txt");

	_house = new StaticObject;
	_house->Create(_device, _immediateContext,
		L"../../resource/sprite/map/propsTilesetBlue.png", _defaultShader);
	_house->SetRect({ 192, 0, 96, 112 });
	_house->SetPosition({ -304 + (14 * 16), -184 + (0 * 16) });
	_house->SetCameraPos(_cam._pos);
	_house->SetCameraSize(_cam._size);

	// Generate gems
	// Generate game end indicator
}

void InGameScreen2::GeneratePlayer()
{
	_player = new Player;
	_player->Create(); 
	_player->SetPosition({ -273, -20 });
	_player->SetDirection({ 0,0 });
	_player->SetCameraPos(_cam._pos);
	_player->SetCameraSize(_cam._size);
	_player->SetColour(Colour::BLUE);
	_player->_origin = { -273, -8 };

	Enemy* enemy = new Enemy;
	enemy->Init();
	enemy->SetCameraPos(_cam._pos);
	enemy->SetCameraSize(_cam._size);
	enemy->SetPosition({ 15, 168 });
	_enemies.push_back(enemy);
}


void InGameScreen2::GenerateGuides()
{
	return;
}

void InGameScreen2::GenerateDialogs()
{
	return;
}

bool InGameScreen2::Frame()
{
	if (_showAlert)
	{
		_alertPopUp->Frame();
		return true;
	}

	_player->SetCameraPos(_cam._pos);
	_player->Frame();
	
	for (auto enemy : _enemies)
	{
		enemy->SetCameraPos(_cam._pos);
		enemy->Frame();
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (i != 2) _backgrounds[i]->SetScrollDirection(_player->_xOffset);
		_backgrounds[i]->Frame();
	}

	for (StaticObject* collider : _colliders)
	{
		collider->SetCameraPos(_cam._pos);
		collider->Frame();
	}
	for (StaticObject* prop : _props)
	{
		prop->SetCameraPos(_cam._pos);
		prop->Frame();
	}
	// Portal 
	for (Portal* portal : _portals)
	{
		portal->SetCameraPos(_cam._pos);
		portal->Update();

		if (Collision2D::RectToRect(portal->_collider, _player->_collider)) {
			if (_colourCollected[portal->_color] == true)
				_player->SetPosition(portal->_tpPos);
		}
	}

	_house->SetCameraPos(_cam._pos);
	_house->Frame();
	if (Collision2D::RectToRect(_house->_collider, _player->_collider)) {
		_showAlert = true;
		_player->_moveAllowed = false;
	}

	_cam.AdjustPos(_player->_pos);
	return true;
}

bool InGameScreen2::Render()
{
	D3D11_VIEWPORT vp;
	vp.Width = 992;
	vp.Height = 624;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_immediateContext->RSSetViewports(1, &vp);

	for (auto bg : _backgrounds)
	{
		bg->Render();
	}
	for (StaticObject* prop : _props)
	{
		prop->StaticObject::PreRender();
		if (_player->_colour == GREEN)
		{
			prop->_immediateContext->PSSetShaderResources(0, 1, &_propsTextures[1]->_textureSRV);
		}
		prop->StaticObject::PostRender();
	}
	for (StaticObject* collider : _colliders)
	{
		collider->StaticObject::PreRender();
		if (_player->_colour == GREEN)
		{
			collider->_immediateContext->PSSetShaderResources(0, 1, &_colliderTextures[1]->_textureSRV);
		}
		collider->StaticObject::PostRender();
	}
	for (auto portal : _portals)
	{
		portal->_sprite->SetPosition(portal->_pos, portal->_cameraPos);
		portal->_sprite->Render();
	}
	_house->StaticObject::PreRender();
	_house->_immediateContext->PSSetShaderResources(0, 1, &_propsTextures[1]->_textureSRV);
	_house->StaticObject::PostRender();
	
	for (auto enemy : _enemies)
	{
		enemy->Render();
	}
	_player->Render();
	
	if (_showAlert)
	{
		_alertPopUp->Render();
	}
	return true;
}

bool InGameScreen2::Release()
{
	_house->Release();
	delete _house;
	_alertPopUp->Release();
	delete _alertPopUp;
	for (auto bg : _backgrounds)	{ bg->Release(); }
	for (auto portal : _portals)	{ portal->_sprite->Release();}
	for (auto portal : _portals)	{ delete portal; }
	for (StaticObject* gem : _gems)	{ gem->Release();}
	for (StaticObject* gem : _gems) { delete gem;	}
	for (auto prop : _props)		{ prop->Release();	}
	for (auto prop : _props)		{ delete prop;	}
	for (auto world : _colliders)	{ world->Release();	}
	for (auto world : _colliders)	{ delete world; }
	for (auto bg : _backgrounds)	{ delete bg;}
	for (auto bg : _backgrounds)	{ bg = nullptr;	}
	for (auto ene : _enemies)		{ ene->Release();}
	for (auto ene : _enemies)		{ delete ene; }
	_colliders.clear();
	_enemies.clear();
	_props.clear();
	_player->Release();
	_player = nullptr;
	delete _player;
	_backgrounds.clear();

	return true;
}