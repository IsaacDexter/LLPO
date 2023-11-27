#pragma once
#include "Box.h"
#include <array>
#include <thread>

class BoxThread
{
private:
	BoxArray::iterator start;
	BoxArray::iterator end;
	std::thread thread;
	double deltaTime;
	bool update = false;
	bool active = false;
public:
	BoxThread();
	void CreateThread(BoxArray::iterator start, BoxArray::iterator end);
	void StartThread();
	void Update();
	void UpdateThread(const double deltaTime);
};

typedef std::array<BoxThread, THREAD_COUNT> ThreadArray;
