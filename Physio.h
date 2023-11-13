#pragma once

#include "stdafx.h"
#include "Scene.h"

double g_last = 0.0f;
GLFWwindow* g_window = nullptr;

using namespace std::chrono;


// this is the number of falling physical items. 

class Physio
{
private:
	Scene* scene;
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

