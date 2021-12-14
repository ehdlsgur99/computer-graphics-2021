#include "System.h"


void main(int argc, char* argv[])
{
	if (!System::GetInstance()->init(argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT)) exit(100);

	System::GetInstance()->run();
}