#pragma once
#include <chrono>
#include "FPSCounter.h"

using namespace std::chrono;

extern float g_deltaTime;

namespace DeltaTime
{
    static steady_clock::time_point g_last = steady_clock::now();

	static void UpdateDeltaTime()
	{
        //Recalculate deltaTime
        {
            auto now = steady_clock::now();
            const duration<float> frameTime = now - g_last;
            g_deltaTime = frameTime.count();
            g_last = steady_clock::now();

            FPSCounter::ShowFPS(g_deltaTime);
        }
	}
}

