#include "GameClass.h"

GameClass::GameClass()
{
	//m_game_rect_width = 600;
	//m_game_rect_height = 600;
	//m_game_rect_leftOffset = 20;
	//m_game_rect_topOffset = 20;

	//m_block_rows = 5;
	//m_block_columns = 10;

	//m_block_dx = 5;
	//m_block_dy = 10;

	//m_block_height = 25;
	//m_block_width = 50;

	//m_block_leftOffset = (m_game_rect_width - (m_block_dx + m_block_width) * m_block_columns + m_block_dx) / 2;
	//m_block_topOffset = 10;
	m_isPlayed = true;
	m_isPaused = false;
	m_isPass = false;	//上个状态为过关
	m_isDie = false;		//上个状态为失败一次
	m_isOver = false;	//上个状态为结束游戏

	m_ballTime = 0.0f;
	m_bottomTime = 0.0f;
	m_blocks = NULL;
	m_ball = NULL;
	m_bottomBlock = NULL;
	m_moveDir.Zero();
	m_currentMoveDir.Zero();
	m_state = GameState::Game_Initialize;
	m_score = 0;
	m_lifeNum = 3;
	m_currentLevel = 1;
    m_ballSpeed = 8;    
	m_blockLast = m_block_rows * m_block_columns;
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
	Vector2 vec2(-0.7, -0.7);
	m_ball->Initialize(15, vec);

	m_bottomBlock = new BottomBlock;
	if (!m_bottomBlock)
		return false;
	vec2.setValue(100, 15);
	vec.setValue(vec.x - (vec2.x - m_ball->m_ballSize)/2, vec.y + m_ball->m_ballSize);
	m_bottomBlock->Initilaize(vec, vec2);

	return true;
}

bool GameClass::Frame(HDC hdc, float spacingTime)
{
	if (m_state == GameState::Game_Run)
	{
		if (m_isPaused)
		{
			m_isPaused = false;
			RebuildSences(hdc);
		}
        
		if (m_isDie)
		{
			m_isDie = false;
			ResetGame(hdc);
		}

        if (m_isPass)
        {
            m_isPass = false;
            ResetGame(hdc);
        }

		//处理球和挡板用的时间
		m_ballTime += spacingTime;
		m_bottomTime += spacingTime;

		//刷新球
		if (m_ballTime > 10.0f)
		{
			Vector2 hitBlock;
			hitBlock.x = -5;

			CheckHit(hitBlock);
			if (hitBlock.x >= 0)
			{
				//char logTmp[20];
				//sprintf(logTmp, "x = %d, y = %d", hitBlock.x, hitBlock.y);
				//LOG(logTmp);
                m_blocks[(int)hitBlock.y][(int)hitBlock.x] = 1;
                --m_blockLast;

                if (m_blockLast <= 0)
                    this->m_state = Game_Pass;

				Draw_Rect(hdc, hitBlock.x  * (m_block_width + m_block_dx) + m_block_leftOffset + m_game_rect_leftOffset, 
					hitBlock.y * (m_block_height + m_block_dy) + m_block_topOffset + m_game_rect_topOffset, 
					m_block_width, m_block_height, 1);
			}

			Delete_Ball(hdc);
			m_ball->GoForward(m_ballSpeed);
			Draw_Ball(hdc);

			m_ballTime = 0.0f;
		}

		//刷新底部挡板
		if (m_bottomTime > 10.0f)
		{
			Vector2 position;
			Vector2 size;
			m_bottomBlock->GetPosition(position);
			m_bottomBlock->GetSize(size);
			if (position.x + m_moveDir.x < m_game_rect_leftOffset)
			{
				m_moveDir.x = m_game_rect_leftOffset - position.x;
			}
			else if (position.x + m_moveDir.x + size.x > m_game_rect_leftOffset + m_game_rect_width)
			{
				m_moveDir.x = m_game_rect_width + m_game_rect_leftOffset - position.x - size.x;
			}

			Delete_BottomBlock(hdc);
			m_bottomBlock->makeMove(m_moveDir);
			Draw_BottomBlock(hdc);

			m_bottomTime = 0.0f;
		}
		
		return true;
	}
	else if (m_state == GameState::Game_Pause)
	{
		if (m_isPaused)
			return true;

		m_isPaused = true;
		//Draw_PauseInfo(hdc);
		Draw_PassInfo(hdc);
		return true;
	}
	else if (m_state == GameState::Game_Pass)
	{
        if (m_isPass)
            return true;

        m_isPass = true;
		++m_currentLevel;
        ++m_ballSpeed;
	}
	else if (m_state == GameState::Game_Over)
	{
		if (m_isDie)
			return true;
		m_isDie = true;
		if (m_lifeNum > 0)
		{
			--m_lifeNum;
			Draw_DieInfo(hdc);
		}
	}
	else if (m_state == GameState::Game_Initialize)
	{
		InitiSences(hdc);
	}
	return true;
}

