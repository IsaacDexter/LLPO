#pragma once
#include <iostream>

class Vector3d {
public:
    float x, y, z;

    Vector3d() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}

    // overload the minus operator
    Vector3d operator-(const Vector3d& other) const {
        return Vector3d(x - other.x(), y - other.y(), z - other.z());
    }

    // Normalize the vector
    void normalise() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length != 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    // get the length of a vector
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};