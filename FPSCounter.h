#pragma once

struct FPSData
{
    //Used to output FPS
    char buffer[64];
    float elapsedTime;
    //How frequently to update the counter
    float interval;
    int frames;
    /// <param name="interval">How often to output the FPS</param>
    FPSData(const float interval)
    {
        elapsedTime = 0.0f;
        this->interval = interval;
        frames = 0;
    }
};

class FPSCounter;

template<class T>
class FPSCounter_template
{
protected:
    static FPSData m_data;
public:
    inline static void ShowFPS(const float deltaTime, GLFWwindow* window)
    {
        m_data.elapsedTime += deltaTime;
        //If the difference is greater than the interval
        m_data.frames++;
        if (m_data.elapsedTime >= m_data.interval)
        {
            //Output FPS and reset the timer
            sprintf_s(m_data.buffer, "FPS = %f\n", m_data.frames / m_data.elapsedTime);
            glfwSetWindowTitle(window, m_data.buffer);
            m_data.elapsedTime = 0.0f;
            m_data.frames = 0;
        }
    }

    //Prevent instanciation
    FPSCounter_template() = delete;
};

FPSData FPSCounter_template<FPSCounter>::m_data(1.0f);
class FPSCounter : public FPSCounter_template<FPSCounter> {};