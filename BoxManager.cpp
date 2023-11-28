#include "BoxManager.h"
#include "Physio.h"

BoxManager::BoxManager()
{
    boxes = new BoxArray();
    threads = new ThreadArray();
    completedThreads.second = 0;
}

void BoxManager::StartThreads()
{
    for (auto& thread : *threads)
    {
        thread.Start();
    }
}

void BoxManager::UpdateThreads()
{
    //Called when all the previous threads have updated
    //Sync deltaTime
    Physio::Update();
    //Start the threads' next update
    for (auto& thread : *threads)
    {
        thread.Update();
    }

}

void BoxManager::CreateThreads(BoxArray* boxes)
{
    //early out with empty box array/no threads to prevent throws from -1 or /0 errors
    if (boxes->size() == 0 || threads->size() == 0)
    {
        return;
    }

    //Calculate how much of the cubes each thread will iterate through
    const int totalSize = boxes->size();
    const unsigned int sectionSize = totalSize / threads->size();
    //Caclulate how much additional load the first thread will have to carry
    unsigned int remainder = totalSize - sectionSize * threads->size();

    auto end = boxes->begin();

    std::function<void()>* completeCall = new std::function<void()>();
    *completeCall = [this] { OutputDebugStringA("Update complete!\n"); this->MarkUpdateComplete(); };

    //Assign the first thread
    for (auto& thread : *threads)
    {
        auto start = end;
        //Calculate at which iterator to end this thread
        end += sectionSize + remainder;
        //Spin up the thread, including the remainder
        thread.Create(start, end, completeCall);
        //Set the remainder to 0, to prevent anything but the first thread from using it
        remainder = 0;
    }
}

void BoxManager::MarkUpdateComplete()
{
    //Called when each thread completes its update
    //Stop other threads from incrementing until this check is complete
    completedThreads.first.lock();
    completedThreads.second++;
    //If that was the final thread to complete its update
    if (completedThreads.second >= threads->size())
    {
        OutputDebugStringA("Both updates complete!\n");
        completedThreads.second = 0;
        //Handle collisions before unlocking threads
        CheckCollisions();
        //Have the threads update again
        completedThreads.first.unlock();
        UpdateThreads();
        return;
    }
    completedThreads.first.unlock();
}

BoxThread::BoxThread()
{
    this->update = false;
}

void BoxThread::Create(BoxArray::iterator start, BoxArray::iterator end, std::function<void()>* completeCall)
{
    this->start = start;
    this->end = end;
    this->update = true;
    thread = std::thread(&BoxThread::UpdateScene, this);
    this->completeCall = completeCall;
}

void BoxThread::Start()
{
    thread.detach();
}

void BoxThread::UpdateScene()
{
    while (true)
    {
        if (update)
        {
            const float floorY = 0.0f;

            //char buffer[100];
            //sprintf_s(buffer, "DeltaTime = %f\n", deltaTime);
            //OutputDebugStringA(buffer);

            for (auto box = start; box != end; box++)
            {
                if (!box->active)
                {
                    continue;
                }

                box->velocity.y() += GRAVITY * g_deltaTime;
                // Update position based on velocity
                box->position += box->velocity * g_deltaTime;

                // Check for collision with the floor
                if (box->position.y() - box->size.y() / 2.0f < floorY) {
                    box->position.y() = floorY + box->size.y() / 2.0f;
                    float dampening = 0.7f;
                    box->velocity.y() = -box->velocity.y() * dampening;
                }

                // Check for collision with the walls
                if (box->position.x() - box->size.x() / 2.0f < minX || box->position.x() + box->size.x() / 2.0f > maxX) {
                    box->velocity.x() = -box->velocity.x();
                }
                if (box->position.z() - box->size.z() / 2.0f < minZ || box->position.z() + box->size.z() / 2.0f > maxZ) {
                    box->velocity.z() = -box->velocity.z();
                }
            }
            update = false;
            //Mark in the thread manager that the thread is complete
            (*completeCall)();
        }
    }
}

void BoxManager::Init()
{
    for (auto &box: *boxes) 
    {
        //Initialize a new box
        box = Box();

        // Assign random x, y, and z positions within specified ranges
        box.position.x() = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));
        box.position.y() = 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
        box.position.z() = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));

        // Assign random x-velocity between -1.0f and 1.0f
        box.velocity.x() = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));;

        // Assign a random color to the box
        box.colour.x() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        box.colour.y() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        box.colour.z() = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    
    //Initialize thread manager
    CreateThreads(boxes);
    StartThreads();
}

void BoxManager::Draw()
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

    for (const auto& box : *boxes) {
        if (!box.active)
        {
            continue;
        }
        drawBox(box);
    }
}

