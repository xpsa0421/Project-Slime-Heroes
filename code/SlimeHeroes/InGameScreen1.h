#pragma once
#include "InGameScreen.h"

class InGameScreen1 : public InGameScreen
{
public:
	StaticObject*	_gameEndIndicator = nullptr;
	StaticObject*	_controlsPopUp;
	StaticObject*	_stageClearPopUp;
	StaticObject*	_tempSpace = nullptr;
	Emote*			_emote;
	bool			_gemCollected = false;
	bool			_gameEnded = false;
	bool			_dialog1Shown = false;

private:
	void	GenerateBackground();
	void	GenerateWorld();
	void	GenerateObstacles();
	void	GeneratePlayer();
	void	GenerateDialogs();
	void	GenerateGuides();

public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};
