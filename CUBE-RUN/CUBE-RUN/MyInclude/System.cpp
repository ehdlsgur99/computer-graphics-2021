#include "System.h"
#include "Object.h"
#include "Timer.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "CubeMap.h"
#include "DepthMap.h"
#include "TextManager.h"


System::System()
{
	m_pScene = nullptr;
}

System::~System()
{
	delete m_pScene;
	delete m_pMainShader;
	delete m_pCube;
	delete m_pSkyCube;
}

bool System::init(int argc, char* argv[], int sizex, int sizey)
{
	System::GetInstance()->m_tWndSize = { sizex, sizey };

	// Initialize Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(sizex, sizey);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CUBE-RUN");

	TextManager::GetInstance()->InitFont();
	

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW!!" << std::endl;
		return false;
		//exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW initialized \n" << std::endl;

	// init
	initializeProgram();

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Callback Functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardChecker);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseAct);
	glutMouseWheelFunc(mouseWheel);

	glutIdleFunc(gameLoop);

	// init completed
	return true;
}

void System::run()
{
	System::GetInstance()->m_pTimer = new Timer;
	//m_pTimer->resetTimer();

	glutMainLoop();
}

void System::changeScene(int sceneNum)
{
	if (!m_pScene) delete m_pScene;

	CameraVectors temp;
	temp.pitch = 30.0f;
	temp.yaw = 90.0f;
	temp.vEYE = glm::vec3(0.0f,2.5f,3.0f);
	temp.vAT = glm::vec3(0.0f,0.0,-1.0f);
	temp.vUP = glm::vec3(0.0f, 1.0f, 0.0f);
	temp.scroll = 25.0f;

	m_pScene = new Scene(sceneNum, temp);

	if (sceneNum) updateViewMat();
}

void System::endProgram()
{
	glutLeaveMainLoop();
}

void System::initializeProgram()
{
	// init scene
	changeScene(0);

	// init shaders
	m_pMainShader = new ShaderProgram("Shader/vertex_1.glsl", "Shader/fragment_1.glsl");
	m_pShadowShader = new ShaderProgram("Shader/1pass_vertex.glsl","Shader/1pass_fragment.glsl");
	m_pCubeShader = new ShaderProgram("Shader/vertex_cube.glsl", "Shader/fragment_cube.glsl");
	// init mainshader transform
	//-------------------------------------------------------------------------------------
	// main shader
	m_pMainShader->use();

	// proj mat
	glm::mat4 projectionMat =
		glm::perspective(glm::radians(90.0f), float(m_tWndSize.cx) / float(m_tWndSize.cy), 0.1f, 300.0f);
	m_pMainShader->setMat4("projectionTransform", projectionMat);

	// view mat
	glm::mat4 viewTransform = m_pScene->m_tCamera.getViewMat();
	m_pMainShader->setMat4("viewTransform", viewTransform);

	// light pos;
	glm::vec3 viewPos = m_pScene->m_tCamera.vEYE;
	m_vLightPos = glm::vec3(25.0f);
	m_vLightColor = glm::vec3(1.0f);

	m_pMainShader->setVec3("viewPos", viewPos);
	m_pMainShader->setVec3("lightPos", m_vLightPos);
	m_pMainShader->setVec3("lightColor", m_vLightColor);

	m_pCubeShader->use();
	m_pCubeShader->setMat4("projectionTransform", projectionMat);
	m_pCubeShader->setMat4("viewTransform", glm::mat3(viewTransform));

	m_pShadowShader->use();
	glm::mat4 shdwProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 300.0f);
	glm::mat4 lightView = glm::lookAt(m_vLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pShadowShader->setMat4("lightSpace", (shdwProj * lightView));
	//-------------------------------------------------------------------------------------
	m_pMainShader->use();
	m_pMainShader->setMat4("lightVP", (shdwProj * lightView));

	m_pDepthMap = new DepthMap(4096, 4096);

	m_pMainShader->setInt("shadowMap", 0);
	m_pMainShader->setInt("texture0", 1);

}

void System::updateViewMat()
{
	glm::mat4 viewTransform = m_pScene->m_tCamera.getViewMat();

	// view mat
	// main
	m_pMainShader->use();
	m_pMainShader->setMat4("viewTransform", viewTransform);
	m_pMainShader->setVec3("viewPos", m_pScene->m_tCamera.vEYE * m_pScene->m_tCamera.scroll);

	// cube
	m_pCubeShader->use();
	m_pCubeShader->setMat4("viewTransform", glm::mat3(viewTransform));

}


