#pragma once
#include "Box.h"
#include <array>
#include <thread>
#include <mutex>
#include <concurrent_queue.h>
#include "stdafx.h"
#include <functional>

class BoxThread
{
private:
	BoxArray::iterator start;
	BoxArray::iterator end;
	std::thread thread;
	std::function<void()>* completeCall;
	bool update;
public:
	BoxThread();
	void Create(BoxArray::iterator start, BoxArray::iterator end, std::function<void()>* completeCall);
	void Start();
	void UpdateScene();
	void Update()
	{
		update = true;
	}
};

typedef std::array<BoxThread, THREAD_COUNT> ThreadArray;

class BoxManager
{
private:
	BoxArray* boxes;
	ThreadArray* threads;
	std::pair<std::mutex, int> completedThreads;
	void UpdateThreads();
public:
	BoxManager();
	void StartThreads();
	void CreateThreads(BoxArray* boxes);
	void MarkUpdateComplete();

	Vector3f ScreenToWorld(const double x, const double y);
	bool rayBoxIntersection(const Vector3f& rayOrigin, const Vector3f& rayDirection, const Box& box);
	void resolveCollision(Box& a, Box& b);
	bool checkCollision(const Box& a, const Box& b);

	void drawQuad(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4);
	void drawBox(const Box& box);
	void drawCube();

	void SelectBox(const Vector3f& camPos, const Vector3f& rayDir);

	void ApplyImpulse(const Vector3f& impulse);

	void Init();
	void Draw();
	void CheckCollisions();
};

