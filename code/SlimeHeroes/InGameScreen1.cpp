#include "InGameScreen1.h"

bool InGameScreen1::Init()
{
	_defaultShader = L"../../resource/shader/defaultShader.txt";

	// camera initialisation
	_cam.SetSize({ 1000/2, 625/2 });
	_cam.SetLimit(-40.0f, 270.0f, -27.0f, -16.0f);
	
	// world initialisation
	GenerateBackground();
	GenerateWorld();
	GenerateObstacles();
	GeneratePlayer();
	GenerateGuides();
	GenerateDialogs();

	_stageClearPopUp = new StaticObject;
	_stageClearPopUp->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/stageClear.png",
		_defaultShader
	);
	_stageClearPopUp->SetRect({ 0, 0, 320, 345 });
	_stageClearPopUp->SetPosition({ 992 / 2 - 160, 624 / 2 - 172.5 });

	_controlsPopUp = new StaticObject;
	_controlsPopUp->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/controls.png",
		_defaultShader
	);
	_controlsPopUp->SetRect({ 0, 0, 320, 345 });
	_controlsPopUp->SetPosition({ 992 / 2 - 160, 624 / 2 - 172.5 });

	_BGMusic = S_SoundManager.LoadSound(L"../../resource/audio/background/Pikachu_Valley.mp3");

	return true;
}


void InGameScreen1::GenerateBackground()
{
	ScrollObject* bg = nullptr;
	W_STR filePath;
	const WCHAR* bgNames[4]	= { L"Background", L"Middleground", L"Shadow", L"Foreground" };
	float bgSpeed[4]		= { 0.01f, 0.03f, 0.004f, 0.05f };

	for (int i = 0; i < 4; i++)
	{
		bg = new ScrollObject;
		filePath = mtw("../../resource/background/darkForest_") + bgNames[i] + mtw(".png");
		bg->Create(_device, _immediateContext, filePath, _defaultShader);
		bg->SetRect({ 0, 0, 640, 180 });
		bg->SetScrollSpeed(bgSpeed[i]);
		_backgrounds.push_back(bg);
	}
}

void InGameScreen1::GenerateWorld()
{
	// Generate world
	LoadInfoF("../../resource/sprite/mapColliderInfo1.txt", _colliderInfo);
	LoadInfoF("../../resource/sprite/mapPropsInfo1.txt", _propInfo);
	Generate(_colliderInfo, _colliders, L"../../resource/sprite/map/mapTilesetGrey.png");
	Generate(_propInfo, _props, L"../../resource/sprite/map/propsTilesetGrey.png");

	// Save textures for world
	_colliderTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/mapTileSetGrey.png"));
	_colliderTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/mapTileSetBlue.png"));
	_colliderTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/mapTileSet.png"));
	_propsTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/propsTileSetGrey.png"));
	_propsTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/propsTileSetBlue.png"));
	_propsTextures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/map/propsTileSet.png"));

	// Generate portals
	GeneratePortals("../../resource/sprite/portalInfo1.txt");

	Gem* gem = new Gem;
	gem->Create(_device, _immediateContext,
		L"../../resource/sprite/gem.png",
		_defaultShader
	);
	gem->SetRect({ 64, 0, 18, 20 });
	gem->SetPosition({ -40, -50 });// x: -304 + 48, y: 88 - 32 bit
	gem->SetCameraPos(_cam._pos);
	gem->_colour = Colour::BLUE;
	gem->SetCameraSize(_cam._size);
	_gems.push_back(gem);

	_gameEndIndicator = new StaticObject;
	_gameEndIndicator->Create(_device, _immediateContext,
		L"../../resource/sprite/map/propsTileSetGrey.png",
		_defaultShader
	);
	_gameEndIndicator->SetRect({ 7, 92, 18, 20 });
	_gameEndIndicator->SetPosition({ 440, 50 - 14 });// x: -304 + 48, y: 88 - 32 bit
	_gameEndIndicator->SetCameraPos(_cam._pos);
	_gameEndIndicator->SetCameraSize(_cam._size);
}

void InGameScreen1::GenerateObstacles()
{
	StaticObject* obstacle = new StaticObject;
	obstacle->Create(_device, _immediateContext, L"", _defaultShader);
	obstacle->SetColour({ 1, 0, 0, 1 });
	obstacle->SetRect({ 0, 0, 32, 16 });
	obstacle->SetPosition({ -304 + (24 * 16), -184 + (17 * 16) });
	obstacle->SetCameraPos(_cam._pos);
	obstacle->SetCameraSize(_cam._size);
	_obstacles.push_back(obstacle);
}

