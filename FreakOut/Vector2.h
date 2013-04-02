#pragma once

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>

const float PI = 3.1415926;

class Vector2
{
public:
	float x, y;
	Vector2(void);
	Vector2(const Vector2&);
	//Vector2(int&, int&);
	Vector2(float&, float&);
	Vector2(float, float);
	~Vector2();
	void Zero();
	void setValue(float, float);
	void Rotation(float);
	void SetAngle(float);

public:
	// ÔËËã·ûºÅ
	friend Vector2 operator +(const Vector2& Vx, const Vector2& Vy);
	friend Vector2 operator -(const Vector2& Vx, const Vector2& Vy);
	friend Vector2 operator *(const Vector2& Vx, const float& Py);
	friend Vector2 operator /(const Vector2& Vx, const float& Py);
	friend Vector2 operator *(const float& Py, const Vector2& Vx);

public:
	// ÔËËã·ûºÅ
	Vector2& operator =(const Vector2& Vx);
	Vector2& operator +=(const Vector2& Vx);
	Vector2& operator -=(const Vector2& Vx);
	Vector2& operator *=(const float& Px);
	Vector2& operator /=(const float& Px);
};

#endif