#pragma region 辅助性函数

//字符型转换为宽字符，使用后需自己释放掉宽字符内存
wchar_t* charToWchar(char* const tmp)
{
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, tmp, -1, NULL, 0);

	wchar_t* wchar_tmp = new wchar_t[dwNum];
	if (!wchar_tmp)
	{
		delete[] wchar_tmp;
		return false;
	}
	MultiByteToWideChar(CP_ACP, 0, tmp, -1, wchar_tmp, dwNum);

	return wchar_tmp;
}

#pragma endregion

#pragma region 画图

#pragma region 画矩形­

void GameClass::Draw_Rect(HDC hdc, int verticalOffset , int horizontalOffset, int width/* = 50 */, int height/* = 25 */, int i /* = 0*/)
{
	RECT rect;

	rect.top = horizontalOffset;
	rect.left = verticalOffset;
	rect.bottom = horizontalOffset + height;
	rect.right = verticalOffset + width;

	if (i == 0)
	{
		HBRUSH hbrush = CreateSolidBrush(RGB(100, 0, 0));
		FillRect(hdc, &rect, hbrush);
		DeleteObject(hbrush);
	}
	else
	{
		HBRUSH hbrush2 = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rect, hbrush2);
		DeleteObject(hbrush2);
	}
}

void GameClass::Draw_Rect(HDC hdc, int verticalOffset , int horizontalOffset, int width/* = 50 */, int height/* = 25 */, HBRUSH* hbrushTmp /* = NULL */)
{
	RECT rect;

	rect.top = horizontalOffset;
	rect.left = verticalOffset;
	rect.bottom = horizontalOffset + height;
	rect.right = verticalOffset + width;

	if (NULL != hbrushTmp)
	{
		FillRect(hdc, &rect, *hbrushTmp);
	}
}

#pragma endregion

#pragma region 画球

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

#pragma endregion

#pragma region 画砖头

void GameClass::Init_Block(HDC hdc)
{
	//重置砖块
	if (m_blocks == NULL)
	{
		m_blocks = new int*[m_block_rows];

		for (int i = 0; i < m_block_rows; ++i)
		{
			m_blocks[i] = new int[m_block_columns]();
		}
	}
	else
	{
		for (int i = 0; i < m_block_rows; ++i)
		{
			for (int j = 0; j < m_block_columns; ++j)
			{
				m_blocks[i][j] = 0;
			}
		}
	}
	//重置砖块剩余数量
	m_blockLast = m_block_rows * m_block_columns;

	//重画所有砖块
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

void GameClass::Draw_Block(HDC hdc)
{
    int blockLast = 0;
	for (int i = 0; i < m_block_rows; ++i)
	{
		for (int j = 0; j < m_block_columns; ++j)
		{
			if (m_blocks[i][j] == 0)
            {
				Draw_Rect(hdc,
				j * ( m_block_width + m_block_dx ) + m_block_leftOffset + m_game_rect_leftOffset, 
				i * ( m_block_height + m_block_dy ) + m_block_topOffset + m_game_rect_topOffset ,
				m_block_width, m_block_height);
                ++blockLast;
            }
			else if (m_blocks[i][j] == 1)
            {
				Draw_Rect(hdc,
				j * ( m_block_width + m_block_dx ) + m_block_leftOffset + m_game_rect_leftOffset, 
				i * ( m_block_height + m_block_dy ) + m_block_topOffset + m_game_rect_topOffset ,
				m_block_width, m_block_height, 1);
            }
        }
	}

    if (0 == blockLast)
    {
        Init_Block(hdc);
    }
}
#pragma endregion

#pragma region 画底座

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

	HBRUSH hbrush = CreateSolidBrush(RGB(20, 80, 0));

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

#pragma endregion

#pragma region 暂停界面

void GameClass::Draw_PauseInfo(HDC hdc)
{
	int height = 100;
	int width = 500;
	COLORREF	clr;
	//char ccBuffer[] = "Pause";
	//TCHAR szBuffer[] = TEXT(ccBuffer);

	TCHAR szBuffer[] = TEXT("Pause");
	TCHAR szBufferMsg[] = TEXT("按空格键继续！");

	Draw_Pandle(hdc, width, height);


	//背景色透明
	SetBkMode(hdc, TRANSPARENT);
	//设置颜色并输出文字
	clr = SetTextColor(hdc, RGB(255, 0, 0));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 25, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 20, szBuffer, lstrlen(szBuffer));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 45, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szBufferMsg, lstrlen(szBufferMsg));
	//还原颜色
	SetTextColor(hdc, clr);
}

