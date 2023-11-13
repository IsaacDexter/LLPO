#pragma once

#include "stdafx.h"

class Scene
{
private:
	// gravity - change it and see what happens (usually negative!)
	const float gravity = -19.81f;
	std::vector<Box> boxes;
public:
	Scene();
	~Scene();

	Vector3f ScreenToWorld(const double x, const double y);
	bool rayBoxIntersection(const Vector3f& rayOrigin, const Vector3f& rayDirection, const Box& box);
	void resolveCollision(Box& a, Box& b);
	bool checkCollision(const Box& a, const Box& b);

	void drawQuad(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4);
	void drawBox(const Box& box);

	void SelectBox(const Vector3f& camPos, const Vector3f& rayDir);

	void ApplyImpulse(const Vector3f& impulse);

	void Init(const int boxCount);
	void Draw();
	void Update(const double deltaTime);
};

