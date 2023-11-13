#pragma once

#include "stdafx.h"

void initScene(int boxCount);

Vector3f screenToWorld(int x, int y);

bool rayBoxIntersection(const Vector3f& rayOrigin, const Vector3f& rayDirection, const Box& box);
void resolveCollision(Box& a, Box& b);
bool checkCollision(const Box& a, const Box& b);
void updatePhysics(const float deltaTime);

void drawQuad(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4);
void drawBox(const Box& box);
void drawScene();

void display();
void idle();

void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv);

bool Init();
void MainLoop(GLFWwindow* window);
void Close(GLFWwindow* window);