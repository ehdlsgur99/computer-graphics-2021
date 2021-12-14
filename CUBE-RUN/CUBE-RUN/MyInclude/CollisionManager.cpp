#include "CollisionManager.h"
#include "Player.h"
#include "Tiles.h"


CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

// return 1 : Ƣ��� ť��
// return 2 : ���� ���� ť��
// return 3 : ����
// return 4 : ���
int CollisionManager::checkCollPlayerCube(Player *player, Tiles *tiles)
{
	glm::vec3 playerPos = player->getTranslateVec();
	glm::vec3 cubePos;
	

	bool out = true;
	for (int i = 0; i < tiles->tiles.size(); i++)
	{
		
		Tile* nowTile = tiles->tiles[i];
		for (int j = 0; j < nowTile->cubes.size(); j++)
		{
			// ť�� �浹
			
			Box* nowCube = nowTile->cubes[j];
			cubePos = nowCube->getTranslateVec();
			if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)
				cubePos.y += 2.0f;
			float distance = sqrt(pow(((playerPos.x) - cubePos.x), 2) + pow(((playerPos.y) - cubePos.y), 2) + pow((playerPos.z- cubePos.z), 2));
			
			if (distance < 2.5f)
			{
				if (nowTile->cubeTypes[j] == eCubeType::eCubeUp)		out = false;
				if (nowTile->cubeTypes[j] == eCubeType::eCubeDown)		out = false;
				if (nowTile->cubeTypes[j] == eCubeType::eCubeNormal)	out = false;


			}

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
		
		if (out && player->isjump == false)
		{
			printf("123\n");
			player->collision(0); // Down
			return 2;
		}

		// ���� �浹
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