#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#include "MyInclude/includeThis.h"


#define WINDOWSIZE 800

void main(int argc, char* argv[])
{
	if (!CORE->init(argc, argv, WINDOWSIZE, WINDOWSIZE)) exit(100);

	CORE->run();
}