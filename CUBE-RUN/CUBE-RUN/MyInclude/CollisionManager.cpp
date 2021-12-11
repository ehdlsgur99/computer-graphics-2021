#include "CollisionManager.h"
#include "Player.h"
#include "Tiles.h"



CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}
int CollisionManager::checkCollPlayerCube(Player *player, Tiles *tiles)
{
	glm::vec3 playerPos = player->getTranslateVec();
	glm::vec3 cubePos;
	for (int i = 0; i < tiles->tiles.size(); i++)
	{
		Tile* nowTile = tiles->tiles[i];
		for (int j = 0; j < nowTile->cubes.size(); j++)
		{
			// 큐브 충돌
			Cube* nowCube = nowTile->cubes[j];
			cubePos = nowCube->getTranslateVec();
			if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)
				cubePos.y += 2.0f;
			float distance = sqrt(pow(((playerPos.x) - cubePos.x), 2) + pow(((playerPos.y) - cubePos.y), 2) + pow((playerPos.z- cubePos.z), 2));
			if (distance < 1.0f )
			{
				if (nowTile->cubeTypes[j] == eCubeType::eCubeUp)
					return 1;
				if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)
					return 2;

			}
		}
		// 코인 충돌
		for (int j = 0; j < nowTile->coins.size(); j++)
		{
			Cube* nowCoin = nowTile->coins[j];
			cubePos = nowCoin->getTranslateVec();
			float distance = sqrt(pow(((playerPos.x) - cubePos.x), 2) + pow(((playerPos.y) - cubePos.y), 2) + pow((playerPos.z - cubePos.z), 2));
			if (distance < 1.0f)
			{
				delete nowTile->coins[j];
				nowTile->coins.erase(nowTile->coins.begin() + j);
				return 3;
			}
		}
	}
	
}