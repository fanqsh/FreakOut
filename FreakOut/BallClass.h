#pragma once
#include <Windows.h>
#include <list>
#include "common.h"
class Vector2
{
public:
	int x, y;
	Vector2(void);
	Vector2(const Vector2&);
	Vector2(int, int);
	~Vector2();
	void Zero();
};

class BallClass
{
public:
	enum EHitType
	{
		EHitNone = 100,
		EHitLeft = 10000,
		EHitTop,
		EHitRight,
		EHitBottom
	};
public:
	BallClass(void);
	BallClass(const BallClass&);
	~BallClass(void);

	void Initialize(int size = 5, Vector2& position = Vector2(0.0f, 0.0f), Vector2& speed = Vector2(0.0f, 0.0f));

	Vector2 GetPosition(Vector2& position);
	void GetForwardPosition(Vector2& position);
	void GetForwardCenter(Vector2& position);
	void SetBallColor(COLORREF);
	void SetPosition(Vector2& position);
	void AddHitPosition(Vector2& hitPosition);
	void ResetHitPosition();

	void GoForward(int steps);

	//½øÐÐÅö×²
	void HasHit(EHitType type);
	int m_ballSize;
	Vector2 m_speed;
private:

	COLORREF m_color;
	Vector2 m_position;
	std::list<Vector2> m_hitPosition;
};

