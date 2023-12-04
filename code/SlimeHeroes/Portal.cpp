#include "Portal.h"

void Portal::Update()
{
	int numFrames = _sprite->_rects.size();
	_elapsedTime += g_secondPerFrame;
	if (_elapsedTime >= 0.2f)
	{
		_index++;
		if (_index >= numFrames)
		{
			_index = 0;
		}
		_sprite->SetRect(ConvertRect(_sprite->_rects[_index]));
		_elapsedTime -= 0.2f;
	}
}

