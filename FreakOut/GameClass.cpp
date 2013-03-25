#include "GameClass.h"

GameClass::GameClass()
{
	m_game_rect_width = 600;
	m_game_rect_height = 600;
	m_game_rect_leftOffset = 20;
	m_game_rect_topOffset = 20;

	m_block_rows = 5;
	m_block_columns = 10;

	m_block_dx = 5;
	m_block_dy = 10;

	m_block_height = 25;
	m_block_width = 50;

	m_block_leftOffset = (m_game_rect_width - (m_block_dx + m_block_width) * m_block_columns + m_block_dx) / 2;
	m_block_topOffset = 10;

	m_spacingTime = 0.0f;
	m_ball = NULL;
}

GameClass::GameClass(const GameClass&)
{
}

GameClass::~GameClass()
{
}

bool GameClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, HDC hdc)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_ball = new BallClass;
	Vector2 vec(400.0f, 580.0f);
	Vector2 vec2(-5.0f, -5.0f);
	if (!m_ball)
		return false;
	m_ball->Initialize(5, vec, vec2);

}

bool GameClass::Frame(HDC hdc, float spacingTime)
{
	Vector2 hitBlock;
	hitBlock.x = -1;
	m_spacingTime += spacingTime;
	//InitiSences(hdc);

	if (m_spacingTime > 20.0f)
	{
		Delete_Ball(hdc);
		CheckHit(hitBlock);
		if ( -1 != hitBlock.x)
		{
			Draw_Rect(hdc, hitBlock.x  * (m_block_width + m_block_dx) + m_block_leftOffset + m_game_rect_leftOffset, 
				hitBlock.y * ((m_block_height + m_block_dy) + m_block_topOffset + m_game_rect_topOffset), 
				m_block_width, m_block_height, 1);
		}
		m_ball->GoForward(1);
		Draw_Ball(hdc);
		m_spacingTime = 0.0f;
	}
	
	return true;
}

void GameClass::Draw_Rect(HDC hdc, int verticalOffset , int horizontalOffset, int width/* = 50 */, int height/* = 25 */, int i /* = 0*/)
{
	RECT rect;

	rect.top = horizontalOffset + height;
	rect.left = verticalOffset;
	rect.bottom = horizontalOffset;
	rect.right = verticalOffset + width;

	HBRUSH hbrush = CreateSolidBrush(RGB(100, 0, 0));
	HBRUSH hbrush2 = CreateSolidBrush(RGB(0, 0, 0));

	if (i == 0)
		FillRect(hdc, &rect, hbrush);
	else
		FillRect(hdc, &rect, hbrush2);

	DeleteObject(hbrush);
	DeleteObject(hbrush2);

}

void GameClass::Draw_Ball(HDC hdc)
{
	Vector2 vec;
	m_ball->GetPosition(vec);
	Ellipse(hdc, vec.x, vec.y, vec.x + 15, vec.y + 15);
}

void GameClass::Delete_Ball(HDC hdc)
{
	Vector2 vec;
	m_ball->GetPosition(vec);
	RECT rect;
	rect.top = vec.y;
	rect.bottom = vec.y + 15;
	rect.left = vec.x;
	rect.right = vec.x + 15;
	HBRUSH hbrush = CreateSolidBrush(RGB(0,0,0));
	
	FillRect(hdc, &rect, hbrush);

	DeleteObject(hbrush);
}

void GameClass::InitiSences(HDC hdc)
{
	RECT rect;
	rect.top = m_game_rect_topOffset;
	rect.bottom = m_game_rect_topOffset + m_game_rect_height;
	rect.left = m_game_rect_leftOffset;
	rect.right = m_game_rect_leftOffset + m_game_rect_width;

	HBRUSH hbrush = CreateSolidBrush(RGB(0,0,0));
	HBRUSH hbrush2 = CreateSolidBrush(RGB(100,0,0));

	FillRect(hdc, &rect, hbrush);
	FrameRect(hdc, &rect, hbrush2);

	Init_Block(hdc);

	m_blocks = new int*[m_block_rows];
	for (int i = 0; i < m_block_rows; ++i)
	{
		m_blocks[i] = new int[m_block_columns]();
	}

	DeleteObject(hbrush);
	DeleteObject(hbrush2);
}

