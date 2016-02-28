#pragma once

#include "IwTween.h"

class Foodwar
{
private:
	IwTween::CTweenManager* zGlobalTweener;

public:
	Foodwar();
	~Foodwar();

	void Init();

	void Run();
	void Update(int delta);

	static void PauseCallback(void* systemData, void* userData);
	static void ResumeCallback(void* systemData, void* userData);
};