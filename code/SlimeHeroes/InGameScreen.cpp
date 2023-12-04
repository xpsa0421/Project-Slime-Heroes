#include "InGameScreen.h"

void InGameScreen::Generate(std::vector<std::vector<int>>& worldInfo,
	std::vector<StaticObject*>& world, W_STR texFileName)
{
	if (world.size() != 0) world.clear();
	Vector2 topLeft = { -304, -184 };
	for (int row = 0; row < worldInfo.size(); row++)
	{
		for (int column = 0; column < worldInfo[row].size(); column++)
		{
			int id = worldInfo[row][column];
			if (id == -1) continue;
			else
			{
				StaticObject* mapTile = new StaticObject;
				mapTile->Create(_device, _immediateContext,
					texFileName, _defaultShader);

				// retrieve texture rect position
				int maxCol = mapTile->_imageSize.x / 16;
				float rectY = (id / maxCol) * 16;
				float rectX = (id % maxCol) * 16;

				// set tile attributes
				mapTile->SetRect({ rectX, rectY, 16, 16 });
				mapTile->SetPosition({ topLeft.x + (column * 16), topLeft.y + (row * 16) });
				mapTile->SetCameraPos(_cam._pos);
				mapTile->SetCameraSize(_cam._size);
				world.push_back(mapTile);
			}
		}
	}
}

void InGameScreen::GameOver()
{
	if (_player->_state == DEAD) return;
	_player->SetState(DEAD);
	_player->_moveAllowed = false;
}

void InGameScreen::RestartScene()
{
	_player->SetState(IDLE);
	_player->SetColour(_playerColourInit);
	_player->SetPosition(_player->_origin);
	_player->_moveAllowed = true;
	_guideIdx = -1;
	_dialogIdx = -1;
	for (auto dialog : _dialogs) dialog->Reset();
	for (auto guide : _guides) guide->Reset();
	memcpy(_colourCollected, _colourCollectedInit, sizeof(_colourCollected));
}

void InGameScreen::LoadInfoF(
	STR fileName, std::vector<std::vector<int>>& infoVector)
{
	std::ifstream fp;
	int width = 0, height = 0;

	fp.open(fileName);
	fp >> width >> height;
	infoVector.resize(height, std::vector<int>(width, -1));

	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			fp >> infoVector[row][column];
		}
	}
}

void InGameScreen::GeneratePortals(STR fileName)
{
	int numPortal;
	STR portalType;
	int portalColour;
	Vector2 portalPos;
	Vector2 tpPos;

	std::ifstream fp(fileName);
	if (!fp) return;

	fp >> numPortal;
	for (int i = 0; i < numPortal; i++) {
		std::getline(fp, portalType);
		std::getline(fp, portalType);
		fp >> portalColour;
		fp >> portalPos.x >> portalPos.y;
		fp >> tpPos.x >> tpPos.y;

		Portal* portal = new Portal;

		portal->_sprite = S_SpriteManager.Get(mtw(portalType));
		portal->_sprite->SetRect(ConvertRect(portal->_sprite->_rects[0]));
		portal->_sprite->_updateTime = 0.2f;
		portal->_pos = portalPos;
		portal->_tpPos = tpPos;
		portal->_color = portalColour;
		portal->_sprite->SetCameraPos(_cam._pos);
		portal->_sprite->SetPosition(portalPos, _cam._pos);
		portal->_collider = {
			portalPos.x, portalPos.y - 20,
			portal->_sprite->_rectInit.w,
			portal->_sprite->_rectInit.h / 2
		};
		portal->_sprite->SetCameraSize(_cam._size);
		_portals.push_back(portal);
	}

	fp.close();
}