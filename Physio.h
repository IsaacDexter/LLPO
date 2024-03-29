#pragma once

#include "stdafx.h"


using namespace std::chrono;

class BoxManager;

namespace Physio
{
	void Init(int argc, char** argv);

	void Idle();
	void Update();
	void Draw();

	void Close();

	void OnMouseButtonDown(const int button, const int x, const int y);
	void OnKeyDown(const int key);
}