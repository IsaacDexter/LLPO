#pragma once

#include "stdafx.h"


using namespace std::chrono;

class Scene;

namespace Physio
{
	void Start();
	bool Init();
	void MainLoop();

	void Update(const float deltaTime);
	void Draw();

	void Close();

	void OnMouseButtonDown(const int button, const double x, const double y);
	void OnKeyDown(int key);
}