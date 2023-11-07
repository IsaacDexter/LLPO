// TODO 
// Tidy and label the code
// add memory chunks to the box
// C++ it all
// convert to PS4? (or make this a student task?) - check out the PS4 SDK samples
// rename project etc. 
#include "CubePhysics.h"
#include <GL/glut.h>

CubePhysics* g_application;

void Keyboard(unsigned char key, int x, int y)
{
    g_application->Keyboard(key, x, y);
}

void Mouse(int button, int state, int x, int y)
{
    g_application->Mouse(button, state, x, y);
}

void Display()
{
    g_application->Display();
}

void Idle()
{
    g_application->Idle();
}

// the main function. 
int main(int argc, char** argv) 
{
    g_application = new CubePhysics();

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    g_application->Init();
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

    // it will stick here until the program ends. 
    glutMainLoop();
    return 0;
}
