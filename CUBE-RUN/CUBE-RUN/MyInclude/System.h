#pragma once

#include "Game.h"
#include "Singleton.h"

class Timer;
class Scene;
class ShaderProgram;
class Object;
class CubeMap;
class DepthMap;

class System : public CSingleton<System>
{

public:
	System();
	~System();

public:
	SIZE m_tWndSize;
	Timer* m_pTimer;

public:
	ShaderProgram* m_pMainShader;
	ShaderProgram* m_pCubeShader;
	ShaderProgram* m_pShadowShader;

public:
	Scene* m_pScene;

public:
	// init glut
	bool init(int argc, char* argv[], int sizex, int sizey);
	void run();

public:
	void changeScene(int sceneNum);
	void endProgram();

public:
	// init program
	void initializeProgram();

public:
	void updateViewMat();
	void update_lightpos(float brightness);
	void show_fog();
public:
	Object* m_pCube;
	CubeMap* m_pSkyCube;
	DepthMap* m_pDepthMap;

public:
	void drawSkyCube();

public:
	glm::vec3 m_vLightPos;
	glm::vec3 m_vLightColor;

public:
	// callback func
	static void drawScene();
	static void reshape(int w, int h);
	static void mouseAct(int key, int state, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseWheel(int wheel, int dir, int x, int y);
	static void keyboardChecker(unsigned char key, int x, int y);

	static void gameLoop();

};

