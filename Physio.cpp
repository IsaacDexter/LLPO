#include "Physio.h"
#include "BoxManager.h"
#include "callbacks.h"
#include "constants.h"

static BoxManager* g_boxManager = nullptr;

void Physio::Init(int argc, char** argv)
{
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    

    //Initialise GLUT
    bool success = true; 
    glutInit(&argc, argv);
    //init display with bit mask double buffering, RGBA mode window, and a depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    //Create window
    glutInitWindowSize(WINDOW_X, WINDOW_Y);
    glutCreateWindow(WINDOW_TITLE);

    //Set callbacks
    glutKeyboardFunc(key_callback);
    glutMouseFunc(mouse_button_callback);
    glutDisplayFunc(Draw);
    glutIdleFunc(Idle);

    //GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    g_boxManager = new BoxManager();
    g_boxManager->Init();

    glutMainLoop();
}

void Physio::Idle()
{
    //g_boxManager->Update();
}

void Physio::Update()
{
    DeltaTime::UpdateDeltaTime();

    glutPostRedisplay();
}

void Physio::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    g_boxManager->Draw();

    glutSwapBuffers();

}

void Physio::Close()
{
    glutDestroyWindow(glutGetWindow());
    glutLeaveMainLoop();
    exit(EXIT_SUCCESS);
}

void Physio::OnMouseButtonDown(const int button, const int x, const int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        // Get the camera position and direction
        Vector3f cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vector3f cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vector3f clickedWorldPos = g_boxManager->ScreenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vector3f rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalize();

        //selectBox
        g_boxManager->SelectBox(cameraPosition, rayDirection);
    }
}

void Physio::OnKeyDown(const int key)
{
    const Vector3f impulse = Vector3f(0.0f, 20.0f, 0.0f);

    switch (key)
    {
    case ' ':
    {
        g_boxManager->ApplyImpulse(impulse);
        break;
    }
    case 'm':
    {
        DefaultTracker::OutputStats();
        break;
    }
    case 'f':
    {
        FPSCounter::LogFPS();
        break;
    }
    case 'c':
    {
        printf_s("Boxes = %i\n", BOX_COUNT);
        break;
    }
    case 't':
    {
        printf_s("Threads = %i\n", THREAD_COUNT);
        break;
    }
    case 'h':
    {
        DefaultTracker::WalkHeap();
        break;
    }
    case 'p':
    {
        boxPool.Output();
        break;
    }
    case 'n':
    {
        g_boxManager->AddBox();
        break;
    }
    case GLUT_KEY_ESCAPE:    //Escape to exit the game
    {
        Close();
        break;
    }
    default:
    {
        break;
    }
    }
}