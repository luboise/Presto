
#include "Vector3.h"
#include <cmath>

Vector3::Vector3() : Vector3(0, 0, 0) {}

Vector3::Vector3(const float x, const float y) : Vector3(x, y, 0){};

Vector3::Vector3(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

Vector3::~Vector3() {}

Vector3 Vector3::normalised() const {
    if (this->getLength() == 0.0f) {
        float val = 1 / sqrt(3);
        return Vector3(val, val, val);
    }

    auto length = this->getLength();

    return Vector3(this->x / length, this->y / length, this->z / length);
}

float Vector3::getLength() const { return sqrt((x * x) + (y * y) + (z * z)); }

bool Vector3::operator==(const Vector3& other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool Vector3::operator==(const float val) const {
    return this->getLength() == val;
}