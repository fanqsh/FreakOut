#include "BottomBlock.h"

BottomBlock::BottomBlock()
{
	m_position.setValue(0,0);

	m_size.setValue(60, 15);
}

BottomBlock::BottomBlock(const BottomBlock&)
{
}

BottomBlock::~BottomBlock()
{
}

bool BottomBlock::Initilaize(Vector2& position, Vector2& size)
{
	if (position.x < 0 || position.y < 0 || size.x <= 0 || size.y <= 0)
		return false;

	m_position = position;
	m_size = size;

	return true;
}

void BottomBlock::makeMove(Vector2& step)
{
	m_position.x = m_position.x + step.x;
	m_position.y = m_position.y + step.y;
}

Vector2 BottomBlock::GetPosition(Vector2& position)
{
	position = m_position;
	return m_position;
}

Vector2 BottomBlock::GetSize(Vector2& size)
{
	size = m_size;
	return m_size;
}

bool BottomBlock::CheckHit(Vector2& position, Vector2& speed, int ballSize /* = 0 */)
{
	if (speed.y < 0)
		return false;
	if (position.y < m_position.y)
		return false;

	float dx = 0.0f;
	float k = 0.0f;
	k = speed.x / speed.y;
	dx = k * m_position.y;

	dx = dx +  position.x - (k * position.y);
	if (dx > m_position.x - ballSize && dx < m_position.x + m_size.x + ballSize)
		return true;

	return false;
}