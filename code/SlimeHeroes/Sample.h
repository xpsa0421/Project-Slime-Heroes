#pragma once
#include "GameCore.h"
#include "TitleScreen.h"
#include "InGameScreen1.h"
#include "InGameScreen2.h"
#include "IntroScene.h"
extern Scene* _currentScene;

class Sample : public GameCore
{
public:
	TitleScreen*	_titleScreen	= nullptr;
	IntroScene*		_introScreen	= nullptr;
	InGameScreen1*	_inGameScreen1	= nullptr;
	InGameScreen2*	_inGameScreen2	= nullptr;
public:
	virtual bool		Init()		override;
	virtual bool		Frame()		override;
	virtual bool		Render()	override;
	virtual bool		Release()	override;
};

