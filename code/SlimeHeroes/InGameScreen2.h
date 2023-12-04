#pragma once
#include "InGameScreen.h"

class InGameScreen2 : public InGameScreen
{
public:
	StaticObject*	_house;
	StaticObject*	_alertPopUp;
	bool			_gemCollected = false;
	bool			_moveAllowed = false;
	bool			_gameEnded = false;
	bool			_colourCollectedInit[4] = { 1,1,0,0 };
	Colour			_playerColourInit = BLUE;
	bool			_showAlert = false;
private:
	void	GenerateBackground();
	void	GenerateWorld();
	void	GeneratePlayer();
	void	GenerateDialogs();
	void	GenerateGuides();

public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};
