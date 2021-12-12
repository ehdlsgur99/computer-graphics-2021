#include "Screen.h"

Screen::Screen()
{
	nowScreenState = eGameStart;
	screenPos = glm::vec3(0, 0, 0);
	screenPos.y += 4.0f;
}

Screen::~Screen()
{

}

void Screen::init()
{
	gameStartCube = new Cube("Objs/Cube.obj", glm::vec3(10.0f, 5.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), screenPos, "Texture/start.png");
	gameOverCube = new Cube("Objs/Cube.obj", glm::vec3(10.0f, 5.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), screenPos, "Texture/over.png");
}

void Screen::update()
{
	if (nowScreenState == eScreenState::eGameOver)
	{
		glm::vec3 rVec = gameOverCube->getRotateVec();
		gameOverCube->setTranslate(screenPos);
		gameOverCube->setRotate(glm::vec3(rVec.x, rVec.y + 0.5f, rVec.z));
	}
	else if (nowScreenState == eScreenState::eGameStart)
	{
		glm::vec3 rVec = gameStartCube->getRotateVec();
		gameStartCube->setTranslate(screenPos);
		gameStartCube->setRotate(glm::vec3(rVec.x, rVec.y + 0.5f, rVec.z));
	}
}

void Screen::draw(unsigned int shaderNum, int bindTex)
{
	if (nowScreenState == eScreenState::eGameOver)
	{
		gameOverCube->draw(shaderNum, bindTex);
	}
	else if (nowScreenState == eScreenState::eGameStart)
	{
		gameStartCube->draw(shaderNum, bindTex);
	}
}

void Screen::popupGameOver()
{

}

void Screen::popupGameStart()
{

}

void Screen::changeState(int state, Player *player)
{
	nowScreenState = state;
	screenPos = player->getTranslateVec();
	
	if (nowScreenState == eScreenState::eGameOver)
	{
		screenPos.y += 5.0f;
		popupGameOver();
	}
	else if (nowScreenState == eScreenState::eGameStart)
	{
		screenPos.y += 10.0f;
		popupGameStart();
	}
}

