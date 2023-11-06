#pragma once
#include <iostream>
#include <array>
#include <glm.hpp>
#include <GL/glut.h>

using namespace glm;

typedef const unsigned int& Box;

#define BOX_COUNT 50

#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

class BoxManager
{
private:
    // this is the number of falling physical items. 
    static const unsigned int maximum = 50;
    const float gravity = -19.81f;
    const float floorY = 0.0f;
    unsigned int m_count = 0;

    /// <summary>Coefficient of restitution (0 = inelastic, 1 = elastic)summary>
    const double e = 0.01;
    /// <summary>Dampening factor (0.9 = 10% energy reduction)</summary>
    const double dampening = 0.9;

    std::array<fvec3, maximum>* positions    = new std::array<fvec3, maximum>();
    std::array<fvec3, maximum>* sizes        = new std::array<fvec3, maximum>();
    std::array<fvec3, maximum>* velocities   = new std::array<fvec3, maximum>();
    std::array<fvec3, maximum>* colors       = new std::array<fvec3, maximum>();
    std::array<bool, maximum>* active = new std::array<bool, maximum>();

public:
    BoxManager();

    void Update(const float& deltaTime);
    // draw the physics objects
    void Draw();


    void Init(const unsigned int& count);
    // a ray which is used to tap (by default, remove) a box - see the 'mouse' function for how this is used.
    bool RayBoxIntersection(const fvec3& rayOrigin, const fvec3& rayDirection, Box i);

    void ResolveCollision(Box a, Box b);
    Box SelectBox(const fvec3& rayOrigin, const fvec3& rayDirection);
    bool RemoveBox(Box a);

    /// <summary>Are these two boxes colliding?</summary>
    /// <param name="a">The index of the first box's position, size etc</param>
    /// <param name="b">The index of the second box's position, size etc</param>
    /// <returns>Whether or not the two boxes overlap</returns>
    bool CheckCollision(Box a, Box b);

    void ApplyImpulse(const fvec3& impulse);
    void ApplyImpulse(const fvec3& impulse, Box a);
};