void InGameScreen1::GeneratePlayer()
{
	_player = new Player;
	_player->Create();
	_player->SetPosition({ -220, -40 });// x: -304 + 48, y: 88 - 32 bit
	_player->SetDirection({ 0,0 });
	_player->SetCameraPos(_cam._pos);
	_player->SetCameraSize(_cam._size);
	_player->_origin = { -220, -40 };

	_emote = new Emote;
	_emote->Create(_device, _immediateContext,
		L"../../resource/sprite/slime/emotes.png",
		_defaultShader
	);
	_emote->_rects.push_back({ 80, 128, 16, 16 }); // ?
	_emote->_rects.push_back({ 112, 0, 16, 16 }); // À½Ç¥
	_emote->_rects.push_back({ 32, 128, 16, 16 }); // !
	_emote->_show = { false, false, false };
	_emote->SetRect(_emote->_rects[0]);
	_emote->SetPosition({ _player->_pos.x, -_player->_pos.y-8 });
	_emote->SetCameraPos(_cam._pos);
	_emote->SetCameraSize(_cam._size);
}

void InGameScreen1::GenerateGuides()
{
	W_STR fileName[3] = { L"../../resource/sprite/ui/guide1.png",
		L"../../resource/sprite/ui/guide2.png",
		L"../../resource/sprite/ui/guide3.png" };

	for (int i = 0; i < 3; i++)
	{
		PopUpScreen* guide = new PopUpScreen;
		guide->Create(_device, _immediateContext,
			fileName[i],
			_defaultShader
		);
		guide->_textures.push_back(guide->_texture);
		guide->SetRect({ 9, 8, 782, 95 });
		guide->SetPosition({ (992 - 782) / 2, 624 - 95 - 30 });
		_guides.push_back(guide);
	}
}

void InGameScreen1::GenerateDialogs()
{
	PopUpScreen* dialog = new PopUpScreen;
	dialog->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/1-1.png",
		_defaultShader
	);
	dialog->_textures.push_back(dialog->_texture);
	dialog->_textures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/ui/1-2.png"));
	dialog->_textures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/ui/1-3.png"));
	dialog->SetRect({ 15, 9, 452, 92 });
	dialog->SetPosition({ 20, 624 - 92 - 30 });
	_dialogs.push_back(dialog);

	dialog = new PopUpScreen;
	dialog->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/2-1.png",
		_defaultShader
	);
	dialog->_textures.push_back(dialog->_texture);
	dialog->_textures.push_back(S_TexManager.LoadTexture(L"../../resource/sprite/ui/2-2.png"));
	dialog->SetRect({ 15, 9, 452, 92 });
	dialog->SetPosition({ 20, 624 - 92 - 30 });
	_dialogs.push_back(dialog);
}

bool InGameScreen1::Frame()
{
	Scene::Frame();

	if (_gameEnded == true)
	{
		_stageClearPopUp->Frame();
		if (S_Input.GetKey(VK_SPACE) == KEY_DOWN)
		{
			_isNextScene = true;
		}
	}
	
	if ((_guideIdx != -1) || (_dialogIdx != -1))
		_player->_moveAllowed = false;
	else _player->_moveAllowed = true;
	
	if (_guideIdx != -1)
	{
		PopUpScreen* guide = _guides[_guideIdx];
		guide->Frame();
		if (S_Input.GetKey(VK_SPACE) == KEY_DOWN) {
			guide->_texIdx++;
			if (guide->_texIdx >= guide->_textures.size()) {
				if (_guideIdx == 2) _gameEnded = true;
				_guideIdx = -1;
			}
		}
	}

	if (_dialogIdx != -1)
	{
		PopUpScreen* dialog = _dialogs[_dialogIdx];
		dialog->Frame();
		if (S_Input.GetKey(VK_SPACE) == KEY_DOWN) {
			dialog->_texIdx++;
			if (dialog->_texIdx >= dialog->_textures.size()) {
				if (_dialogIdx == 0) _guideIdx = 0;
				else if (_dialogIdx == 1) _guideIdx = 1;
				_dialogIdx = -1;
			}
		}
	}

	if (_controlsPopUp != nullptr)
	{
		_player->_moveAllowed = false;
		_controlsPopUp->Frame();
		if (S_Input.GetKey(VK_SPACE) == KEY_DOWN)
		{
			_tempSpace = _controlsPopUp;
			_controlsPopUp = nullptr;
			_dialogIdx = 0;
			_emote->_show[0] = true;
			_player->_moveAllowed = true;
		}
	}
	
	// Instant death
	for (auto obstacle : _obstacles)
	{
		if (Collision2D::RectToRect(obstacle->_collider, _player->_collider)) {
			GameOver();
		}
	}

	//if (_player->_pos.y > 56) _player->_pos.y = 56;

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
	
	_gameEndIndicator->SetCameraPos(_cam._pos);
	_gameEndIndicator->Frame();

	if (Collision2D::RectToRect(_gameEndIndicator->_collider, _player->_collider)) {
		_emote->_show[2] = true;
		if (_gameEnded == false)
		{
			if (S_Input.GetKey(VK_SPACE) == KEY_DOWN)
			{
				_guideIdx = 2;
			}
		}
	}

	// Portal 
	for (Portal* portal : _portals)
	{
		portal->SetCameraPos(_cam._pos);
		portal->Update();

		if (Collision2D::RectToRect(portal->_collider, _player->_collider)){
			if (_colourCollected[portal->_color] == true)
				_player->SetPosition(portal->_tpPos);
		}
	}
	_emote->Update();

	for (int i = 0; i < _emote->_rects.size(); i++)
	{
		if (_emote->_show[i] == true)
		{
			_emote->SetRect(_emote->_rects[i]);
			_emote->SetCameraPos(_cam._pos);
			_emote->SetPosition({ _player->_pos.x - 8, _player->_pos.y - 33 }, _cam._pos);
			_emote->Frame();
		}
	}

	// Set Gem positions
	for (Gem* gem : _gems)
	{
		gem->SetCameraPos(_cam._pos);
		gem->Frame();

		if (_colourCollected[gem->_colour] == false)
		{
			if (Collision2D::RectToRect(gem->_collider, _player->_collider)) {
				_emote->SetRect(_emote->_rects[2]);
				_emote->_show[2] = true;
				if (S_Input.GetKey(VK_SPACE)) {
					_colourCollected[gem->_colour] = true;
					_player->SetColour(gem->_colour);
					_emote->_show[2] = false;
					_emote->_elapsedTime = 0.0f;
					_emote->SetRect(_emote->_rects[1]);
					_emote->_show[1] = true;
					if (gem->_colour == Colour::BLUE)
					{
						_dialogIdx = 1;
					}
				}
			}
		}
	}

	for (auto obstacle : _obstacles)
	{
		obstacle->SetCameraPos(_cam._pos);
		obstacle->Frame();
	}

	_player->SetCameraPos(_cam._pos);
	_player->Frame();

	_cam.AdjustPos(_player->_pos);
	return true;
}

