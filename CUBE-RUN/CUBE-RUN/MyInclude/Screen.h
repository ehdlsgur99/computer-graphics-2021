#pragma once

#include "Mesh.h"
#include "Cube.h"
#include "Player.h"

enum eScreenState { eGameOver = 0, eGameStart, eGameRun };

class Screen
{
public:
	int nowScreenState;
	Cube* gameOverCube;
	Cube* gameStartCube;
	glm::vec3 screenPos;
	float time;
public:
	Screen();
	~Screen();
	void init();
	void update();
	void draw(unsigned int shaderNum, int bindTex);
	void popupGameOver();
	void popupGameStart();
	void changeState(int state, Player *player);
};

