#include "TitleScreen.h"

bool TitleScreen::Init()
{
	_title = new BaseObject;
	_title->Create(_device, _immediateContext,
		L"../../resource/background/title2.png",
		L"../../resource/shader/defaultShader.txt"
	);
	
	_playButton = new Button;
	_playButton->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/playBtn.png",
		L"../../resource/shader/defaultShader.txt"
	);
	_playButton->SetRect({ 0, 0, 180, 63 });
	_playButton->SetPosition({ 20, 445 });

	Texture* texture = S_TexManager.LoadTexture(L"../../resource/sprite/ui/playBtn.png");
	_playButton->_tex = texture;
	_playButton->_texList.push_back(texture);
	texture = S_TexManager.LoadTexture(L"../../resource/sprite/ui/playBtnMouseOver.png");
	_playButton->_texList.push_back(texture);


	_quitButton = new Button;
	_quitButton->Create(_device, _immediateContext,
		L"../../resource/sprite/ui/quitBtn.png",
		L"../../resource/shader/defaultShader.txt"
	);
	_quitButton->SetRect({ 0, 0, 180, 63 });
	_quitButton->SetPosition({ 20, 520 });

	texture = S_TexManager.LoadTexture(L"../../resource/sprite/ui/quitBtn.png");
	_quitButton->_tex = texture;
	_quitButton->_texList.push_back(texture);
	texture = S_TexManager.LoadTexture(L"../../resource/sprite/ui/quitBtnMouseOver.png");
	_quitButton->_texList.push_back(texture);

	_BGMusic = S_SoundManager.LoadSound(L"../../resource/audio/background/The_Adventure_Begins.mp3");
	_BGMusic->StartPlay(true);
	return true;
}

bool TitleScreen::Frame()
{
	_title->Frame();
	_playButton->Frame();
	_quitButton->Frame();

	if (_quitButton->_clicked == true) {
		_gameQuit = true;
	}
	if (_playButton->_clicked == true)
	{
		_gameStart = true;
		_BGMusic->VolumeDown(0.5f);
		_BGMusic->Stop();
	}
	
	return true;
}

bool TitleScreen::Render()
{
	_immediateContext->PSSetSamplers(0, 1, &DxState::g_SSSmooth);
	_title->Render();
	_playButton->Render();
	_quitButton->Render();

	return true;
}

bool TitleScreen::Release()
{
	_title->Release();
	_playButton->Release();
	_quitButton->Release();
	return true;
}
