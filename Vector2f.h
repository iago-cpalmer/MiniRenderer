#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <cmath>

class Vector2f {
public:
    float x;
    float y;
    float z;
    Vector2f();
    Vector2f(float x, float y);
    float DotProduct(Vector2f b);
    float Magnitude();
    Vector2f Cross(Vector2f b);
    void Normalize();
    float Distance(Vector2f b);
};

#endif