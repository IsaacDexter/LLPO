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

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "Box.h"
#include "constants.h"

#if defined(_DEBUG)

#include "MemoryManager.h"
#include "MemoryTracker.h"
#include "FPSCounter.h"

#endif

using Eigen::Vector3f;