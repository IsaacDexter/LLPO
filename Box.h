#pragma once

using Eigen::Vector3f;

// the box (falling item)
struct Box {
    Vector3f position;
    Vector3f size;
    Vector3f velocity;
    Vector3f colour;
};