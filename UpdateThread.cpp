#include "UpdateThread.h"
#include "Scene.h"

BoxThread::BoxThread()
{
    deltaTime = 0.0f;
}

void BoxThread::CreateThread(BoxArray::iterator start, BoxArray::iterator end)
{
    this->start = start;
    this->end = end;
    deltaTime = 0.0f;
    thread = std::thread(&BoxThread::StartThread, this);

    if (thread.joinable())
    {
        active = true;
        thread.detach();
    }
}

void BoxThread::StartThread()
{
    while (active)
    {
        Update();
    }
}

void BoxThread::Update()
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

            box->velocity.y() += GRAVITY * deltaTime;
            // Update position based on velocity
            box->position += box->velocity * deltaTime;

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
    }
}

void BoxThread::UpdateThread(const double deltaTime)
{
    this->deltaTime = deltaTime;
    update = true;
}
