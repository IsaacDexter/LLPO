#pragma once
#include "constants.h"
#include <Dense>
#include "MemoryPool.h"

using Eigen::Vector3f;


// the box (falling item)
struct Box {
    Vector3f position;
    Vector3f size;
    Vector3f velocity;
    Vector3f colour;
    Box()
    {
        this->position = Vector3f::Zero();
        this->size = Vector3f::Ones();
        this->velocity = Vector3f::Zero();
        this->colour = Vector3f::Zero();
    }
    Box(const Vector3f& position, const Vector3f& velocity, const Vector3f& colour)
    {
        this->position = position;
        this->size = Vector3f::Ones();
        this->velocity = velocity;
        this->colour = colour;
    }
    static void* operator new(size_t size);

    static void operator delete(void* pMem, size_t size);
};

extern MemoryPool<BOX_COUNT, sizeof(Box)> boxPool;
