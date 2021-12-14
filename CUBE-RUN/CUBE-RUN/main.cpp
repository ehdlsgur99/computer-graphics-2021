#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#include "MyInclude/System.h"


#define WINDOWSIZE 800

void main(int argc, char* argv[])
{
	if (!System::GetInstance()->init(argc, argv, WINDOWSIZE, WINDOWSIZE)) exit(100);

	System::GetInstance()->run();
}