// TODO 
// Tidy and label the code
// add memory chunks to the box
// C++ it all
// convert to PS4? (or make this a student task?) - check out the PS4 SDK samples
// rename project etc. 


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
#include "BoxManager.h"
#include "eigen-3.4.0/Eigen/Dense"

using Eigen::Vector3d;

using namespace std::chrono;



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

// used in the 'mouse' tap function to convert a screen point to a point in the world
Vector3d screenToWorld(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return Vector3d((float)posX, (float)posY, (float)posZ);
}


// draw the sides of the containing area
void drawQuad(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, const Vector3d& v4) {
    
    glBegin(GL_QUADS);
    glVertex3f(v1.x(), v1.y(), v1.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
    glVertex3f(v3.x(), v3.y(), v3.z());
    glVertex3f(v4.x(), v4.y(), v4.z());
    glEnd();
}

// draw the entire scene
void drawScene() {

    // Draw the side wall
    GLfloat diffuseMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3d leftSideWallV1(minX, 0.0f, maxZ);
    Vector3d leftSideWallV2(minX, 50.0f, maxZ);
    Vector3d leftSideWallV3(minX, 50.0f, minZ);
    Vector3d leftSideWallV4(minX, 0.0f, minZ);
    drawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3d rightSideWallV1(maxX, 0.0f, maxZ);
    Vector3d rightSideWallV2(maxX, 50.0f, maxZ);
    Vector3d rightSideWallV3(maxX, 50.0f, minZ);
    Vector3d rightSideWallV4(maxX, 0.0f, minZ);
    drawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3d backWallV1(minX, 0.0f, minZ);
    Vector3d backWallV2(minX, 50.0f, minZ);
    Vector3d backWallV3(maxX, 50.0f, minZ);
    Vector3d backWallV4(maxX, 0.0f, minZ);
    drawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

    for (const Box& box : boxes) {
        drawBox(box);
    }
}

// called by GLUT - displays the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    drawScene();

    glutSwapBuffers();
}

// called by GLUT when the cpu is idle - has a timer function you can use for FPS, and updates the physics
// see https://www.opengl.org/resources/libraries/glut/spec3/node63.html#:~:text=glutIdleFunc
// NOTE this may be capped at 60 fps as we are using glutPostRedisplay(). If you want it to go higher than this, maybe a thread will help here. 
void idle() {
    static auto last = steady_clock::now();
    auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    float deltaTime = frameTime.count();

    updatePhysics(deltaTime);

    // tell glut to draw - note this will cap this function at 60 fps
    glutPostRedisplay();
}

// called the mouse button is tapped
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Get the camera position and direction
        Vector3d cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vector3d cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vector3d clickedWorldPos = screenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vector3d rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalise();

        // Perform a ray-box intersection test and remove the clicked box
        size_t clickedBoxIndex = -1;
        float minIntersectionDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < boxes.size(); ++i) {
            if (rayBoxIntersection(cameraPosition, rayDirection, boxes[i])) {
                // Calculate the distance between the camera and the intersected box
                Vector3d diff = boxes[i].position - cameraPosition;
                float distance = diff.length();

                // Update the clicked box index if this box is closer to the camera
                if (distance < minIntersectionDistance) {
                    clickedBoxIndex = i;
                    minIntersectionDistance = distance;
                }
            }
        }

        // Remove the clicked box if any
        if (clickedBoxIndex != -1) {
            boxes.erase(boxes.begin() + clickedBoxIndex);
        }
    }
}

// called when the keyboard is used
void keyboard(unsigned char key, int x, int y) {
    const float impulseMagnitude = 20.0f; // Upward impulse magnitude

    switch (key)
    {
    case ' ':
    {
        for (Box& box : boxes) 
        {
            box.velocity.y() += impulseMagnitude;
        }
        break;
    }
    case 'd':
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

// the main function. 
int main(int argc, char** argv) {

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    initScene(NUMBER_OF_BOXES);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    // it will stick here until the program ends. 
    glutMainLoop();
    return 0;
}
