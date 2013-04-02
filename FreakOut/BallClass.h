//#pragma once
#ifndef _BALLCLASS_H_
#define _BALLCLASS_H_   

#include <Windows.h>
#include "Vector2.h"

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
	
	void Initialize(int size = 5, Vector2& position = Vector2(0.0f, 0.0f), float speedAngle = -0.536);

	Vector2 GetPosition(Vector2& position);
	void GetForwardPosition(Vector2& position);
	void GetForwardCenter(Vector2& position);
	void SetBallColor(COLORREF);
	void SetPosition(Vector2& position);
	void AddHitPosition(Vector2& hitPosition);
	void ResetHitPosition();
	void GoForward(int steps);
    void SetSpeedDirect(float angle);       //设置方向

	//进行碰撞
	void HasHit(EHitType type, float persentage = 10.0f);
	int m_ballSize;
	Vector2 m_speed;
private:

	COLORREF m_color;
	Vector2 m_position;
};

#endif
