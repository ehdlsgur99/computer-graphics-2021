#pragma once

#include "common.h"


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
