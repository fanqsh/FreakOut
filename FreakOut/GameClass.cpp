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
	m_bottomBlock = NULL;
	m_moveDir.Zero();
	m_state = GameState::Game_Initialize;
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
	if (!m_ball)
		return false;

	Vector2 vec(400, 580);
	Vector2 vec2(-1, -1);
	m_ball->Initialize(15, vec, vec2);

	m_bottomBlock = new BottomBlock;
	if (!m_bottomBlock)
		return false;
	vec2.setValue(100, 15);
	vec.setValue(vec.x - (vec2.x - m_ball->m_ballSize)/2, vec.y + m_ball->m_ballSize);
	m_bottomBlock->Initilaize(vec, vec2);
}

bool GameClass::Frame(HDC hdc, float spacingTime)
{
	Vector2 hitBlock;
	hitBlock.x = -5;
	m_spacingTime += spacingTime;

	Delete_BottomBlock(hdc);
	m_bottomBlock->makeMove(m_moveDir);
	m_moveDir.Zero();
	Draw_BottomBlock(hdc);
	
	if (m_spacingTime > 20.0f)
	{
		CheckHit(hitBlock);
		if ( -5 != hitBlock.x)
		{
			//char logTmp[20];
			//sprintf(logTmp, "x = %d, y = %d", hitBlock.x, hitBlock.y);
			//LOG(logTmp);

			Draw_Rect(hdc, hitBlock.x  * (m_block_width + m_block_dx) + m_block_leftOffset + m_game_rect_leftOffset, 
				hitBlock.y * (m_block_height + m_block_dy) + m_block_topOffset + m_game_rect_topOffset, 
				m_block_width, m_block_height, 1);
		}

		Delete_Ball(hdc);
		m_ball->GoForward(5);
		Draw_Ball(hdc);

		m_spacingTime = 0.0f;
	}

	return true;
}

void GameClass::Draw_Rect(HDC hdc, int verticalOffset , int horizontalOffset, int width/* = 50 */, int height/* = 25 */, int i /* = 0*/)
{
	RECT rect;

	rect.top = horizontalOffset;
	rect.left = verticalOffset;
	rect.bottom = horizontalOffset + height;
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
	Ellipse(hdc, vec.x, vec.y, vec.x + m_ball->m_ballSize, vec.y + m_ball->m_ballSize);
}

void GameClass::Delete_Ball(HDC hdc)
{
	Vector2 vec;
	m_ball->GetPosition(vec);
	RECT rect;
	rect.top = vec.y;
	rect.bottom = vec.y + m_ball->m_ballSize;
	rect.left = vec.x;
	rect.right = vec.x + m_ball->m_ballSize;
	HBRUSH hbrush = CreateSolidBrush(RGB(0,0,0));
	
	FillRect(hdc, &rect, hbrush);

	DeleteObject(hbrush);
}

void GameClass::Draw_BottomBlock(HDC hdc)
{
	Vector2 position;
	Vector2 size;
	RECT rect;
	m_bottomBlock->GetPosition(position);
	m_bottomBlock->GetSize(size);

	rect.top = position.y;
	rect.left = position.x;
	rect.bottom = position.y + size.y;
	rect.right = position.x + size.x;

	HBRUSH hbrush = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));

	FillRect(hdc, &rect, hbrush);
	DeleteObject(hbrush);
}

void GameClass::Delete_BottomBlock(HDC hdc)
{
	Vector2 position;
	Vector2 size;
	RECT rect;
	m_bottomBlock->GetPosition(position);
	m_bottomBlock->GetSize(size);

	rect.top = position.y;
	rect.left = position.x;
	rect.bottom = position.y + size.y;
	rect.right = position.x + size.x;

	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));

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
	Draw_BottomBlock(hdc);	
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
	Vector2 ballCenter;
	hitBlock.x = -1;
	BallClass::EHitType type = BallClass::EHitNone;
	m_ball->GetForwardPosition(vec);
	m_ball->GetForwardCenter(ballCenter);

	if (vec.y < m_block_rows * ( m_block_height + m_block_dy ) + m_game_rect_topOffset + m_block_topOffset)
	{
		for (int i = m_block_rows - 1; i >= 0; --i)
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
					
					if (m_blocks[i][j] == 0 && IsInRect(vec, ballRightBottom, blockLeftTop, blockRightBottom) )
					{
						char logTmp[30];
						if (m_ball->m_speed.x > 0 && !IsHitInLevel(ballCenter, m_ball->m_speed, blockLeftTop))
						{
							sprintf(logTmp, "x = %d, y = %d type = Left", j, i);
							type = BallClass::EHitLeft;
						}
						else if (m_ball->m_speed.y > 0 && IsHitInLevel(ballCenter, m_ball->m_speed, blockLeftTop))
						{
							sprintf(logTmp, "x = %d, y = %d type = Top", j, i);
							type = BallClass::EHitTop;
						}
						else if (m_ball->m_speed.x < 0 && !IsHitInLevel(ballCenter, m_ball->m_speed, blockLeftTop))
						{
							sprintf(logTmp, "x = %d, y = %d type = Right", j, i);
							type = BallClass::EHitRight;
						}

						else if (m_ball->m_speed.y < 0 && IsHitInLevel(ballCenter, m_ball->m_speed, blockLeftTop))
						{
							sprintf(logTmp, "x = %d, y = %d type = Bottom", j, i);
							type = BallClass::EHitBottom;
						}

						LOG(logTmp);

						m_blocks[i][j] = 1;
						hitBlock.x = j;
						hitBlock.y = i;
						break;
					}
				}
			}
		}
	}

	if (vec.x - m_ball->m_ballSize < m_game_rect_leftOffset)
		type = BallClass::EHitLeft;
	else if (vec.x + m_ball->m_ballSize > m_game_rect_width + m_game_rect_leftOffset)
		type = BallClass::EHitRight;
	else if (vec.y - m_ball->m_ballSize < m_game_rect_topOffset)
		type = BallClass::EHitTop;
	else if (m_bottomBlock->CheckHit(vec, m_ball->m_speed, m_ball->m_ballSize))
		type = BallClass::EHitBottom;

	m_ball->HasHit(type);


	if (vec.y > m_game_rect_height)
		m_state = GameState::Game_Over;

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

bool GameClass::IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop)
{
	float dx = 0.0f;
	float k = 0.0f;
	k = speed.x / speed.y;

	if (speed.y < 0)
		dx = k * (blockLeftTop.y + m_block_height);
	else
		dx = k * blockLeftTop.y;

	dx = dx +  position.x - (k * position.y);
	if (dx > blockLeftTop.x && dx < blockLeftTop.x + m_block_width)
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
