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

bool BottomBlock::CheckHit(Vector2& position, Vector2& speed, float& persentage, int ballSize /* = 0 */)
{
	if (speed.y < 0)
		return false;
	if (position.y + ballSize < m_position.y)
		return false;

	float dx = 0.0f;
	float k = 0.0f;
	k = speed.x / speed.y;
	dx = k * m_position.y;

	//计算以撞击点所在各个半边所处百分比，两个半边均设挡板中间为 0 点，左半边为负，右半边为正
	//即撞击到中间为0，最左边为 -1，最右边为 1
	dx = dx +  (position.x + ballSize) - (k * (position.y - ballSize));
	if (dx > m_position.x - ballSize && dx < m_position.x + m_size.x + ballSize)
	{
		persentage = 2 * (dx - m_position.x + ballSize)/(m_size.x + 2 * ballSize) - 1.0f;
		if (persentage < -0.000001)
			persentage = -1.0f - persentage;
		return true;
	}
	return false;
}