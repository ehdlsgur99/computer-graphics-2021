#pragma once

#include "Game.h"

class Timer {
private:
	LARGE_INTEGER m_liCountTime;
	LARGE_INTEGER m_liCurTime;
	LARGE_INTEGER m_liPrevTime;

	float m_fDeltaTime;

public:
	Timer();
	~Timer() = default;

public:
	void resetTimer();
	void updateDeltaTime();

	float getDeltaTime();
};

