#include "Tiles.h"

void Tiles::init()
{
	Tile *testTile = new Tile();
	testTile->init(eTileType::eDirect, glm::vec3(0, 0, 0), eCreatePos::eUp);
	tiles.push_back(testTile);
}
void Tiles::update()
{

}
void Tiles::draw(unsigned int shaderNum, int bindTex)
{
	for(int i= 0 ; i<tiles.size(); i++ )
	{
		tiles[i]->draw(shaderNum, bindTex);
	}
}
void Tiles::release()
{

}

void Tiles::createTile()
{
	// ������ Ÿ���� turn ���� direct ���� Ȯ��
	Tile* nowTile = tiles[tiles.size() - 1];
	glm::vec3 startPos = nowTile->startPos;
	// ���� Ÿ���� direct Ÿ�� �̾��ٸ�.. �̹����� ���� �ϳ��� ����
	Tile *nextTile = new Tile();
	if (nowTile->tileType == eTileType::eDirect)
	{
		// set object color
		int random = rand() % 3;

		if (nowTile->createDirection == eCreatePos::eRight)
		{
			startPos.x += 6.0f;
		}
		if (nowTile->createDirection == eCreatePos::eLeft)
		{
			startPos.x -= 6.0f;
		}
		if (nowTile->createDirection == eCreatePos::eUp)
		{
			startPos.z -= 6.0f;
		}
		nextTile->init((eTileType)random, startPos, eCreatePos::eUp);
	}
	// ���� Ÿ���� left turn Ÿ�� �̾��ٸ�..�̹����� direct
	if (nowTile->tileType == eTileType::eLeftTurn)
	{
		startPos.x -= 1.0f;
		nextTile->init(eTileType::eDirect, startPos, eCreatePos::eLeft);
	}
	// ���� Ÿ���� right turn Ÿ�� �̾��ٸ�..�̹����� direct
	else if (nowTile->tileType == eTileType::eRightTurn)
	{
		startPos.x += 3.0f;

		nextTile->init(eTileType::eDirect, startPos, eCreatePos::eRight);
	}
	tiles.push_back(nextTile);

}



