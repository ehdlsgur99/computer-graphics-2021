#include "System.h"
#include "Scene.h"
#include "Box.h"
#include "Player.h"
#include "Portal.h"
#include "Tile.h"
#include "CollisionManager.h"
#include "Tiles.h"
#include "Screen.h"
#include "TextManager.h"

Scene::Scene(int sceneNum, CameraVectors& cam) :
	m_pPortal{ nullptr, nullptr },
	m_pPlane(nullptr),
	m_iSceneNum(sceneNum),
	brightness(1.0f)
{
	m_tCamera = cam;

	m_pPlayer = new Player(1.0f, glm::vec3(0.0f,0.0f,0.0f));

	m_pPlayer = new Player(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	testTiles = new Tiles();
	testTiles->init();
	for (int i = 0; i < 300; i++)
	{
		testTiles->createTile();
	}

	screen = new Screen();
	screen->init();

	isGameStart = false;

	m_pPortal[0] = new Portal(5.0f, 0, glm::vec3(10.0f, 0.0f, 0.0f));
	m_pPortal[1] = new Portal(5.0f, 2, glm::vec3(0.0f, 0.0f, 10.0f));
}

Scene::~Scene()
{
	//delete m_pObjectes;
}

void Scene::input()
{
	// player move here
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pPlayer->input('a');
		glm::vec3 move = m_pPlayer->getTranslateVec();
		m_pPlayer->Player_side_move('a');
	}
	
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pPlayer->input('d');
		glm::vec3 move = m_pPlayer->getTranslateVec();
		m_pPlayer->Player_side_move('d');
	}

	if (GetAsyncKeyState('Z') & 0x0001)
	{
		m_pPlayer->input('z');
		m_tCamera.updatePos(m_pPlayer->angle, 30);
		System::GetInstance()->updateViewMat();
	}
	if (GetAsyncKeyState('X') & 0x0001)
	{
		m_pPlayer->input('x');
		m_tCamera.updatePos(m_pPlayer->angle, 30);
		System::GetInstance()->updateViewMat();

	}
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		m_pPlayer->input(VK_SPACE);
	}
	if (GetAsyncKeyState('K') & 0x0001) brightness += 0.1f;
	if (GetAsyncKeyState('J') & 0x0001) brightness -= 0.1f;
	if (GetAsyncKeyState('M') & 0x0001) m_pPlayer->invincibility = !m_pPlayer->invincibility;

	if (GetAsyncKeyState('C') & 0x8000) testTiles->createTile();
	if (GetAsyncKeyState('P') & 0x8000) m_pPlayer->getCoin();

	if (GetAsyncKeyState('S') & 0x8000)
	{
		screen->changeState(eScreenState::eGameRun, m_pPlayer);
		m_tCamera.updatePos(m_pPlayer->angle, 30.0f);
		System::GetInstance()->updateViewMat();
		isGameStart = true;
	}

	if (GetAsyncKeyState('O') & 0x8000) m_pPlayer->destroyparticle();
	if (GetAsyncKeyState('R') & 0x0001) System::GetInstance()->changeScene(0);
}

void Scene::update(float frameTime)
{
	// update here
	glm::vec3 foward = m_tCamera.vEYE;
	foward.y = 0;
	foward = glm::normalize(foward);
	Player::setForward(-foward);
	
	//player and camera move
	if (isGameStart)
	{
		m_pPlayer->update(frameTime);
		if (m_pPlayer->life) m_tCamera.setTarget(m_pPlayer->getTranslateVec());
		System::GetInstance()->update_lightpos(brightness);

		if (!start_update_viewmat)
		{
			start_update_viewmat = true;
			m_pPlayer->input('x');
			m_tCamera.updatePos(m_pPlayer->angle, 30);
			System::GetInstance()->updateViewMat();
		}
	}
	int checkCollision = 0;
	checkCollision = CollisionManager::GetInstance()->checkCollPlayerCube(m_pPlayer, testTiles);
	if (checkCollision == 1 || checkCollision == 2)
	{
		screen->changeState(eScreenState::eGameOver, m_pPlayer);
	}

	ParticleManager::GetInstance()->Update(0.1f);
	screen->update();
	Player::setDirZero();
}

void Scene::draw(unsigned int shaderNum, int textureBind)
{
	m_pPlayer->draw(shaderNum, textureBind);

	glColor3f(1, 0, 0);
	glRasterPos3f(0.0, 0.0, 0.0);
	TextManager::GetInstance()->glPrint("test");

	testTiles->draw(shaderNum, textureBind);
	
	screen->draw(shaderNum, textureBind);

	ParticleManager::GetInstance()->Draw(90.0f, 36, shaderNum, textureBind);


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


glm::vec3 Scene::get_player_pos()
{
	return m_pPlayer->getTranslateVec();
}
