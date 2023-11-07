#include "BoxManager.h"

BoxManager::BoxManager()
{
    positions = new std::array<dvec3, maximum>();
    positions->fill(dvec3(0.0f));

    velocities = new std::array<dvec3, maximum>();
    velocities->fill(dvec3(0.0f));

    sizes = new std::array<dvec3, maximum>();
    sizes->fill(dvec3(0.0f));

    colors = new std::array<dvec3, maximum>();
    colors->fill(dvec3(0.0f));

    active = new std::array<bool, maximum>();
    active->fill(false);
}

void BoxManager::Update(const double deltaTime)
{

    char buffer[1000];

    sprintf_s(buffer, "DeltaTime = %f, Position = %f, %f, %f, Velocity = %f, %f, %f\n", deltaTime, positions->at(0).x, positions->at(0).y, positions->at(0).z, velocities->at(0).x, velocities->at(0).y, velocities->at(0).z);
    OutputDebugStringA(buffer);

    for (unsigned int a = 0; a < m_count; a++)
    {
        //Early out for inactive boxes (i.e. 'deleted')
        if (!active->at(a))
        {
            continue;
        }

        //dvec3& position = positions->at(a);
        //dvec3& velocity = velocities->at(a);
        //dvec3 halfSize = sizes->at(a) / 2.0;

        dvec3& position = positions->at(a);
        dvec3& velocity = velocities->at(a);
        dvec3 halfSize = sizes->at(a) / 2.0;

        //Update velocity due to gravity
        //v = a * t
        velocity.y += gravity * deltaTime;
        //s = v * t
        position += velocity * deltaTime;

        //Check for collision with the floor
        if (position.y - sizes->at(a).y / 2.0f < floorY)
        {
            //Move the box to sit on the floor and have it bounce up
            position.y = floorY + sizes->at(a).y / 2.0f;
            velocity.y = -velocity.y * dampening;
        }

        // Check for collision with the walls
        if (position.x - sizes->at(a).x / 2.0f < minX || position.x + sizes->at(a).x / 2.0f > maxX)
        {
            velocity.x = -velocity.x * dampening;
        }
        if (position.z - sizes->at(a).z / 2.0f < minZ || position.z + sizes->at(a).z / 2.0f > maxZ)
        {
            velocity.z = -velocity.z * dampening;
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
                break;
            }
        }
    }
}

void BoxManager::Draw()
{
    for (unsigned int a = 0; a < m_count; a++)
    {
        //early out for inactive ('deleted') boxes
        if (!active->at(a))
        {
            continue;
        }

        glPushMatrix();
        glTranslatef(positions->at(a).x, positions->at(a).y, positions->at(a).z);
        GLfloat diffuseMaterial[] = { colors->at(a).x, colors->at(a).y, colors->at(a).z, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
        glScalef(sizes->at(a).x, sizes->at(a).y, sizes->at(a).z);
        glRotatef(-90, 1, 0, 0);
        glutSolidCube(1.0);
        //glutSolidTeapot(1);
        //glutSolidCone(1, 1, 10, 10);
        glPopMatrix();
    }
}

void BoxManager::Init(const unsigned int& count)
{
    m_count = std::min(count, maximum);
    for (unsigned int i = 0; i < m_count; i++)
    {
        (positions->at(i)) = dvec3(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)), 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f)), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)));
        (sizes->at(i)) = dvec3(1.0f, 1.0f, 1.0f);
        (velocities->at(i)) = dvec3(-1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f)), 0.0f, 0.0f);
        (colors->at(i)) = dvec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
        (active->at(i)) = true;
    }
}

bool BoxManager::RayBoxIntersection(const dvec3& rayOrigin, const dvec3& rayDirection, Box i)
{
    dvec3& position = positions->at(i);
    dvec3 halfSize = sizes->at(i);
    halfSize /= 2.0;

    float txMin = (position.x - halfSize.x - rayOrigin.x) / rayDirection.x;
    float txMax = (position.x + halfSize.x - rayOrigin.x) / rayDirection.x;

    if (txMin > txMax) std::swap(txMin, txMax);

    float tyMin = (position.y - halfSize.y - rayOrigin.y) / rayDirection.y;
    float tyMax = (position.y + halfSize.y - rayOrigin.y) / rayDirection.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((txMin > tyMax) || (tyMin > txMax))
        return false;

    if (tyMin > txMin)
        txMin = tyMin;

    if (tyMax < txMax)
        txMax = tyMax;

    float tzMin = (position.z - halfSize.z - rayOrigin.z) / rayDirection.z;
    float tzMax = (position.z + halfSize.z - rayOrigin.z) / rayDirection.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((txMin > tzMax) || (tzMin > txMax))
        return false;

    return true;
}

void BoxManager::ResolveCollision(Box a, Box b)
{
    dvec3 relativeVelocity = velocities->at(a) - velocities->at(b);

    //Find the normal vector
    dvec3 normal = positions->at(a) - positions->at(b);
    normal = normalize(normal);

    //Compute relative velocity along the normal
    double impulse = dot(relativeVelocity, normal);

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

Box BoxManager::SelectBox(const dvec3& cameraPosition, const dvec3& rayDirection)
{
    float minIntersectionDistance = std::numeric_limits<float>::max();
    //Check if any box intersects with the ray
    for (unsigned int a = 0; a < m_count; a++)
    {
        //Early out for inactive boxes (i.e. 'deleted')
        if (!active->at(a))
        {
            continue;
        }
        if (RayBoxIntersection(cameraPosition, rayDirection, a))
        {
            // Calculate the distance between the camera and the intersected box
            dvec3 diff = positions->at(a) - cameraPosition;
            float distance = diff.length();

            // Update the clicked box index if this box is closer to the camera
            if (distance < minIntersectionDistance) {
                minIntersectionDistance = distance;
                return a;
            }
        }
    }

    return -1;
}

bool BoxManager::RemoveBox(Box a)
{
    if (a < 0 || a > m_count)
    {
        return false;
    }
    active->at(a) = false;
    return true;
}

bool BoxManager::CheckCollision(Box a, Box b)
{
    dvec3 distance = abs((positions->at(a) - positions->at(b))) * 2.0;
    dvec3 totalSize = sizes->at(a) + sizes->at(b);

    return(
        distance.x < totalSize.x &&
        distance.y < totalSize.y &&
        distance.z < totalSize.z
        );
}

void BoxManager::ApplyImpulse(const dvec3& impulse)
{
    for (unsigned int a = 0; a < m_count; a++)
    {
        //Early out for inactive boxes (i.e. 'deleted')
        if (!active->at(a))
        {
            continue;
        }
        velocities->at(a) += impulse;
    }
}

void BoxManager::ApplyImpulse(const dvec3& impulse, Box a)
{
    velocities->at(a) += impulse;
}