void GameClass::CheckHit(Vector2& hitBlock)
{
	Vector2 vec;
	hitBlock.x = -1;
	BallClass::EHitType type = BallClass::EHitNone;
	m_ball->GetForwardPosition(vec);

	if (vec.y < m_block_rows * ( m_block_height + m_block_dy ) + m_game_rect_topOffset + m_block_topOffset)
	{
		for (int i = m_block_rows; i > 0; --i)
		{
			if (vec.y < i * ( m_block_height + m_block_dy ) + m_game_rect_topOffset + m_block_topOffset + m_block_height)
			{
				for (int j = 0; j < m_block_columns; ++j)
				{
					Vector2 blockLeftTop;
					blockLeftTop.x = j * (m_block_width + m_block_dx) + m_block_leftOffset + m_game_rect_leftOffset;
					blockLeftTop.y = i * (m_block_height + m_block_dy) + m_block_topOffset + m_game_rect_topOffset;

					Vector2 blockRightBottom;
					blockRightBottom.x = j * (m_block_width + m_block_dx) + m_block_leftOffset + m_block_width + m_game_rect_leftOffset;
					blockRightBottom.y = i * (m_block_height + m_block_dy) + m_block_topOffset + m_block_height + m_game_rect_topOffset;

					Vector2 ballRightBottom;
					ballRightBottom.x = vec.x + m_ball->m_ballSize;
					ballRightBottom.y = vec.y + m_ball->m_ballSize;
					
					if (m_blocks[i - 1][j] == 0 && IsInRect(vec, ballRightBottom, blockLeftTop, blockRightBottom) )
					{
						if ((m_ball->m_speed.x > 0 && m_ball->m_speed.y > 0 && 
							vec.x + m_ball->m_ballSize - blockLeftTop.x > vec.y + m_ball->m_ballSize - blockLeftTop.y) ||
							(m_ball->m_speed.x > 0 && m_ball->m_speed.y < 0 &&
							vec.x + m_ball->m_ballSize - blockLeftTop.x > vec.y - blockRightBottom.y))
						{
							type = BallClass::EHitLeft;
						}
						else if ((m_ball->m_speed.x > 0 && m_ball->m_speed.y > 0 && 
								vec.x + m_ball->m_ballSize - blockLeftTop.x < vec.y + m_ball->m_ballSize - blockLeftTop.y) ||
								(m_ball->m_speed.x < 0 && m_ball->m_speed.y > 0 &&
								vec.x + m_ball->m_ballSize - blockLeftTop.x > vec.y - blockRightBottom.y))
						{
							type = BallClass::EHitTop;
						}
						else if ((m_ball->m_speed.x < 0 && m_ball->m_speed.y > 0 &&
							vec.y + m_ball->m_ballSize - blockLeftTop.y < vec.x - blockRightBottom.x) ||
							(m_ball->m_speed.x < 0 && m_ball->m_speed.y < 0 &&
							vec.x - blockRightBottom.x > vec.y - blockRightBottom.y))
						{
							type = BallClass::EHitRight;
						}

						else if ((m_ball->m_speed.x < 0 && m_ball->m_speed.y > 0 &&
							vec.x + m_ball->m_ballSize - blockLeftTop.x < vec.y - blockRightBottom.y) ||
							(m_ball->m_speed.x < 0 && m_ball->m_speed.y < 0 &&
							vec.x - blockRightBottom.x < vec.y - blockRightBottom.y))
						{
							type = BallClass::EHitBottom;
						}
						m_blocks[i - 1][j] = 1;
						hitBlock.x = j;
						hitBlock.y = i - 1;
					}
				}
			}
		}
	}

	if (vec.x - m_ball->m_ballSize - m_ball->m_ballSize < m_game_rect_leftOffset)
		type = BallClass::EHitLeft;
	else if (vec.x + ( 5 * m_ball->m_ballSize ) > m_game_rect_width + m_game_rect_leftOffset)
		type = BallClass::EHitRight;
	else if (vec.y - m_ball->m_ballSize - m_ball->m_ballSize < m_game_rect_topOffset)
		type = BallClass::EHitTop;
	else if (vec.y + m_ball->m_ballSize + m_ball->m_ballSize > m_game_rect_topOffset + m_game_rect_height)
		type = BallClass::EHitBottom;

	m_ball->HasHit(type);
	return;
}

void GameClass::Shutdown()
{

}

bool GameClass::IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom)
{
	if (point.x <= rightBottom.x && point.x >= leftTop.x && point.y >= leftTop.y && point.y <= rightBottom.y)
		return true;

	return false;
}

bool GameClass::IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom)
{
	Vector2 rightTop;
	rightTop.y = rectLeftTop.y;
	rightTop.x = rectRightBottom.x;

	Vector2 leftBottom;
	leftBottom.x = rectLeftTop.x;
	leftBottom.y = rectRightBottom.y;

	if (IsInRect(rectLeftTop, leftTop, rightBottom) || IsInRect(rectRightBottom, leftTop, rightBottom ) ||
		IsInRect(rightTop, leftTop, rightBottom) || IsInRect(leftBottom, leftTop, rightBottom))
		return true;

	return false;
}

void GameClass::Init_Block(HDC hdc)
{
	for (int i = 0; i < m_block_columns; ++i)
	{
		for (int j = 0; j < m_block_rows; ++j)
		{
			Draw_Rect(hdc, 
				i * ( m_block_width + m_block_dx ) + m_block_leftOffset + m_game_rect_leftOffset, 
				j * ( m_block_height + m_block_dy ) + m_block_topOffset + m_game_rect_topOffset ,
				m_block_width, m_block_height);
		}
	}
}

