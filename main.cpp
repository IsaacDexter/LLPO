// TODO 
// Tidy and label the code
// add memory chunks to the box
// C++ it all
// convert to PS4? (or make this a student task?) - check out the PS4 SDK samples
// rename project etc. 
#include "CubePhysics.h"
#include <GL/glut.h>

// the main function. 
int main(int argc, char** argv) 
{
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    CubePhysics();
    return 0;
}
