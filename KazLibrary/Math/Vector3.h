#pragma once
#include"../DirectXCommon/Base.h"

struct Vector
{
	XMVECTOR vec;

	float x()
	{
		return vec.m128_f32[0];
	}
	float y()
	{
		return vec.m128_f32[1];
	}
	float z()
	{
		return vec.m128_f32[2];
	}
};

class Vector3{
public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y,float z);

	float Length()const;
	Vector3 &normalize();
	float dot(const Vector3 &v)const;
	float cross(const Vector3 &v)const;

	Vector3 operator+()const;

	Vector3 operator-()const;

	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(float s);
	Vector3 &operator/=(float s);

};

const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator*(const Vector3 &v1, float s);
const Vector3 operator*(float s, const Vector3 &v2);
const Vector3 operator/(const Vector3 &v1, float s);


