#pragma once

#include "game.h"

class GameManager
{
private:
	static GameManager* m_pInst;

public:
	static GameManager* getInst() {
		if (!m_pInst) {
			m_pInst = new GameManager;
		}
		return m_pInst;
	}

	static void DestroyInst() {
		delete m_pInst;
		m_pInst = nullptr;
	}

private:
	GameManager();
	~GameManager();

private:
	int m_iCurSceneNum = 0;
	class Scene* m_pScene;

private:
	// collide check
	bool aabbCollideCheck(const glm::vec3& aMin, const glm::vec3& aMax, const glm::vec3& bMin, const  glm::vec3& bMax);
	bool sphereAndAabbCollideCheck(const glm::vec3& sphere, const float& radius, const glm::vec3& aabbMin, const glm::vec3& aabbMax);
	bool sphereAndSphereCollideCheck(const glm::vec3& aSphere, const float& aRadius, const glm::vec3& bSphere, const float& bRadius);

	void sceneChange(int sceneNum);

public:
	void keyboardCheck(unsigned char key, int x, int y);

public:
	void init();

public:
	void input();
	void update(float deltaTime);
	//void collision();
	void draw(unsigned int shaderNum);


public:
	//void 
};
