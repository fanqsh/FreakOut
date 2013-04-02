#include "BallClass.h"

BallClass::BallClass(void)
{
	m_ballSize = 5;
	m_position.Zero();
	m_speed.Zero();
    m_speed.setValue(-0.7, -0.7);
	m_color = RGB(rand()%255, rand()%255, rand()%255);
}

BallClass::BallClass(const BallClass&)
{
}

BallClass::~BallClass(void)
{
}

void BallClass::Initialize(int size /* = 5 */, Vector2& position /* = Vector2(0.0f, 0.0f) */, float speedAngle /* = Vector2(0.0f, 0.0f) */)
{
	m_ballSize = size;
	m_position = position;
	m_speed.SetAngle(speedAngle);
}

void BallClass::SetSpeedDirect(float angle)
{
    m_speed.SetAngle(angle);
}

void BallClass::GetForwardPosition(Vector2& position)
{
	position.x = m_position.x + m_speed.x;
	position.y = m_position.y + m_speed.y;
	return;
}

void BallClass::GetForwardCenter(Vector2& position)
{
	position.x = m_position.x + (m_ballSize / 2);
	position.y = m_position.y + (m_ballSize / 2);

	return;
}

Vector2 BallClass::GetPosition(Vector2& position)
{
	position = m_position;
	return m_position;
}

void BallClass::SetBallColor(COLORREF col)
{
	m_color = col;
	return;
}

void BallClass::SetPosition(Vector2& position)
{
	m_position = position;
}

void BallClass::GoForward(int steps)
{
	m_position.x += m_speed.x * steps;
	m_position.y += m_speed.y * steps;
}

void BallClass::HasHit(EHitType type, float percentage)
{
	switch (type)
	{
	case BallClass::EHitLeft:
		m_speed.x = -m_speed.x;
		break;
	case BallClass::EHitTop:
		m_speed.y = -m_speed.y;
		break;
	case BallClass::EHitRight:
		m_speed.x = -m_speed.x;
		break;
	case BallClass::EHitBottom:
		m_speed.y = -m_speed.y;
		break;
	default:
		break;
	}

	if (percentage < 1.0 && percentage > -1.0)
		m_speed.SetAngle(PI/2 - percentage * PI/3);
}
