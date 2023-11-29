#pragma once
#include "constants.h"
#include <Dense>

using Eigen::Vector3f;

// the box (falling item)
struct Box {
    Vector3f position;
    Vector3f size;
    Vector3f velocity;
    Vector3f colour;
    bool active;
    Box()
    {
        this->position = Vector3f::Zero();
        this->size = Vector3f::Ones();
        this->velocity = Vector3f::Zero();
        this->colour = Vector3f::Zero();
        active = true;
    }
    Box(const Vector3f& position, const Vector3f& velocity, const Vector3f& colour)
    {
        this->position = position;
        this->size = Vector3f::Ones();
        this->velocity = velocity;
        this->colour = colour;
        active = true;
    }
};

typedef std::array<Box, BOX_COUNT> BoxArray;