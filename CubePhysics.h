#pragma once
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "MemoryManager.h"
#include "MemoryTracker.h"

#include "Camera.h"

using namespace std::chrono;

#pragma region ForwardDeclarations

class BoxManager;

#pragma endregion

// these is where the camera is, where it is looking and the bounds of the continaing box. You shouldn't need to alter these

#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0

#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

class CubePhysics
{
public:
	CubePhysics();
	~CubePhysics();
private:
	Vector3d ScreenToWorld(int x, int y);
	void DrawQuad(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, const Vector3d& v4);
	void DrawScene();
	void Init();
	//GLUT FUNCTIONS
	void Display();
	void Idle();
	void Mouse(int button, int state, int x, int y);
	void Keyboard(unsigned char key, int x, int y);


private:
	BoxManager* m_boxes;
	const unsigned int& m_boxCount = 50;
};