//void GameClass::Delete_PauseInfo(HDC hdc)
//{
//	int width = 400;
//	int height = 300;
//
//	Draw_Rect(hdc, 
//		(m_game_rect_width - width)/2 + m_game_rect_leftOffset, 
//		(m_game_rect_height - height)/2 + m_game_rect_topOffset, 
//		width, height, 1);
//}

#pragma endregion 

#pragma region 过关界面

void GameClass::Draw_PassInfo(HDC hdc)
{
	int height = 200;
	int width = 500;
	COLORREF	clr;
	char tmp[5];
	wchar_t* szLevel = NULL;
	wchar_t* szLifeNum = NULL;
	sprintf(tmp, "%d", m_currentLevel);
	szLevel = charToWchar(tmp);
	sprintf(tmp, "%d", m_lifeNum);
	szLifeNum = charToWchar(tmp);

	TCHAR szBuffer[] = TEXT("K O ! !");
	TCHAR szLevelInfo[] = TEXT("当前关卡：");
	TCHAR szLifeInfo[] = TEXT("剩余生命数量：");

	TCHAR szBufferMsg[] = TEXT("按空格键进入下一关！");

	Draw_Pandle(hdc, width, height);

	//背景色透明
	SetBkMode(hdc, TRANSPARENT);
	//设置颜色并输出文字
	clr = SetTextColor(hdc, RGB(255, 0, 0));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 15, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 20, szBuffer, lstrlen(szBuffer));

	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 45, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szLevelInfo, lstrlen(szLevelInfo));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset + 33, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szLevel, lstrlen(szLevel));

	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 55, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 80, szLifeInfo, lstrlen(szLifeInfo));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset + 43, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 80, szLifeNum, lstrlen(szLifeNum));

	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 45, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 110, szBufferMsg, lstrlen(szBufferMsg));
	//还原颜色
	SetTextColor(hdc, clr);
	
}

#pragma endregion

#pragma region GameOver界面

void GameClass::Draw_GameOverInfo(HDC hdc)
{
	int height = 100;
	int width = 500;
	COLORREF	clr;
	TCHAR szBuffer[] = TEXT("~~~~");
	TCHAR szBufferMsg[] = TEXT("按空格键重新开始！");

	Draw_Pandle(hdc, width, height);


	//背景色透明
	SetBkMode(hdc, TRANSPARENT);
	//设置颜色并输出文字
	clr = SetTextColor(hdc, RGB(255, 0, 0));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 25, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 20, szBuffer, lstrlen(szBuffer));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 45, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szBufferMsg, lstrlen(szBufferMsg));
	//还原颜色
	SetTextColor(hdc, clr);
}

#pragma endregion

#pragma region 失败界面

void GameClass::Draw_DieInfo(HDC hdc)
{
	int height = 100;
	int width = 500;
	COLORREF	clr;
	char tmp[5];
	wchar_t* szLifeNum = NULL;
	sprintf(tmp, "%d", m_lifeNum);
	szLifeNum = charToWchar(tmp);

	TCHAR szLifeInfo[] = TEXT("剩余生命数量：");
	TCHAR szBuffer[] = TEXT("YOU ARE DEAD！");
	TCHAR szBufferMsg[] = TEXT("按空格键继续！");

	Draw_Pandle(hdc, width, height);

	//背景色透明
	SetBkMode(hdc, TRANSPARENT);
	//设置颜色并输出文字
	clr = SetTextColor(hdc, RGB(255, 0, 0));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 35, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 20, szBuffer, lstrlen(szBuffer));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 55, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szLifeInfo, lstrlen(szLifeInfo));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset + 43, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 50, szLifeNum, lstrlen(szLifeNum));
	TextOut(hdc, m_game_rect_width/2 + m_game_rect_leftOffset - 45, (m_game_rect_height - height)/2 + m_game_rect_topOffset + 80, szBufferMsg, lstrlen(szBufferMsg));
	//还原颜色
	SetTextColor(hdc, clr);

	delete[] szLifeNum;
}

#pragma endregion

#pragma region 画提示板

void GameClass::Draw_Pandle(HDC hdc, int width /* = 400 */, int height /* = 300 */, int leftOffset /* = 0 */, int topOffset /* = 0 */, HBRUSH* hbrush /* = NULL */)
{
	HBRUSH hbrush2 = CreateSolidBrush(RGB(100, 100, 100));

	Draw_Rect(hdc, 
		(m_game_rect_width - width)/2 + m_game_rect_leftOffset, 
		(m_game_rect_height - height)/2 + m_game_rect_topOffset, 
		width, height, hbrush == NULL ? &hbrush2 : hbrush);

	DeleteObject(hbrush2);
}

#pragma endregion 