void BoxManager::CheckCollisions()
{
    // Check for collisions between each box other boxes
    for (Box& box : *boxes) {
        for (Box& other : *boxes) {
            if (&box == &other) continue;
            if (checkCollision(box, other)) {
                resolveCollision(box, other);
                break;
            }
        }
    }

    //const float floorY = 0.0f;

    ////char buffer[100];
    ////sprintf_s(buffer, "DeltaTime = %f\n", deltaTime);
    ////OutputDebugStringA(buffer);

    //for (Box& box : *boxes) {
    //    // Update velocity due to gravity
    //    box.velocity.y() += GRAVITY * g_deltaTime;

    //    // Update position based on velocity
    //    box.position += box.velocity * g_deltaTime;

    //    // Check for collision with the floor
    //    if (box.position.y() - box.size.y() / 2.0f < floorY) {
    //        box.position.y() = floorY + box.size.y() / 2.0f;
    //        float dampening = 0.7f;
    //        box.velocity.y() = -box.velocity.y() * dampening;
    //    }

    //    // Check for collision with the walls
    //    if (box.position.x() - box.size.x() / 2.0f < minX || box.position.x() + box.size.x() / 2.0f > maxX) {
    //        box.velocity.x() = -box.velocity.x();
    //    }
    //    if (box.position.z() - box.size.z() / 2.0f < minZ || box.position.z() + box.size.z() / 2.0f > maxZ) {
    //        box.velocity.z() = -box.velocity.z();
    //    }

    //    // Check for collisions with other boxes
    //    for (Box& other : *boxes) {
    //        if (&box == &other) continue;
    //        if (checkCollision(box, other)) {
    //            resolveCollision(box, other);
    //            break;
    //        }
    //    }
    //}
}

//void BoxManager::UpdateSection(const double deltaTime, BoxArray::iterator box, BoxArray::iterator end)
//{
//    const float floorY = 0.0f;
//
//    //char buffer[100];
//    //sprintf_s(buffer, "DeltaTime = %f\n", deltaTime);
//    //OutputDebugStringA(buffer);
//
//    for (;box != end; box++)
//    {
//        if (!box->active)
//        {
//            continue;
//        }
//        box->velocity.y() += GRAVITY * deltaTime;
//
//        // Update position based on velocity
//        box->position.x() += box->velocity.x() * deltaTime;
//        box->position.y() += box->velocity.y() * deltaTime;
//        box->position.z() += box->velocity.z() * deltaTime;
//
//        // Check for collision with the floor
//        if (box->position.y() - box->size.y() / 2.0f < floorY) {
//            box->position.y() = floorY + box->size.y() / 2.0f;
//            float dampening = 0.7f;
//            box->velocity.y() = -box->velocity.y() * dampening;
//        }
//
//        // Check for collision with the walls
//        if (box->position.x() - box->size.x() / 2.0f < minX || box->position.x() + box->size.x() / 2.0f > maxX) {
//            box->velocity.x() = -box->velocity.x();
//        }
//        if (box->position.z() - box->size.z() / 2.0f < minZ || box->position.z() + box->size.z() / 2.0f > maxZ) {
//            box->velocity.z() = -box->velocity.z();
//        }
//
//        // Check for collisions with other boxes
//        for (Box& other : *boxes) {
//            if (&*box == &other) continue;
//            if (checkCollision(*box, other)) {
//                resolveCollision(*box, other);
//                break;
//            }
//        }
//    }
//}


Vector3f BoxManager::ScreenToWorld(const double x, const double y)
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

bool BoxManager::rayBoxIntersection(const Vector3f& rayOrigin, const Vector3f& rayDirection, const Box& box)
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

void BoxManager::resolveCollision(Box& a, Box& b)
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

bool BoxManager::checkCollision(const Box& a, const Box& b)
{
    return (std::abs(a.position.x() - b.position.x()) * 2 < (a.size.x() + b.size.x())) &&
        (std::abs(a.position.y() - b.position.y()) * 2 < (a.size.y() + b.size.y())) &&
        (std::abs(a.position.z() - b.position.z()) * 2 < (a.size.z() + b.size.z()));
}

void BoxManager::drawQuad(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4)
{
    glBegin(GL_QUADS);
    glVertex3f(v1.x(), v1.y(), v1.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
    glVertex3f(v3.x(), v3.y(), v3.z());
    glVertex3f(v4.x(), v4.y(), v4.z());
    glEnd();
}

void BoxManager::drawBox(const Box& box)
{
    glPushMatrix();
    glTranslatef(box.position.x(), box.position.y(), box.position.z());
    GLfloat diffuseMaterial[] = { box.colour.x(), box.colour.y(), box.colour.z(), 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glScalef(box.size.x(), box.size.y(), box.size.z());
    glRotatef(-90, 1, 0, 0);

    //drawCube();
    
    glutSolidCube(1);
    //glutSolidTeapot(1);
    //glutSolidCone(1, 1, 10, 10);
    glPopMatrix();
}

void BoxManager::drawCube()
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

void BoxManager::SelectBox(const Vector3f& camPos, const Vector3f& rayDir)
{
    // Perform a ray-box intersection test and remove the clicked box
    float minIntersectionDistance = FLT_MAX;

    Box* selection = nullptr;

    for (auto& box : *boxes) {
        if (rayBoxIntersection(camPos, rayDir, box)) {
            // Calculate the distance between the camera and the intersected box
            Vector3f diff = box.position - camPos;
            float distance = diff.norm();

            // Update the clicked box if this box is closer to the camera
            if (distance < minIntersectionDistance) {
                selection = &box;
                minIntersectionDistance = distance;
            }
        }
    }

    // Remove the clicked box if any
    if (selection != nullptr) {
        selection->active = false;
    }
}

void BoxManager::ApplyImpulse(const Vector3f& impulse)
{
    for (auto& box : *boxes) {
        box.velocity += impulse;
    }
}