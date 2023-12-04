#pragma once
#include "Scene.h"
#include "StaticObject.h"

class IntroScene : public Scene
{
	std::vector<Texture*> _introTextures;
	StaticObject* _intro;
	int	_texIdx = 0;
	float _elapsedTime = 0.0f;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

