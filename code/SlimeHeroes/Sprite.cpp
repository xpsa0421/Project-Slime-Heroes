#include "Sprite.h"
#include <Windows.h>

bool Sprite::CreateSprite(
	ID3D11Device* device, ID3D11DeviceContext* context,	std::wstring texFileName)
{
	bool result = Create(
		device, context,
		texFileName,
		L"../../resource/shader/defaultShader.txt"
	);
	SetRect(ConvertRect(_rects[0]));

	return result;
}

void Sprite::SetPosition(Vector2 pos)
{
	BaseObject::SetPosition({ pos.x - _rectInit.w / 2.0f , _pos.y - _rectInit.h });
	_pos = pos;
}

void Sprite::SetPosition(Vector2 pos, Vector2 cameraPos)
{
	BaseObject::SetPosition({ pos.x - _rectInit.w / 2.0f, _pos.y - _rectInit.h }, cameraPos);
	_pos = pos;
}

void Sprite::ResetFrame()
{
	SetRect(ConvertRect(_rects[0]));
	_elapsedTime = 0.0f;
	_isLoopFinished = false;
	_index = 0;
}

void Sprite::UpdateFrame()
{
	// update sprite image
	if (!_isLooping && _isLoopFinished) return;

	_elapsedTime += g_secondPerFrame;
	if (_elapsedTime >= _updateTime)
	{
		if (++_index == _rects.size())
		{
			// Return to the first sprite if looping
			// Stay at the last sprite if non-looping
			_isLooping ? _index = 0 : _index = _rects.size() - 1;
			if (!_isLooping)
			{
				_isLoopFinished = true;
				return;
			}
		}
		
		// Update rect
		SetRect(ConvertRect(_rects[_index]));
		//std::wstring s = L"UpdateFrame: " + _name + L"Idx[" + std::to_wstring(_index) + L"]\n";
		//OutputDebugStringW(s.c_str());
		_elapsedTime -= _updateTime;
	}
}
