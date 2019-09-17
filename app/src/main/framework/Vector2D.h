//
//
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

struct Vector2D {
    float x;
    float y;
public:
    void operator=(const Vector2D& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
    }
    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    }
    Vector2D(float xPos, float yPos) {
        x = xPos;
        y = yPos;
    }
};

#endif //VECTOR2D_H_H