bool InGameScreen1::Render()
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
		if (_colourCollected[1] == true)
		{
			prop->_immediateContext->PSSetShaderResources(0, 1, &_propsTextures[1]->_textureSRV);
		}
		prop->StaticObject::PostRender();
	}
	for (StaticObject* collider : _colliders)
	{
		collider->StaticObject::PreRender();
		if (_colourCollected[1] == true)
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
	for (Gem* gem : _gems)
	{
		if (_colourCollected[gem->_colour] == false)
			gem->Render();
	}

	_gameEndIndicator->StaticObject::PreRender();
	if (_colourCollected[1] == true)
	{
		_gameEndIndicator->_immediateContext->PSSetShaderResources(0, 1, &_propsTextures[1]->_textureSRV);
	}
	_gameEndIndicator->StaticObject::PostRender();

	_player->Render();
	for (int i = 0; i < _emote->_rects.size(); i++)
	{
		if (_emote->_show[i] == true)
		{
			_emote->Render();
		}
	}

	if (_guideIdx != -1)
	{
		PopUpScreen* guide = _guides[_guideIdx];
		guide->Render();
	}

	if (_dialogIdx != -1)
	{
		PopUpScreen* dialog = _dialogs[_dialogIdx];
		dialog->Render();
	}
	if (_gameEnded == true)
	{
		_stageClearPopUp->Render();
	}
	if (_controlsPopUp != nullptr)
	{
		_controlsPopUp->Render();
	}

	return true;
}

bool InGameScreen1::Release()
{
	_stageClearPopUp->Release();
	delete _stageClearPopUp;

	if (_tempSpace != nullptr)
	{
		_tempSpace->Release();
		delete _tempSpace;
	}
	
	_gameEndIndicator->Release();
	delete _gameEndIndicator;

	for (auto dialog : _dialogs)
	{
		dialog->Release();
	}
	for (auto dialog : _dialogs)
	{
		delete dialog;
	}
	for (auto guide : _guides)
	{
		guide->Release();
	}
	for (auto guide : _guides)
	{
		delete guide;
	}

	for (auto bg : _backgrounds)
	{
		bg->Release();
	}
	for (auto portal : _portals)
	{
		portal->_sprite->Release();
	}
	for (auto portal : _portals)
	{
		delete portal;
	}
	for (StaticObject* gem : _gems)
	{
		gem->Release();
	}
	for (StaticObject* gem : _gems)
	{
		delete gem;
	}
	for (auto prop : _props)
	{
		prop->Release();
	}
	for (auto prop : _props)
	{
		delete prop;
	}
	_props.clear();

	for (auto world : _colliders)
	{
		world->Release();
	}
	for (auto world : _colliders)
	{
		delete world;
	}
	_colliders.clear();
	for (auto obs : _obstacles)
	{
		obs->Release();
	}
	for (auto obs : _obstacles)
	{
		delete obs;
	}
	_player->Release();

	delete _player;

	_player = nullptr;

	for (auto bg : _backgrounds)
	{
		delete bg;
	}
	
	for (auto bg : _backgrounds)
	{
		bg = nullptr;
	}
	_backgrounds.clear();
	_emote->Release();
	delete _emote;

	return true;
}