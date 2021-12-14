#pragma once

#include "Object.h"
#include "Box.h"
#include "Player.h"

enum eScreenState { eGameOver = 0, eGameStart, eGameRun };

class Screen
{
public:
	int nowScreenState;
	Box* gameOverCube;
	Box* gameStartCube;
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

