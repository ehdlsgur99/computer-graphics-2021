#pragma once

#include "common.h"


class Player;
class Object;
class Box;
class Tile;
class Tiles;
class Screen;

class Scene {
private:
	bool m_bDragging = false;
	POINT m_tBefPoint = { 0,0 };
	int m_iSceneNum;
	bool start_update_viewmat = false;
	float brightness;
public:
	CameraVectors m_tCamera;
	Player* m_pPlayer;
private:
	Box* m_pPlane;

	Tiles* testTiles;
	Screen* screen;

	bool isGameStart;
	

public:
	Scene() = delete;
	Scene(int sceneNum, CameraVectors& cam);
	~Scene();
	glm::vec3 get_player_pos();
public:
	void input();
	void update(float frameTime);
	void draw(unsigned int shaderNum, int textureBind);

	void drawPortal(unsigned int shaderNum, int textureBind);
	
public:
	// mouseAct
	void activeDragging(bool active, POINT pt);
	// movesMove
	void moveMouse(POINT pt);
	// mouseWheel
	void scrollMouse(int dir);

public:
};

