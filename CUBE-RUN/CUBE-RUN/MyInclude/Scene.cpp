#include "Core.h"
#include "Scene.h"
#include "Cube.h"
#include "Player.h"
#include "RotatingCube.h"
#include "Portal.h"

Scene::Scene(int sceneNum, CameraVectors& cam) :
	m_pPortal{nullptr, nullptr},
	m_pPlane(nullptr),
	m_iSceneNum(sceneNum)
{
	m_tCamera = cam;

	m_pPlane = new Cube("Objs/Cube.obj", glm::vec3(20.0f, 0.1f, 20.0f), glm::vec3(0.0f), glm::vec3(0.0f), "Texture/bg.png");
	m_pPlayer = new Player(1.5f, glm::vec3(0.0f));

	//m_pWall[0] = new Cube("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(-10.0f,5.0f,0.0f), "Texture/bg.png");	// /로/봇이 바라보는 방향 기준 오른쪽 벽
	//m_pWall[1] = new Cube("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(10.0f,5.0f,0.0f), "Texture/bg.png");	// /로/봇이 바라보는 방향 기준 왼쪽 벽
	//m_pWall[2] = new Cube("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f,5.0f, -10.0f), "Texture/bg.png");	// 
	//m_pWall[3] = new Cube("Objs/Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 5.0f, 10.0f), "Texture/bg.png");
	//
	//m_pPortal[0] = new Portal(5.0f, 0, glm::vec3(10.0f, 0.0f, 0.0f));
	//m_pPortal[1] = new Portal(5.0f, 2, glm::vec3(0.0f, 0.0f, 10.0f));
}

Scene::~Scene()
{
	//delete m_pMeshes;
}

void Scene::input()
{
	// player move here
	if (GetAsyncKeyState('W') & 0x8000) Player::input('w');
	if (GetAsyncKeyState('S') & 0x8000) Player::input('s');
	if (GetAsyncKeyState('A') & 0x8000) Player::input('a');
	if (GetAsyncKeyState('D') & 0x8000) Player::input('d');

	if (GetAsyncKeyState('Z') & 0x0001)
	{
		Player::input('z');
		m_tCamera.updatePos(-90.0f, 0);
		CORE->updateViewMat();
	}
	if (GetAsyncKeyState('X') & 0x0001)
	{
		Player::input('x');
		m_tCamera.updatePos(90.0f, 0);
		CORE->updateViewMat();

	}
}

void Scene::update(float frameTime)
{
	// update here
	glm::vec3 foward = m_tCamera.vEYE;
	foward.y = 0;
	foward = glm::normalize(foward);
	Player::setForward(-foward);
	 
	//----------------------------------------------
	// player move
	m_pPlayer->update(frameTime, m_tCamera.getvEYE());
	//m_pPlayer->setRotateByCamera(m_tCamera.getvEYE());
	//----------------------------------------------
	// cubes rotate

	//----------------------------------------------
	// collide check
	// player - floor


	// player - rotating cube


	// player button?

	// player sleeping palyer

	// camera update by player[0]
	//m_tCamera.vAT = m_pPlayer->getTranslateVec();
	//CORE->updateViewMat();

	// set Player Dir to zero
	Player::setDirZero();
}

void Scene::draw(unsigned int shaderNum, int textureBind)
{
	// draw all
	m_pPlane->draw(shaderNum, textureBind);
	m_pPlayer->draw(shaderNum, textureBind);
}

void Scene::drawPortal(unsigned int shaderNum, int textureBind)
{
}

void Scene::activeDragging(bool active, POINT pt)
{
	m_bDragging = active;
	m_tBefPoint = pt;
}

void Scene::moveMouse(POINT pt)
{
	if (!m_bDragging) return;

	// rotate camera
	//printf("%ld %ld \n", m_tBefPoint.x - pt.x, m_tBefPoint.y - pt.y);
	m_tCamera.updatePos(m_tBefPoint.x - pt.x, m_tBefPoint.y - pt.y);
	m_tBefPoint = pt;
	
	// rotate robot foward
}

void Scene::scrollMouse(int dir)
{
	if (dir > 0) m_tCamera.scroll -= m_tCamera.scroll / 10;
	else m_tCamera.scroll += m_tCamera.scroll / 10;
}

glm::mat4 Scene::getPortalView(Portal* from, Portal* dist)
{
	glm::mat4 temp = m_tCamera.getViewMat() * from->getModelTransform();
	glm::mat4 portalCam =
		temp *
		glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::inverse(dist->getModelTransform());

	return portalCam;
}
