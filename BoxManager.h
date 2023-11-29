#pragma once
#include "Box.h"
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "stdafx.h"
#include <numeric>
#include <barrier>
#include <functional>

class BoxThread;



class BoxManager
{
private:
	BoxArray* boxes;
	//Pass pointer-to-function type as the template argument, not the actual function
	//Noexcept as barrier requires a callable that is nothrow invocable
	std::barrier<void(*)(void) noexcept>* syncUpdate;
public:
	BoxManager();
	void CreateThreads();
	void StartThreads();

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
	void Update();
	void UpdateScene(BoxArray::iterator start, BoxArray::iterator end, int id);
	void Draw();
	void CheckCollisions();
};

