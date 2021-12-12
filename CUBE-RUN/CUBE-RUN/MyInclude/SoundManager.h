#pragma once

#include "Game.h"
#include "Singleton.h"
#include <Mmsystem.h>
#include <cstring>

#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

class SoundManager : public CSingleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();
public:
	// ������� ����ϴ� �޼���
	void PlayBg(std::string path);

	// ȿ������ ����ϴ� �޼���
	void PlayEf(std::string path);
};
