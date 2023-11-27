#pragma once
#include "Box.h"
#include <array>
#include <thread>
#include <concurrent_queue.h>
#include "stdafx.h"

class BoxThread
{
private:
	BoxArray::iterator start;
	BoxArray::iterator end;
	std::thread thread;
	bool update;
public:
	BoxThread();
	void Create(BoxArray::iterator start, BoxArray::iterator end);
	void Start();
	void UpdateScene();
	void Update()
	{
		update = true;
	}
};

typedef std::array<BoxThread, THREAD_COUNT> ThreadArray;

class BoxThreadManager
{
private:
	ThreadArray* threads;
public:
	BoxThreadManager();
	void StartThreads();
	void UpdateThreads();
	void CreateThreads(BoxArray* boxes);
};

