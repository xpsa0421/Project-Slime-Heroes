#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
	Release();
}

void SpriteManager::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
	_device = device;
	_immediateContext = context;
}

bool SpriteManager::LoadSpriteInfoF(W_STR fileName)
{
	TCHAR srcBuffer[100] = { 0 };
	TCHAR tempBuffer[100] = { 0 };
	TCHAR pathBuffer[100] = { 0 };
	int numSprites = 0;
	
	FILE* file;
	_wfopen_s(&file, fileName.c_str(), _T("rt"));
	if (file == NULL) return false;

	_fgetts(srcBuffer, _countof(srcBuffer), file);
	_stscanf_s(srcBuffer, _T("%s%d"),
		tempBuffer, (unsigned int)_countof(tempBuffer),
		&numSprites);
	_spriteRects.resize(numSprites);

	for (int i = 0; i < numSprites; i++)
	{
		int numFrames = 0;
		_fgetts(srcBuffer, _countof(srcBuffer), file);
		_stscanf_s(srcBuffer, _T("%s%d%s"),
			tempBuffer, (unsigned int)_countof(tempBuffer),
			&numFrames,
			pathBuffer, (unsigned int)_countof(pathBuffer));
		_spriteNames.push_back(tempBuffer);
		_texfileNames.push_back(pathBuffer);

		for (int j = 0; j < numFrames; j++)
		{
			RECT rect;
			_fgetts(srcBuffer, _countof(srcBuffer), file);
			_stscanf_s(srcBuffer, _T("%s%d%d%d%d"), tempBuffer, (unsigned int)_countof(tempBuffer),
				&rect.left, &rect.top, &rect.right, &rect.bottom);
			rect.top += 288;
			_spriteRects[i].push_back(rect);
		}
	}
	fclose(file);
	return true;
}

bool SpriteManager::LoadSprite(W_STR infoFileName)
{
	_spriteNames.clear();
	_spriteRects.clear();

	if (find(_infoFiles.begin(), _infoFiles.end(), infoFileName) != _infoFiles.end())
		return true;
	else if (LoadSpriteInfoF(infoFileName) == false)
		return false;
	else 
		_infoFiles.push_back(infoFileName);

	for (int i = 0; i < _spriteRects.size(); i++)
	{
		Sprite* sprite = Get(_spriteNames[i]);
		if (sprite != nullptr) continue;

		sprite = new Sprite;
		sprite->_name = _spriteNames[i];
		sprite->_rects = _spriteRects[i];

		bool result = sprite->CreateSprite(_device, _immediateContext, _texfileNames[i]);
		if (result)
		{
			//sprite->SetRect(ConvertRect(_spriteRects[i][0]));
			_sprites.insert(std::make_pair(sprite->_name, sprite));
		}
		else return false;
	}
	return true;
}

Sprite* SpriteManager::Get(W_STR spriteName)
{
	auto iter = _sprites.find(spriteName);
	if (iter != _sprites.end())
	{
		return iter->second;
	}
	else return nullptr;
}

bool SpriteManager::Release()
{
	for (auto spriteData : _sprites)
	{
		Sprite* sprite = spriteData.second;
		if (sprite) sprite->Release();
		delete sprite;
	}
	_sprites.clear();
	return true;
}