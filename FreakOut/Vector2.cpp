#include "Vector2.h"

Vector2::~Vector2()
{
}

Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const Vector2&)
{
}

Vector2::Vector2(float px, float py)
{
	x = px;
	y = py;
}

Vector2::Vector2(float& px, float& py)
{
	x = px;
	y = py;
}

void Vector2::setValue(float px, float py)
{
	this->x = px;
	this->y = py;
}

void Vector2::Zero()
{
	x = 0;
	y = 0;
}

void Vector2::Rotation(float angle)
{
	float _x = this->x;
	float _y = this->y;
	x = _x * cos(angle) + _y * sin(angle);
	y = -_x * sin(angle) + _y * cos(angle);
}

void Vector2::SetAngle(float angle)
{
	float len = sqrt(x * x + y * y);
	this->y = - len * sin(angle);
	this->x = len * cos(angle);
}

//运算符重载
///////////////////////////////////////////////////////////
Vector2& Vector2::operator = (const Vector2& Vy)
{
	this->x = Vy.x;
	this->y = Vy.y;
	return (*this);
}

Vector2& Vector2::operator += (const Vector2& Vy)
{
	this->x += Vy.x;
	this->y += Vy.y;
	return (*this);
}

Vector2& Vector2::operator -= (const Vector2& Vy)
{
	this->x -= Vy.x;
	this->y -= Vy.y;
	return (*this);
}

Vector2& Vector2::operator *= (const float& Py)
{
	this->x *= Py;
	this->y *= Py;
	return (*this);
}

Vector2& Vector2::operator /= (const float& Py)
{
	this->x /= Py;
	this->y /= Py;
	return (*this);
}

//友元运算符重载
///////////////////////////////////////////////////////////

Vector2 operator + (const Vector2& Vx, const Vector2& Vy)
{
	Vector2 tmp;
	tmp = Vx;
	tmp += Vy;
	return tmp;
}

Vector2 operator - (const Vector2& Vx, const Vector2& Vy)
{
	Vector2 tmp;
	tmp = Vx;
	tmp -= Vy;
	return tmp;
}

Vector2 operator * (const Vector2& Vx, const float& Py)
{
	Vector2 tmp;
	tmp = Vx;
	tmp.x *= Py;
	tmp.y *= Py;
	return tmp;
}

Vector2 operator / (const Vector2& Vx, const float& Py)
{
	Vector2 tmp;
	tmp = Vx;
	tmp.x /= Py;
	tmp.y /= Py;
	return tmp;
}

Vector2 operator * (const float& Py, const Vector2& Vx)
{
	Vector2 tmp;
	tmp = Vx;
	tmp.x *= Py;
	tmp.y *= Py;
	return tmp;
}

