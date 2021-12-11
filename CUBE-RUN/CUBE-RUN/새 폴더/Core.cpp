#include "Core.h"
#include "GameManager.h"
#include "Timer.h"
#include "ShaderProgram.h"

Core* Core::m_pInst = nullptr;

Core::Core()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(155);
}

Core::~Core()
{

}

bool Core::init(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(m_tWndSize.cx, m_tWndSize.cy);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Game");


	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW!!" << std::endl;
		return false;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW initialized \n" << std::endl;

	// init timer and shaders
	m_pTimer = new Timer();


	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// Callback Functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardChecker);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseAct);
	glutMouseWheelFunc(mouseWheel);
	glutIdleFunc(gameLoop);


	return true;
}

int Core::run()
{
	m_pTimer->resetTimer();
	glutMainLoop();

	return 0;
}

void Core::drawScene()
{
	// shadow
	// shadowShader->use()
	//GameManager::getInst()->draw(shadowShader->getProgram());

	// 
}

void Core::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	Core::getInst()->m_tWndSize = { w,h };
}

void Core::keyboardChecker(unsigned char key, int x, int y)
{
	GameManager::getInst()->keyboardCheck(key, x, y);
}

void Core::mouseAct(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		g_bDragging = true;
		g_iBefX = x;
		g_iBefY = y;
	}
}
