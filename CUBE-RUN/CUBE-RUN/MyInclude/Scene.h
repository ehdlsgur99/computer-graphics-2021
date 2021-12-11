#pragma once

#include "Game.h"


class Player;
class Mesh;
class Cube;
class Portal;
class Tile;
class Tiles;

class Scene {
private:
	bool m_bDragging = false;
	POINT m_tBefPoint = { 0,0 };
	int m_iSceneNum;

public:
	CameraVectors m_tCamera;

private:
	Cube* m_pPlane;
	Cube* m_pWall[4];
	Portal* m_pPortal[2];

	Tiles* testTiles;

	Player* m_pPlayer;

public:
	Scene() = delete;
	Scene(int sceneNum, CameraVectors& cam);
	~Scene();

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
	glm::mat4 getPortalView(Portal* from, Portal* dist);
};

