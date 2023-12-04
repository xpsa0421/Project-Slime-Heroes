#include "IntroScene.h"
#include "Input.h"

bool IntroScene::Init()
{
	_intro = new StaticObject;
	_intro->Create(_device, _immediateContext,
		L"../../resource/background/intro0.png",
		L"../../resource/shader/defaultShader.txt");
	_intro->SetRect({ 0, 0, 992, 624 });
	_intro->SetPosition({ 0,0 });

	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro0.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro1.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro2.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro3.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro4.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro5.png"));
	_introTextures.push_back(S_TexManager.LoadTexture(L"../../resource/background/intro6.png"));

	return true;
}

bool IntroScene::Frame()
{
	Scene::Frame();
	_intro->Frame();

	if (_elapsedTime >= 1.5f)
	{
		_texIdx++;
		_elapsedTime -= 1.5f;
		if (_texIdx >= _introTextures.size()-1)
		{
			_isNextScene = true;
		}
	}
	_elapsedTime += g_secondPerFrame;
	return true;
}

bool IntroScene::Render()
{
	_intro->StaticObject::PreRender();
	_intro->_immediateContext->PSSetShaderResources(0, 1, &_introTextures[_texIdx]->_textureSRV);
	_intro->StaticObject::PostRender();
	return true;
}

bool IntroScene::Release()
{
	_intro->Release();
	delete _intro;
	return true;
}