#pragma once
#include <iostream>
#include <array>

#define BOX_COUNT 50

#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

class BoxManager
{
private:
    // this is the number of falling physical items. 
    static const size_t maximum = 50;
    const float gravity = -19.81f;
    const float floorY = 0.0f;
    unsigned int count = maximum;

    /// <summary>Coefficient of restitution (0 = inelastic, 1 = elastic)summary>
    const double e = 0.01;
    /// <summary>Dampening factor (0.9 = 10% energy reduction)</summary>
    const double dampening = 0.9;
    
    std::array<Vector3d, maximum>* positions    = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* sizes        = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* velocities   = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* colors       = new std::array<Vector3d, maximum>();

public:
    BoxManager()
    {
    }

    void DrawBoxes()
    {
        for (unsigned int a = 0; a < count; a++)
        {
            DrawBox(a);
        }
    }

    // draw the physics object
    void DrawBox(const unsigned int& a) 
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

    void InitBoxes()
    {
        for (unsigned int i = 0; i < count; i++)
        {
            (positions->at(i)) = { static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)), 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f)), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f)) };
            (sizes->at(i)) = { 1.0f, 1.0f, 1.0f };
            (velocities->at(i)) = { -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f)), 0.0f, 0.0f };
            (colors->at(i)) = { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX) , static_cast<float>(rand()) / static_cast<float>(RAND_MAX) };
        }
    }

    // a ray which is used to tap (by default, remove) a box - see the 'mouse' function for how this is used.
    bool RayBoxIntersection(const Vector3d& rayOrigin, const Vector3d& rayDirection, const unsigned int& i)
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

    void ResolveCollision(const unsigned int& a, const unsigned int& b)
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

    /// <summary>Are these two boxes colliding?</summary>
    /// <param name="a">The index of the first box's position, size etc</param>
    /// <param name="b">The index of the second box's position, size etc</param>
    /// <returns>Whether or not the two boxes overlap</returns>
    bool CheckCollision(const unsigned int& a, const unsigned int& b)
    {
        Vector3d distance = (positions->at(a) - positions->at(b)) * 2;
        Vector3d totalSize = sizes->at(a) + sizes->at(b);

        return(
            distance.x() < totalSize.x() &&
            distance.y() < totalSize.y() &&
            distance.z() < totalSize.z()
            );
    }

    void Update()

    void UpdatePhysics(const float& deltaTime)
    {
        for (unsigned int a = 0; a < count; a++)
        {
            Vector3d &position = positions->at(a);
            Vector3d &velocity = velocities->at(a);
            Vector3d &halfSize = sizes->at(a);
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
            for (unsigned int b = 0; b < count; b++)
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
};

