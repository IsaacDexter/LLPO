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
    static const unsigned int maximum = 50;
    const float gravity = -19.81f;
    const float floorY = 0.0f;
    unsigned int m_count;

    /// <summary>Coefficient of restitution (0 = inelastic, 1 = elastic)summary>
    const double e = 0.01;
    /// <summary>Dampening factor (0.9 = 10% energy reduction)</summary>
    const double dampening = 0.9;
    
    std::array<Vector3d, maximum>* positions    = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* sizes        = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* velocities   = new std::array<Vector3d, maximum>();
    std::array<Vector3d, maximum>* colors       = new std::array<Vector3d, maximum>();

public:
    BoxManager();

    void Update(const float& deltaTime);
    void Draw();

    // draw the physics object
    void DrawBox(const unsigned int& a);

    void Init(const unsigned int& count);

    // a ray which is used to tap (by default, remove) a box - see the 'mouse' function for how this is used.
    bool RayBoxIntersection(const Vector3d& rayOrigin, const Vector3d& rayDirection, const unsigned int& i);

    void ResolveCollision(const unsigned int& a, const unsigned int& b);

    /// <summary>Are these two boxes colliding?</summary>
    /// <param name="a">The index of the first box's position, size etc</param>
    /// <param name="b">The index of the second box's position, size etc</param>
    /// <returns>Whether or not the two boxes overlap</returns>
    bool CheckCollision(const unsigned int& a, const unsigned int& b);

    void ApplyImpulse(const Vector3d& impulse);
    void ApplyImpulse(const Vector3d& impulse, const unsigned int& a);
};