#pragma endregion

#pragma region 碰撞判断

#pragma region 检测碰撞

void GameClass::CheckHit(Vector2& hitBlock)
{
	Vector2 vec;
	Vector2 ballCenter;
	float persentage = 10.0f;
	hitBlock.x = -5;
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
	else if (m_bottomBlock->CheckHit(vec, m_ball->m_speed, persentage, m_ball->m_ballSize))
		type = BallClass::EHitBottom;

	m_ball->HasHit(type, persentage);

	if (vec.y > m_game_rect_height)
		m_state = GameState::Game_Over;

	return;
}

#pragma endregion

#pragma region 检测点是否在矩形内

bool GameClass::IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom)
{
	if (point.x <= rightBottom.x && point.x >= leftTop.x && point.y >= leftTop.y && point.y <= rightBottom.y)
		return true;

	return false;
}

#pragma endregion 

#pragma region 检测是否碰撞到线段

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

#pragma endregion 

#pragma region 检测两个矩形是否有重合

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

#pragma endregion 

#pragma endregion

//初始化场景
void GameClass::InitiSences(HDC hdc)
{
	if (!m_isPlayed)
		return;

	RECT rect;
	Vector2 vec(400, 580);
	Vector2 vec2(-1, -1);

	rect.top = m_game_rect_topOffset;
	rect.bottom = m_game_rect_topOffset + m_game_rect_height;
	rect.left = m_game_rect_leftOffset;
	rect.right = m_game_rect_leftOffset + m_game_rect_width;

	HBRUSH hbrush = CreateSolidBrush(RGB(0,0,0));
	HBRUSH hbrush2 = CreateSolidBrush(RGB(100,0,0));

    m_ballSpeed = 15;

	FillRect(hdc, &rect, hbrush);
	FrameRect(hdc, &rect, hbrush2);
	Init_Block(hdc);

	m_ball->Initialize(15, vec);
	vec2.setValue(600, 15);
	vec.setValue(vec.x - (vec2.x - m_ball->m_ballSize)/2, vec.y + m_ball->m_ballSize);
	m_bottomBlock->Initilaize(vec, vec2);

	Draw_Ball(hdc);
	Draw_BottomBlock(hdc);

	DeleteObject(hbrush);
	DeleteObject(hbrush2);

	m_isPlayed = false;
}

void GameClass::RebuildSences(HDC hdc)
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
	Draw_Block(hdc);

	Draw_Ball(hdc);

	Draw_BottomBlock(hdc);

	DeleteObject(hbrush);
	DeleteObject(hbrush2);
}

void GameClass::ResetGame(HDC hdc)
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
	Draw_Block(hdc);

	Vector2 vec(400, 580);
	Vector2 vec2(-1, -1);
	m_ball->Initialize(15, vec);

	vec2.setValue(100, 15);
	vec.setValue(vec.x - (vec2.x - m_ball->m_ballSize)/2, vec.y + m_ball->m_ballSize);
	m_bottomBlock->Initilaize(vec, vec2);

	Draw_Ball(hdc);
	Draw_BottomBlock(hdc);

	DeleteObject(hbrush);
	DeleteObject(hbrush2);
}

void GameClass::Shutdown()
{
	if (m_blocks != NULL)
	{
		for (int i = 0; i < m_block_rows; ++i)
		{
			delete[] m_blocks[i];
			m_blocks[i] = NULL;
		}

		delete[] m_blocks;
		m_blocks = NULL;
	}
}


//
//int WINAPI WinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nShowCmd)
//{
//	HDC			hdc;
//	HWND		hwnd;
//	COLORREF	clr;
//	TCHAR		szBuffer[] = TEXT("Http://Www.ProgramLife.Net");
//
//	//获取桌面窗口句柄
//	hwnd = GetDesktopWindow();
//	//获取桌面窗口DC
//	hdc = GetWindowDC(hwnd);
//	//背景色透明
//	SetBkMode(hdc, TRANSPARENT);
//	//设置颜色并输出文字
//	clr = SetTextColor(hdc, RGB(255, 0, 0));
//	TextOut(hdc, 0, 0, szBuffer, lstrlen(szBuffer));
//	SetTextColor(hdc, RGB(0, 255, 0));
//	TextOut(hdc, 0, 20, szBuffer, lstrlen(szBuffer));
//	//背景色不透明
//	SetBkMode(hdc, OPAQUE);
//	SetTextColor(hdc, RGB(0, 0, 255));
//	TextOut(hdc, 0, 40, szBuffer, lstrlen(szBuffer));
//	//还原颜色
//	SetTextColor(hdc, clr);
//	//释放句柄DC
//	ReleaseDC(hwnd, hdc);
//
//	Sleep(5000);
//
//	return 0;
//}
