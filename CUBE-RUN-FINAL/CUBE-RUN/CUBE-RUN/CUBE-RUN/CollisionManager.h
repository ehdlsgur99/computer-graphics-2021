#pragma once

#include "common.h"

/*
	�浹üũ Ŭ����
*/
class Player;
class Tiles;

class CollisionManager : public CSingleton<CollisionManager>
{
public:
	CollisionManager();
	~CollisionManager();
public:
	int checkCollPlayerCube(Player *player, Tiles *tiles);
};

