#pragma once

#include "Game.h"

// main singleton

class Timer;
class Scene;
class ShaderProgram;
class Mesh;
class CubeMap;
class DepthMap;

class Single {
private:
	static Single* m_pInst;

public:
	static Single* getInst() {
		if (!m_pInst) m_pInst = new Single;
		return m_pInst;
	}

private:
	Single();
	~Single();

private:
	SIZE m_tWndSize;
	Timer* m_pTimer;

private:
	ShaderProgram* m_pMainShader;
	ShaderProgram* m_pCubeShader;
	ShaderProgram* m_pShadowShader;

private:
	Scene* m_pScene;

public:
	// init glut
	bool init(int argc, char* argv[], int sizex, int sizey);
	void run();

public:
	void changeScene(int sceneNum);
	void endProgram();

private:
	// init program
	void initializeProgram();

public:
	void updateViewMat();

private:
	Mesh* m_pCube;
	CubeMap* m_pSkyCube;
	DepthMap* m_pDepthMap;

private:
	void drawSkyCube();

private:
	glm::vec3 m_vLightPos;
	glm::vec3 m_vLightColor;

private:
	// callback func
	static void drawScene();
	static void reshape(int w, int h);
	static void mouseAct(int key, int state, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseWheel(int wheel, int dir, int x, int y);
	static void keyboardChecker(unsigned char key, int x, int y);

	static void gameLoop();

};

