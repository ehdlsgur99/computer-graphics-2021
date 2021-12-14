#include "common.h"


CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

// return 1 : 튀어나온 큐브
// return 2 : 땅에 꺼진 큐브
// return 3 : 코인
// return 4 : 장외
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
			Box* nowCube = nowTile->cubes[j];
			cubePos = nowCube->getTranslateVec();
			if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)
				cubePos.y += 2.0f;
			float distance = sqrt(pow(((playerPos.x) - cubePos.x), 2) + pow(((playerPos.y) - cubePos.y), 2) + pow((playerPos.z- cubePos.z), 2));
			
			if (distance < 0.5f )
			{
				if (nowTile->cubeTypes[j] == eCubeType::eCubeUp)
				{
					ParticleManager::GetInstance()->createParticle(player->getTranslateVec(), 1); // 1 = PlayerDestroyParticle
					player->collision(1);
					return 1;
				}
				if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)
				{
					player->collision(0); // Down
					return 2;
				}
				
			}
		}
		

		// 코인 충돌
		for (int j = 0; j < nowTile->coins.size(); j++)
		{
			Box* nowCoin = nowTile->coins[j];
			cubePos = nowCoin->getTranslateVec();
			float distance = sqrt(pow(((playerPos.x) - cubePos.x), 2) + pow(((playerPos.y) - cubePos.y), 2) + pow((playerPos.z - cubePos.z), 2));
			if (distance < 1.0f)
			{
				ParticleManager::GetInstance()->createParticle(nowTile->coins[j]->getTranslateVec(),0); // 0 = coin particle
				delete nowTile->coins[j];
				nowTile->coins.erase(nowTile->coins.begin() + j);
				return 3;
			}
		}
	}
	
}