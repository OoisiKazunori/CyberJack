#include "Vector3.h"
#include<math.h>

Vector3::Vector3() :Vector3(0, 0, 0) {
}

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}




float Vector3::Length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3 &Vector3::normalize() {
    float len = Length();
    if (len != 0) {
        return *this /= len;
    }
    return *this;
}

float Vector3::dot(const Vector3 &v) const {
    return (x * v.x) + (y * v.y) + (z * v.z);
}

float Vector3::cross(const Vector3 &v) const {
    return (x * v.x) - (y * v.y) - (z * v.z);
}






Vector3 Vector3::operator+() const {
    return *this;
}

Vector3 Vector3::operator-() const {
    return -*this;
}

Vector3 &Vector3::operator+=(const Vector3 &v) {
    *this = *this + v;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &v) {
    *this = *this - v;
    return *this;
}

Vector3 &Vector3::operator*=(float s) {
    *this = *this * s;
    return *this;
}

Vector3 &Vector3::operator/=(float s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

const Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {

    float ansX = v1.x + v2.x;
    float ansY = v1.y + v2.y;
    float ansZ = v1.z + v2.z;

    Vector3 ans(ansX, ansY, ansZ);

    return ans;
}

//---------------------------------------------------------
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    float ansX = v1.x - v2.x;
    float ansY = v1.y - v2.y;
    float ansZ = v1.z - v2.z;

    Vector3 ans(ansX, ansY, ansZ);

    return ans;
}


const Vector3 operator*(const Vector3 &v1, float s) {
    return Vector3{ v1.x * s,v1.y * s,v1.z * s };
}


const Vector3 operator*(float s, const Vector3 &v2) {

    float ansX = v2.x * s;
    float ansY = v2.y * s;
    float ansZ = v2.z * s;

    Vector3 asn(ansX, ansY, ansZ);

    return asn;
}


const Vector3 operator/(const Vector3 &v1, float s) {
    Vector3 tmp(v1);
    return tmp / s;
}
