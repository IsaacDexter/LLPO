#include "BoxManager.h"

BoxManager::BoxManager()
{
}

void BoxManager::Update(const float& deltaTime)
{
    for (unsigned int a = 0; a < m_count; a++)
    {
        Vector3d& position = positions->at(a);
        Vector3d& velocity = velocities->at(a);
        Vector3d& halfSize = sizes->at(a);
        halfSize / 2.0;

        //Update velocity due to gravity
        //v = a * t
        velocity.y() += gravity * deltaTime;
        //s = v * t
        position += velocity * deltaTime;

        //Check for collision with the floor
        if (position.y() - halfSize.y() < floorY)
        {
            //Move the box to sit on the floor and have it bounce up
            position.y() = floorY + halfSize.y();
            velocity.y() = -velocity.y() * dampening;
        }

        // Check for collision with the walls
        if (position.x() - halfSize.x() < minX || position.x() + halfSize.x() > maxX)
        {
            velocity.x() = -velocity.x() * dampening;
        }
        if (position.z() - halfSize.z() < minZ || position.z() + halfSize.z() > maxZ)
        {
            velocity.z() = -velocity.z() * dampening;
        }

        //Check collisions with other boxes
        for (unsigned int b = 0; b < m_count; b++)
        {
            //early out to avoid checking the same box
            if (b == a)
            {
                continue;
            }
            //If theres a collision, handle it
            if (CheckCollision(a, b))
            {
                ResolveCollision(a, b);
            }
        }
    }
}

void BoxManager::Draw()
{
    for (unsigned int a = 0; a < m_count; a++)
    {
        DrawBox(a);
    }
}

void BoxManager::DrawBox(const unsigned int& a)
{
    Vector3d& position = positions->at(a);
    Vector3d& size = sizes->at(a);
    Vector3d& color = colors->at(a);

    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    GLfloat diffuseMaterial[] = { color.x(), color.y(), color.z(), 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glScalef(size.x(), size.y(), size.z());
    glRotatef(-90, 1, 0, 0);
    glutSolidCube(1.0);
    //glutSolidTeapot(1);
    //glutSolidCone(1, 1, 10, 10);
    glPopMatrix();
}

void BoxManager::Init(const unsigned int& count)
{
    m_count = std::min(count, maximum);
    for (unsigned int i = 0; i < m_count; i++)
    {
        (positions->at(i)) = { static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)), 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f)), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)) };
        (sizes->at(i)) = { 1.0f, 1.0f, 1.0f };
        (velocities->at(i)) = { -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f)), 0.0f, 0.0f };
        (colors->at(i)) = { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX) };
    }
}

bool BoxManager::RayBoxIntersection(const Vector3d& rayOrigin, const Vector3d& rayDirection, const unsigned int& i)
{
    Vector3d& position = positions->at(i);
    Vector3d& halfSize = sizes->at(i);
    halfSize /= 2.0;

    float txMin = (position.x() - halfSize.x() - rayOrigin.x()) / rayDirection.x();
    float txMax = (position.x() + halfSize.x() - rayOrigin.x()) / rayDirection.x();

    if (txMin > txMax) std::swap(txMin, txMax);

    float tyMin = (position.y() - halfSize.y() - rayOrigin.y()) / rayDirection.y();
    float tyMax = (position.y() + halfSize.y() - rayOrigin.y()) / rayDirection.y();

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((txMin > tyMax) || (tyMin > txMax))
        return false;

    if (tyMin > txMin)
        txMin = tyMin;

    if (tyMax < txMax)
        txMax = tyMax;

    float tzMin = (position.z() - halfSize.z() - rayOrigin.z()) / rayDirection.z();
    float tzMax = (position.z() + halfSize.z() - rayOrigin.z()) / rayDirection.z();

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((txMin > tzMax) || (tzMin > txMax))
        return false;

    return true;
}

void BoxManager::ResolveCollision(const unsigned int& a, const unsigned int& b)
{
    Vector3d relativeVelocity = velocities->at(a) - velocities->at(b);

    //Find the normal vector
    Vector3d normal = positions->at(a) - positions->at(b);
    normal.normalize();

    //Compute relative velocity along the normal
    double impulse = relativeVelocity.dot(normal);

    if (impulse > 0)
    {
        //Early out if objects are moving away from eachother
        return;
    }

    //compute collision impulse scalar
    double j = -(1.0f + e) * impulse * dampening;
    normal *= j;

    //Apply the impulse to the boxes velocities
    velocities->at(a) += normal;
    velocities->at(b) -= normal;
}

bool BoxManager::CheckCollision(const unsigned int& a, const unsigned int& b)
{
    Vector3d distance = (positions->at(a) - positions->at(b)) * 2;
    Vector3d totalSize = sizes->at(a) + sizes->at(b);

    return(
        distance.x() < totalSize.x() &&
        distance.y() < totalSize.y() &&
        distance.z() < totalSize.z()
        );
}

void BoxManager::ApplyImpulse(const Vector3d& impulse)
{
    for (unsigned int a = 0; a < m_count; a++)
    {
        velocities->at(a) += impulse;
    }
}

void BoxManager::ApplyImpulse(const Vector3d& impulse, const unsigned int& a)
{
    velocities->at(a) += impulse;
}
