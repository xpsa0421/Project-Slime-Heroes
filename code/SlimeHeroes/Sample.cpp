#include "Sample.h"
Scene* _currentScene;

bool Sample::Init()
{
	S_SpriteManager.SetDevice(_device, _immediateContext);
	S_SpriteManager.LoadSprite(L"../../resource/sprite/spriteInfo.txt");

	_titleScreen = new TitleScreen;
	_introScreen = new IntroScene;
	_inGameScreen1 = new InGameScreen1;
	_inGameScreen2 = new InGameScreen2;
	_titleScreen->SetDevice(_device, _immediateContext);
	_introScreen->SetDevice(_device, _immediateContext);
	_inGameScreen1->SetDevice(_device, _immediateContext);
	_inGameScreen2->SetDevice(_device, _immediateContext);
	_titleScreen->Init();
	_introScreen->Init();
	_inGameScreen1->Init();
	_currentScene = _titleScreen;
	return true;
}

bool Sample::Frame()
{
	if (_currentScene->IsNextScene())
	{
		if (_currentScene == _titleScreen)
		{
			_currentScene = _introScreen;
		}
		else if (_currentScene == _introScreen)
		{
			_currentScene = _inGameScreen1;
			_currentScene->_BGMusic->StartPlay(true);
		}
		else if (_currentScene == _inGameScreen1)
		{
			_inGameScreen2->Init();
			_currentScene = _inGameScreen2;
		}
	}
	if (_currentScene->IsGameQuit())
		_isGameRun = false;

	_currentScene->Frame();
	return true;
}

bool Sample::Render()
{
	D3D11_VIEWPORT vp;
	vp.Width = 992;
	vp.Height = 624;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_immediateContext->RSSetViewports(1, &vp);

	_currentScene->Render();
	return true;
}

bool Sample::Release()
{
	_titleScreen->Release();
	_introScreen->Release();
	_inGameScreen1->Release();
	if (_currentScene == _inGameScreen2) _inGameScreen2->Release();
	delete _titleScreen;
	delete _introScreen;
	delete _inGameScreen1;
	delete _inGameScreen2;
	return true;
}


GAME_RUN(Slime Heroes, 992, 624)