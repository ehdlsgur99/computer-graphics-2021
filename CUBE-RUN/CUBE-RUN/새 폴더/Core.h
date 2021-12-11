#pragma once

#include "Game.h"

class Timer;
class ShaderProgram;

class Core
{
private:
	static Core* m_pInst;

public:
	static Core* getInst() {
		if (!m_pInst) m_pInst = new Core;
		return m_pInst;
	}

	static void DestroyInst() {
		delete m_pInst;
		m_pInst = nullptr;
	}

private:
	Core();
	~Core();

private:
	Timer* m_pTimer;
	SIZE m_tWndSize;

private:
	ShaderProgram* m_pMainShader;
	ShaderProgram* m_pShadowShader;

//private:
//	FMOD_SYSTEM* m_pSoundSystem;
//
//public:
//	FMOD_SYSTEM* getFmodSystem() { return m_pSoundSystem; }

public:
	bool init(int argc, char* argv[]);
	int run();
	void endGame();

private:
	// callback functions
	// draw
	static void drawScene();

	// reshape
	static void reshape(int w, int h);

	// inputs
	// keyboard
	static void keyboardChecker(unsigned char key, int x, int y);

	// mouse 
	static void mouseAct(int key, int state, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseWheel(int wheel, int dir, int x, int y);

	// idle function
	static void gameLoop();

private:
	void draw();
};