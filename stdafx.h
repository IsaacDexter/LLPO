#pragma once

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
#include <limits>

#include <Dense>

//Prevent windows' min max overrides
#define NOMINMAX
//Prevent bulk chaff
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>
//#include <GL/glut.h>

#include "Box.h"
#include "constants.h"
#include "DeltaTime.h"

#include "MemoryManager.h"
#include "MemoryTracker.h"
#include "FPSCounter.h"

using Eigen::Vector3f;