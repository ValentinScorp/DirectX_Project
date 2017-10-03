#pragma once

#include <windows.h>

class Clock
{
private:
	LARGE_INTEGER performanceCounter;
	LARGE_INTEGER frequency;
	float elapsedTimeSec;

public:
	Clock();
	~Clock();

	void Update();
	float GetDeltaTime();
};

