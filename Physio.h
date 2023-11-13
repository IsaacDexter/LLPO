#pragma once

#include "stdafx.h"
#include "Scene.h"

using namespace std::chrono;

class Physio
{
private:
	Scene* scene;
	GLFWwindow* m_window = nullptr;
	double m_last = 0.0f;
public:
	Physio();
	~Physio();

	bool Init();
	void MainLoop();

	void Update(const float deltaTime);
	void Draw();

	void Close();

	void OnMouseButtonDown(const int button, const double x, const double y);
	void OnKeyDown(int key);
};

