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
	// 배경음을 출력하는 메서드
	void PlayBg(std::string path);

	// 효과음을 출력하는 메서드
	void PlayEf(std::string path);
};
