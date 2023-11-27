#pragma once

#include "stdafx.h"
#include "Box.h"
#include "BoxThreadManager.h"

#define GRAVITY -19.81f

class Scene
{
private:
	BoxArray* boxes;
public:
	Scene();
	~Scene();

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
	void Update();
	//void UpdateSection(const double deltaTime, BoxArray::iterator start, BoxArray::iterator end);
};

