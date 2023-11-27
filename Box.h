#pragma once

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
        Box(Vector3f::Zero(), Vector3f::Ones(), Vector3f::Zero(), Vector3f::Zero());
    }
    Box(const Vector3f& position, const Vector3f& size, const Vector3f& velocity, const Vector3f& colour)
    {
        this->position = position;
        this->size = size;
        this->velocity = velocity;
        this->colour = colour;
    }
};