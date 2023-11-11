#include "CubePhysics.h"

CubePhysics::CubePhysics()
{
    m_boxes = new BoxManager();
}

CubePhysics::~CubePhysics()
{
}

// used in the 'mouse' tap function to convert a screen point to a point in the world
fvec3 CubePhysics::ScreenToWorld(int x, int y)
{
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

    return fvec3((float)posX, (float)posY, (float)posZ);
}

//Draw the sides of the containing area
void CubePhysics::DrawQuad(const fvec3& v1, const fvec3& v2, const fvec3& v3, const fvec3& v4)
{
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}

// draw the entire scene
void CubePhysics::DrawScene()
{
    // Draw the side wall
    GLfloat diffuseMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    fvec3 leftSideWallV1(minX, 0.0f, maxZ);
    fvec3 leftSideWallV2(minX, 50.0f, maxZ);
    fvec3 leftSideWallV3(minX, 50.0f, minZ);
    fvec3 leftSideWallV4(minX, 0.0f, minZ);
    DrawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    fvec3 rightSideWallV1(maxX, 0.0f, maxZ);
    fvec3 rightSideWallV2(maxX, 50.0f, maxZ);
    fvec3 rightSideWallV3(maxX, 50.0f, minZ);
    fvec3 rightSideWallV4(maxX, 0.0f, minZ);
    DrawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    fvec3 backWallV1(minX, 0.0f, minZ);
    fvec3 backWallV2(minX, 50.0f, minZ);
    fvec3 backWallV3(maxX, 50.0f, minZ);
    fvec3 backWallV4(maxX, 0.0f, minZ);
    DrawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

    m_boxes->Draw();
}

void CubePhysics::Init()
{
    m_boxes->Init(m_boxCount);
    
}

void CubePhysics::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    DrawScene();

    glutSwapBuffers();
}

// called by GLUT when the cpu is idle - has a timer function you can use for FPS, and updates the physics
// see https://www.opengl.org/resources/libraries/glut/spec3/node63.html#:~:text=glutIdleFunc
// NOTE this may be capped at 60 fps as we are using glutPostRedisplay(). If you want it to go higher than this, maybe a thread will help here. 
void CubePhysics::Idle() {
    static auto last = steady_clock::now();
    auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    float deltaTime = frameTime.count();

    m_boxes->Update(deltaTime);
    FPSCounter::ShowFPS(deltaTime);

    // tell glut to draw - note this will cap this function at 60 fps
    glutPostRedisplay();
}

void CubePhysics::Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Get the camera position and direction
        fvec3 cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        fvec3 cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        fvec3 clickedWorldPos = ScreenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        fvec3 rayDirection = clickedWorldPos - cameraPosition;
        normalize(rayDirection);

        Box box = m_boxes->SelectBox(cameraPosition, rayDirection);

        // Remove the clicked box if any
        m_boxes->RemoveBox(box);
    }
}

void CubePhysics::Keyboard(unsigned char key, int x, int y)
{
    const float impulseMagnitude = 20.0f; // Upward impulse magnitude

    switch (key)
    {
    case ' ':
    {
        m_boxes->ApplyImpulse(fvec3(0.0f, 20.0f, 0.0f));
        break;
    }
    case 'm':
    {
        DefaultTracker::OutputStats();
        break;
    }
    case 't':
    {
        throw;
        break;
    }
    case 'u':
    {
        m_boxes->Update(1.0f / 60.0f);
        break;
    }
    default:
    {
        break;
    }
    }
}
