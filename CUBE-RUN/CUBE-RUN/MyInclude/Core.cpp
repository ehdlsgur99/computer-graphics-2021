#include "Core.h"
#include "Mesh.h"
#include "Timer.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "CubeMap.h"
#include "DepthMap.h"

Single* Single::m_pInst = nullptr;


Single::Single()
{
	m_pScene = nullptr;
}

Single::~Single()
{
	delete m_pScene;
	delete m_pMainShader;
	delete m_pCube;
	delete m_pSkyCube;
}

bool Single::init(int argc, char* argv[], int sizex, int sizey)
{
	CORE->m_tWndSize = { sizex, sizey };

	// Initialize Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(sizex, sizey);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CUBE-RUN");


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

void Single::run()
{
	CORE->m_pTimer = new Timer;
	//m_pTimer->resetTimer();

	glutMainLoop();
}

void Single::changeScene(int sceneNum)
{
	if (!m_pScene) delete m_pScene;

	CameraVectors temp;
	temp.pitch = 1.0f;
	temp.yaw = 0.0f;
	temp.vEYE = glm::vec3(0.0f,2.5f,3.0f);
	temp.vAT = glm::vec3(0.0f,0.0,-1.0f);
	temp.vUP = glm::vec3(0.0f, 1.0f, 0.0f);
	temp.scroll = 11.0f;

	//updateViewMat();
	m_pScene = new Scene(sceneNum, temp);

	if (sceneNum) updateViewMat();
}

void Single::endProgram()
{
	glutLeaveMainLoop();
}

void Single::initializeProgram()
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
	//-------------------------------------------------------------------------------------
	// cube shader
	m_pCube = new Mesh("Objs/Cube.obj", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	m_pSkyCube = new CubeMap("Texture/cube/c");

	m_pCubeShader->use();
	m_pCubeShader->setMat4("projectionTransform", projectionMat);
	m_pCubeShader->setMat4("viewTransform", glm::mat3(viewTransform));
	//-------------------------------------------------------------------------------------
	// shadow shader
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

void Single::updateViewMat()
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

void Single::drawSkyCube()
{
	glDepthFunc(GL_LEQUAL);
	m_pCubeShader->use();

	// bind tex
	m_pSkyCube->bindTexture(0);
	// draw cube
	m_pCube->draw();
	glDepthFunc(GL_LESS);
}

void Single::drawScene()
{
	//--------------------------------------------------------------
	// 1st pass
	glCullFace(GL_FRONT);
	CORE->m_pShadowShader->use();

	CORE->m_pDepthMap->useTexMap();
	CORE->m_pScene->draw(CORE->m_pShadowShader->getProgram(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//--------------------------------------------------------------
	// 2nd pass
	glCullFace(GL_BACK);
	glViewport(0, 0, CORE->m_tWndSize.cx, CORE->m_tWndSize.cy);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw all
	// first should draw portal
	// stensil buffer will do

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 0, 0xFF);
	glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);					// draw 1s when test failed

	CORE->m_pMainShader->use();
	CORE->m_pDepthMap->bindTexture(0);

	// draw stencil pattern
	glClear(GL_STENCIL_BUFFER_BIT);

	// portal draw
	CORE->m_pScene->drawPortal(CORE->m_pMainShader->getProgram(), 1);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// draw all
	CORE->m_pMainShader->use();
	CORE->m_pDepthMap->bindTexture(0);
	CORE->m_pScene->draw(CORE->m_pMainShader->getProgram(), 1);
	//--------------------------------------------------------------
	CORE->drawSkyCube();


	glutSwapBuffers();
}

void Single::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	CORE->m_tWndSize = { w,h };

	// update proj mat
	glm::mat4 projectionMat =
		glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 300.0f);
	CORE->m_pMainShader->setMat4("projectionTransform", projectionMat);
	CORE->m_pCubeShader->setMat4("projectionTransform", projectionMat);

	//-------------------------------------------------------------------------------------
	//// shadow shader
	//glm::mat4 lightView = glm::lookAt(CORE->m_vLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//CORE->m_pShadowShader->setMat4("lightSpace", (projectionMat * lightView));
}

void Single::mouseAct(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON) CORE->m_pScene->activeDragging(state == GLUT_DOWN, { x,y });
}

void Single::mouseMove(int x, int y)
{
	CORE->m_pScene->moveMouse({ x,y });
	CORE->updateViewMat();
}

void Single::mouseWheel(int wheel, int dir, int x, int y)
{
	CORE->m_pScene->scrollMouse(dir);
	CORE->updateViewMat();
}

void Single::keyboardChecker(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

	case 'f':
	case 'F':
		printf("%f\n", 1.0 / CORE->m_pTimer->getDeltaTime());
		break;
	}
}

void Single::gameLoop()
{
	float deltaTime = CORE->m_pTimer->getDeltaTime();

	CORE->m_pScene->input();
	CORE->m_pScene->update(deltaTime);
	CORE->updateViewMat();
	glutPostRedisplay();			// draw

	CORE->m_pTimer->updateDeltaTime();
}
