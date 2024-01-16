// #include "prpch.h"

#include "Vectors.h"

Vector2::Vector2() {}

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2& other) {
    this->x = other.x;
    this->y = other.y;
}

Vector2::~Vector2() {}

Vector2 Vector2::normalised() const {
    auto length = this->getLength();
    return Vector2(this->x / length, this->y / length);
}

float Vector2::getLength() const { return (x * x) + (y * y); }