void System::drawScene()
{
	//--------------------------------------------------------------
	// 1st pass
	glCullFace(GL_FRONT);
	System::GetInstance()->m_pShadowShader->use();

	System::GetInstance()->m_pDepthMap->useTexMap();
	System::GetInstance()->m_pScene->draw(System::GetInstance()->m_pShadowShader->getProgram(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//--------------------------------------------------------------
	// 2nd pass
	glCullFace(GL_BACK);
	glViewport(0, 0, System::GetInstance()->m_tWndSize.cx, System::GetInstance()->m_tWndSize.cy);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw all
	// first should draw portal
	// stensil buffer will do

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 0, 0xFF);
	glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);					// draw 1s when test failed

	System::GetInstance()->m_pMainShader->use();
	System::GetInstance()->m_pDepthMap->bindTexture(0);

	// draw stencil pattern
	glClear(GL_STENCIL_BUFFER_BIT);

	// portal draw
	System::GetInstance()->m_pScene->drawPortal(System::GetInstance()->m_pMainShader->getProgram(), 1);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// draw all
	System::GetInstance()->m_pMainShader->use();
	System::GetInstance()->m_pDepthMap->bindTexture(0);
	System::GetInstance()->m_pScene->draw(System::GetInstance()->m_pMainShader->getProgram(), 1);

	glutSwapBuffers();
}

void System::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	System::GetInstance()->m_tWndSize = { w,h };

	// update proj mat
	glm::mat4 projectionMat =
		glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 300.0f);
	System::GetInstance()->m_pMainShader->setMat4("projectionTransform", projectionMat);
	System::GetInstance()->m_pCubeShader->setMat4("projectionTransform", projectionMat);

	//-------------------------------------------------------------------------------------
	//// shadow shader
	//glm::mat4 lightView = glm::lookAt(CORE->m_vLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//CORE->m_pShadowShader->setMat4("lightSpace", (projectionMat * lightView));
}

void System::mouseAct(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON) System::GetInstance()->m_pScene->activeDragging(state == GLUT_DOWN, { x,y });
}

void System::mouseMove(int x, int y)
{
	System::GetInstance()->m_pScene->moveMouse({ x,y });
	System::GetInstance()->updateViewMat();


}

void System::mouseWheel(int wheel, int dir, int x, int y)
{
	System::GetInstance()->m_pScene->scrollMouse(dir);
	System::GetInstance()->updateViewMat();
}

void System::keyboardChecker(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

	case 'f':
	case 'F':
		printf("%f\n", 1.0 / System::GetInstance()->m_pTimer->getDeltaTime());
		break;
	}
}

void System::gameLoop()
{
	float deltaTime = System::GetInstance()->m_pTimer->getDeltaTime();

	System::GetInstance()->m_pScene->input();
	System::GetInstance()->m_pScene->update(deltaTime);
	System::GetInstance()->updateViewMat();
	glutPostRedisplay();			// draw

	System::GetInstance()->m_pTimer->updateDeltaTime();
}


void System::update_lightpos(float brightness)
{
	glm::vec3 viewPos = m_pScene->m_tCamera.vEYE;
	m_vLightPos = glm::vec3(glm::vec3(m_pScene->get_player_pos().x, m_pScene->get_player_pos().y + 3.0f, m_pScene->get_player_pos().z));
	m_vLightColor = glm::vec3(brightness);

	m_pMainShader->setVec3("viewPos", viewPos);
	m_pMainShader->setVec3("lightPos", m_vLightPos);
	m_pMainShader->setVec3("lightColor", m_vLightColor);

	glutPostRedisplay();
}

void System::show_fog()
{
	GLfloat fogColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glFogi(GL_FOG_MODE, GL_LINEAR); // <1>
	glFogfv(GL_FOG_COLOR, fogColor); // <2>
	glFogf(GL_FOG_DENSITY, 0.7f); // <3>
	glHint(GL_FOG_HINT, GL_NICEST); // <4>
	glFogf(GL_FOG_START, 0.0f); // <5>
	glFogf(GL_FOG_END, 4.0f); // <6>
	glEnable(GL_FOG); // <7>
}