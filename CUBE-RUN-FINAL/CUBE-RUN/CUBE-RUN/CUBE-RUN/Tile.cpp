#include "common.h"

void Tile::init(eTileType tileType, glm::vec3 startPos,  eCreatePos createPos)
{
	this->tileType = tileType;
	this->startPos = startPos;
	this->createDirection = createPos;

	

	if (tileType == eTileType::eDirect)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
				if(createPos == eCreatePos::eLeft)
					cubePos = glm::vec3(startPos.x - 1.0f * i, startPos.y, startPos.z - 1.0f * j);
				if (createPos == eCreatePos::eRight)
					cubePos = glm::vec3(startPos.x + 1.0f * i, startPos.y, startPos.z - 1.0f * j);
				if(createPos == eCreatePos::eUp)
					cubePos = glm::vec3(startPos.x + 1.0f * j, startPos.y, startPos.z - 1.0f * i);

				Box*cube = new Box("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(cubePos), "Texture/tile.png");
				cubes.push_back(cube);
				cubeTypes.push_back(eCubeNormal);
			}
		}
		// 0 = ÇÑÄ­ Àå¾Ö¹°
		// 1 = °¡·Î µÎÄ­ Àå¾Ö¹° 2 = ¼¼·Î µÎÄ­ Àå¾Ö¹° 3 = °¡·Î ¼¼Ä­ Àå¾Ö¹°
		int random = rand() % 3;

		if (random == 0)
			cubeTypes[rand() % 17] = eCubeType::eCubeUp;
			else if (random == 1)
			cubeTypes[rand() % 17] = eCubeType::eCubeDown;
		//cubes[rand() % 17]->setTranslate(glm::vec3(cubes[rand() % 17]->getTranslateVec().x + 0.0f, cubes[rand() % 17]->getTranslateVec().y -1.0f, cubes[rand() % 17]->getTranslateVec().z + 0.0));
		else if (random == 2)
		{
			cubeTypes[rand() % 17] = eCubeType::eCubeCoin;
		}
		for (int i = 0; i < cubes.size(); i++)
		{
			if (cubeTypes[i] == eCubeUp)
			{
				cubes[i]->setTranslate(glm::vec3(cubes[i]->getTranslateVec().x + 0.0f, cubes[i]->getTranslateVec().y +1.0f, cubes[i]->getTranslateVec().z + 0.0));
			}
			else if (cubeTypes[i] == eCubeDown)
			{
				cubes[i]->setTranslate(glm::vec3(cubes[i]->getTranslateVec().x + 0.0f, cubes[i]->getTranslateVec().y - 1.0f, cubes[i]->getTranslateVec().z + 0.0));
			}
			else if (cubeTypes[i] == eCubeCoin)
			{
				Box* coin = new Box("Objs/cube.obj", glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(glm::vec3(cubes[i]->getTranslateVec().x + 0.0f, cubes[i]->getTranslateVec().y + 1.0f, cubes[i]->getTranslateVec().z + 0.0)), "Texture/300coin.png");
				coins.push_back(coin);
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				glm::vec3 cubePos = glm::vec3(startPos.x + 1.0f * j, startPos.y, startPos.z - 1.0f * i);
				Box* cube = new Box("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(cubePos), "Texture/tile.png");
				cubes.push_back(cube);
				cubeTypes.push_back(eCubeNormal);
			}
		}
	}
}

void Tile::update()
{

}

void Tile::draw(unsigned int shaderNum, int bindTex)
{
	for (int i = 0; i < cubes.size(); i++)
	{
		cubes[i]->draw(shaderNum, bindTex);
	}

	for (int i = 0; i < coins.size(); i++)
	{
		coins[i]->setRotate(glm::vec3(coins[i]->getRotateVec().x, coins[i]->getRotateVec().y + 0.3f, coins[i]->getRotateVec().z));
		coins[i]->draw(shaderNum, bindTex);
	}
}

void Tile::release()
{


}