#include "Scene.h"

void Scene::Init(const int boxCount)
{
    for (int i = 0; i < boxCount; ++i) {
        Box box;
        Vector3f vect;
        vect[0];

        // Assign random x, y, and z positions within specified ranges
        box.position.x() = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));
        box.position.y() = 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
        box.position.z() = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));

        box.size = { 1.0f, 1.0f, 1.0f };

        // Assign random x-velocity between -1.0f and 1.0f
        float randomXVelocity = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
        box.velocity = { randomXVelocity, 0.0f, 0.0f };

        // Assign a random color to the box
        box.colour.x() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        box.colour.y() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        box.colour.z() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        boxes.push_back(box);
    }
}

void Scene::Draw()
{
    // Draw the side wall
    GLfloat diffuseMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3f leftSideWallV1(minX, 0.0f, maxZ);
    Vector3f leftSideWallV2(minX, 50.0f, maxZ);
    Vector3f leftSideWallV3(minX, 50.0f, minZ);
    Vector3f leftSideWallV4(minX, 0.0f, minZ);
    drawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3f rightSideWallV1(maxX, 0.0f, maxZ);
    Vector3f rightSideWallV2(maxX, 50.0f, maxZ);
    Vector3f rightSideWallV3(maxX, 50.0f, minZ);
    Vector3f rightSideWallV4(maxX, 0.0f, minZ);
    drawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3f backWallV1(minX, 0.0f, minZ);
    Vector3f backWallV2(minX, 50.0f, minZ);
    Vector3f backWallV3(maxX, 50.0f, minZ);
    Vector3f backWallV4(maxX, 0.0f, minZ);
    drawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

    for (const Box& box : boxes) {
        drawBox(box);
    }
}

void Scene::Update(const double deltaTime)
{
    const float floorY = 0.0f;

    //char buffer[100];
    //sprintf_s(buffer, "DeltaTime = %f\n", deltaTime);
    //OutputDebugStringA(buffer);

    for (Box& box : boxes) {
        // Update velocity due to gravity
        box.velocity.y() += gravity * deltaTime;

        // Update position based on velocity
        box.position.x() += box.velocity.x() * deltaTime;
        box.position.y() += box.velocity.y() * deltaTime;
        box.position.z() += box.velocity.z() * deltaTime;

        // Check for collision with the floor
        if (box.position.y() - box.size.y() / 2.0f < floorY) {
            box.position.y() = floorY + box.size.y() / 2.0f;
            float dampening = 0.7f;
            box.velocity.y() = -box.velocity.y() * dampening;
        }

        // Check for collision with the walls
        if (box.position.x() - box.size.x() / 2.0f < minX || box.position.x() + box.size.x() / 2.0f > maxX) {
            box.velocity.x() = -box.velocity.x();
        }
        if (box.position.z() - box.size.z() / 2.0f < minZ || box.position.z() + box.size.z() / 2.0f > maxZ) {
            box.velocity.z() = -box.velocity.z();
        }

        // Check for collisions with other boxes
        for (Box& other : boxes) {
            if (&box == &other) continue;
            if (checkCollision(box, other)) {
                resolveCollision(box, other);
                break;
            }
        }
    }
}

Scene::Scene()
{
    boxes = std::vector<Box>();
}

Scene::~Scene()
{
}

Vector3f Scene::ScreenToWorld(const double x, const double y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX = 0.0f, posY = 0.0f, posZ = 0.0f;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


    Vector3f(posX, posY, posZ);
    return Vector3f((float)posX, (float)posY, (float)posZ);
}

