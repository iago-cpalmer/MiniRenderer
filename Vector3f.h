#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <cmath>

class Vector3f {
public:
    float x;
    float y;
    float z;
    Vector3f();
    Vector3f(float x, float y, float z);
    float DotProduct(Vector3f b);
    float Magnitude();
    Vector3f Cross(Vector3f b);
    void Normalize();
    float Distance(Vector3f b);
};

#endif