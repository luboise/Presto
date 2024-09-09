#pragma once

class Vector3 {
   public:
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3();
    Vector3(const float x, const float y);
    Vector3(const float x, const float y, const float z);
    Vector3(const Vector3& other);
    ~Vector3();

    Vector3 normalised() const;
    float getLength() const;

    bool operator==(const Vector3& other) const;
    bool operator==(const float val) const;
};