bool Scene::rayBoxIntersection(const Vector3f& rayOrigin, const Vector3f& rayDirection, const Box& box)
{
    float tMin = (box.position.x() - box.size.x() / 2.0f - rayOrigin.x()) / rayDirection.x();
    float tMax = (box.position.x() + box.size.x() / 2.0f - rayOrigin.x()) / rayDirection.x();

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (box.position.y() - box.size.y() / 2.0f - rayOrigin.y()) / rayDirection.y();
    float tyMax = (box.position.y() + box.size.y() / 2.0f - rayOrigin.y()) / rayDirection.y();

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (box.position.z() - box.size.z() / 2.0f - rayOrigin.z()) / rayDirection.z();
    float tzMax = (box.position.z() + box.size.z() / 2.0f - rayOrigin.z()) / rayDirection.z();

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

void Scene::resolveCollision(Box& a, Box& b)
{
    Vector3f normal = { a.position.x() - b.position.x(), a.position.y() - b.position.y(), a.position.z() - b.position.z() };
    float length = std::sqrt(normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z());

    // Normalize the normal vector
    normal.normalize();

    float relativeVelocityX = a.velocity.x() - b.velocity.x();
    float relativeVelocityY = a.velocity.y() - b.velocity.y();
    float relativeVelocityZ = a.velocity.z() - b.velocity.z();

    // Compute the relative velocity along the normal
    float impulse = relativeVelocityX * normal.x() + relativeVelocityY * normal.y() + relativeVelocityZ * normal.z();

    // Ignore collision if objects are moving away from each other
    if (impulse > 0) {
        return;
    }

    // Compute the collision impulse scalar
    float e = 0.01f; // Coefficient of restitution (0 = inelastic, 1 = elastic)
    float dampening = 0.9f; // Dampening factor (0.9 = 10% energy reduction)
    float j = -(1.0f + e) * impulse * dampening;

    // Apply the impulse to the boxes' velocities
    a.velocity.x() += j * normal.x();
    a.velocity.y() += j * normal.y();
    a.velocity.z() += j * normal.z();
    b.velocity.x() -= j * normal.x();
    b.velocity.y() -= j * normal.y();
    b.velocity.z() -= j * normal.z();
}

bool Scene::checkCollision(const Box& a, const Box& b)
{
    return (std::abs(a.position.x() - b.position.x()) * 2 < (a.size.x() + b.size.x())) &&
        (std::abs(a.position.y() - b.position.y()) * 2 < (a.size.y() + b.size.y())) &&
        (std::abs(a.position.z() - b.position.z()) * 2 < (a.size.z() + b.size.z()));
}

void Scene::drawQuad(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4)
{
    glBegin(GL_QUADS);
    glVertex3f(v1.x(), v1.y(), v1.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
    glVertex3f(v3.x(), v3.y(), v3.z());
    glVertex3f(v4.x(), v4.y(), v4.z());
    glEnd();
}

void Scene::drawBox(const Box& box)
{
    glPushMatrix();
    glTranslatef(box.position.x(), box.position.y(), box.position.z());
    GLfloat diffuseMaterial[] = { box.colour.x(), box.colour.y(), box.colour.z(), 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glScalef(box.size.x(), box.size.y(), box.size.z());
    glRotatef(-90, 1, 0, 0);

    drawCube();

    //glutSolidTeapot(1);
    //glutSolidCone(1, 1, 10, 10);
    glPopMatrix();
}

void Scene::drawCube()
{
    GLfloat vertices[] =
    {
        -0.5, -0.5, -0.5,   -0.5, -0.5,  0.5,   -0.5,  0.5,  0.5,   -0.5,  0.5, -0.5,
        0.5, -0.5, -0.5,    0.5, -0.5,  0.5,    0.5,  0.5,  0.5,    0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,   -0.5, -0.5,  0.5,    0.5, -0.5,  0.5,    0.5, -0.5, -0.5,
        -0.5,  0.5, -0.5,   -0.5,  0.5,  0.5,    0.5,  0.5,  0.5,    0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,   -0.5,  0.5, -0.5,    0.5,  0.5, -0.5,    0.5, -0.5, -0.5,
        -0.5, -0.5,  0.5,   -0.5,  0.5,  0.5,    0.5,  0.5,  0.5,    0.5, -0.5,  0.5
    };

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Scene::SelectBox(const Vector3f& camPos, const Vector3f& rayDir)
{
    // Perform a ray-box intersection test and remove the clicked box
    size_t clickedBoxIndex = -1;
    float minIntersectionDistance = FLT_MAX;

    for (size_t i = 0; i < boxes.size(); ++i) {
        if (rayBoxIntersection(camPos, rayDir, boxes[i])) {
            // Calculate the distance between the camera and the intersected box
            Vector3f diff = boxes[i].position - camPos;
            float distance = diff.norm();

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

void Scene::ApplyImpulse(const Vector3f& impulse)
{
    for (Box& box : boxes) {
        box.velocity += impulse;
    }
}
