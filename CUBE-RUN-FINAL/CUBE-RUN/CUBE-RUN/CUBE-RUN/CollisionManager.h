#pragma once

#include "common.h"

/*
	충돌체크 클래스
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

