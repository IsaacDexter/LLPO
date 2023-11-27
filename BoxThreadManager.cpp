#include "BoxThreadManager.h"
#include "Scene.h"

BoxThreadManager::BoxThreadManager()
{
    threads = new ThreadArray();
}

void BoxThreadManager::StartThreads()
{
    for (auto& thread : *threads)
    {
        thread.Start();
    }
}

void BoxThreadManager::UpdateThreads()
{
    for (auto& thread : *threads)
    {
        thread.Update();
    }
}

void BoxThreadManager::CreateThreads(BoxArray* boxes)
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

    //Assign the first thread
    for (auto& thread : *threads)
    {
        auto start = end;
        //Calculate at which iterator to end this thread
        end += sectionSize + remainder;
        //Spin up the thread, including the remainder
        thread.Create(start, end);
        //Set the remainder to 0, to prevent anything but the first thread from using it
        remainder = 0;
    }
}

BoxThread::BoxThread()
{
    this->update = false;
}

void BoxThread::Create(BoxArray::iterator start, BoxArray::iterator end)
{
    this->start = start;
    this->end = end;
    this->update = false;
    thread = std::thread(&BoxThread::UpdateScene, this);
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
        }
    }
}
