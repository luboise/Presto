#pragma once
#include "prpch.h"

class Vector2 {
   public:
    float x = 0;
    float y = 0;

    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& other);
    ~Vector2();

    Vector2 normalised() const;
    float getLength() const;
};