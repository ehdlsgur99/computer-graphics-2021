#pragma once

#include "common.h"

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

public:
	Scene* m_pScene;

public:
	bool init(int argc, char* argv[], int sizex, int sizey);
	void run();

public:
	void changeScene(int sceneNum);
	void endProgram();

public:
	void initializeProgram();

public:
	void updateViewMat();
	void updatelightpos(float brightness);
public:
	DepthMap* m_pDepthMap;

public:
	glm::vec3 m_vLightPos;
	glm::vec3 m_vLightColor;

public:

	static void drawScene();
	static void reshape(int w, int h);
	static void mouseAct(int key, int state, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseWheel(int wheel, int dir, int x, int y);
	static void keyboardChecker(unsigned char key, int x, int y);

	static void gameLoop();
};

