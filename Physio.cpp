#include "Physio.h"

Physio::Physio()
{
    scene = nullptr;

    bool success;
    success = Init();
    if (!success)
    {
        exit(EXIT_FAILURE);
    }

    MainLoop();

    Close();
}

Physio::~Physio()
{
}

bool Physio::Init()
{
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    //Initialise GLFW
    bool success = true;
    if (!glfwInit())
    {
        //Initialization failed
        success = false;
        error_callback(1, "GLFW initialization failed.\n");
    }
    //Create window, returning handle to created window and context object
    m_window = glfwCreateWindow(WINDOW_X, WINDOW_Y, WINDOW_TITLE, NULL, NULL);
    if (!m_window)
    {
        //Window or OpenGL context creation failed
        success = false;
        error_callback(2, "Window or OpenGL context creation failed.\n");
    }
    //Make openGL current context
    glfwMakeContextCurrent(m_window);
    //Set callbacks
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);

    //retrieve framebuffer size for glViewport
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    //GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    scene = new Scene();
    scene->Init(NUMBER_OF_BOXES);

    return success;
}

void Physio::MainLoop()
{
    //Until the window needs to close
    while (!glfwWindowShouldClose(m_window))
    {
        double time = glfwGetTime();
        double deltaTime = time - m_last;
        m_last = time;

        FPSCounter::ShowFPS(deltaTime, m_window);

        Update(deltaTime);
        Draw();
        //Poll input
        glfwPollEvents();
    }
}

void Physio::Update(const float deltaTime)
{
    scene->Update(deltaTime);
}

void Physio::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    scene->Draw();

    glfwSwapBuffers(m_window);
}

void Physio::Close()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Physio::OnMouseButtonDown(const int button, const double x, const double y)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        // Get the camera position and direction
        Vector3f cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vector3f cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vector3f clickedWorldPos = scene->ScreenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vector3f rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalize();

        //selectBox
    }
}

void Physio::OnKeyDown(int key)
{
    const Vector3f impulse = Vector3f(0.0f, 20.0f, 0.0f);

    switch (key)
    {
    case GLFW_KEY_SPACE:
    {
        scene->ApplyImpulse(impulse);
        break;
    }
    case GLFW_KEY_M:
    {
        DefaultTracker::OutputStats();
        break;
    }
    default:
    {
        break;
    }
    }
}
