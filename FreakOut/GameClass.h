#ifndef GAMECLASS_H   
#define GAMECLASS_H   

#include <Windows.h>
#include "BallClass.h"
#include "BottomBlock.h"
#include "log.h"

class GameClass
{
public:
	enum GameState
	{
		GameStateFirst = 10000,
		Game_Initialize,
		Game_Run,
		Game_Pass,
		Game_Over,
		GameStateLast
	};

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
	enum GameOffset
	{
		m_game_rect_leftOffset = 20,	//游戏区域相对窗口左侧偏移
		m_game_rect_topOffset = 20,		//游戏区域相对窗口上部偏移

		m_game_rect_height = 600,		//游戏区域高度
		m_game_rect_width = 600,		//游戏区域宽度

		m_block_rows = 5,
		m_block_columns = 10,

		m_block_dx = 5,
		m_block_dy = 10,

		m_block_height = 25,
		m_block_width = 50,

		m_block_leftOffset = (m_game_rect_width - (m_block_dx + m_block_width) * m_block_columns + m_block_dx) / 2,
		m_block_topOffset = 10,
	};

public:
	GameClass();
	GameClass(const GameClass&);
	~GameClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, HDC hdc);
	void Shutdown();
	bool Frame(HDC, float);
	void InitiSences(HDC hdc);
	void Draw_Rect(HDC hdc, int horizontalOffset, int verticalOffset, int width = 50, int height = 25, int i = 0);

	GameState m_state;
	Vector2 m_moveDir;	//底部砖块移动积累量
private:
	bool Render();

	#pragma region 碰撞检测
	void CheckHit(Vector2&);
	bool IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom);
	bool IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom);
	bool IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop);
	#pragma endregion

	#pragma region 画图

	void Init_Block(HDC hdc);

	void Draw_Ball(HDC hdc);
	void Delete_Ball(HDC hdc);

	void Draw_BottomBlock(HDC hdc);
	void Delete_BottomBlock(HDC hdc);

	void Draw_InitInfo(HDC hdc, string title);
	void Draw_PassInfo(HDC hdc, string title);

	#pragma endregion

	#pragma region 系统相关参数
	float m_spacingTime;	//间隔时间
	BallClass* m_ball;		//球
	BottomBlock* m_bottomBlock;		
	int** m_blocks;
	HDC m_hdc;

	#pragma endregion

	#pragma region 位置参数
	int m_screenHeight;
	int m_screenWidth;

	//int m_game_rect_leftOffset;	//游戏区域相对窗口左侧偏移
	//int m_game_rect_topOffset;	//游戏区域相对窗口上部偏移

	//int m_game_rect_height;	//游戏区域高度
	//int m_game_rect_width;	//游戏区域宽度

	//int m_block_rows;		//砖块行数
	//int m_block_columns;	//砖块列数

	//int m_block_width;		//砖块宽度
	//int m_block_height;		//砖块高度
	//int m_block_dx;			//砖块左侧空白长度
	//int m_block_dy;			//砖块下部空白长度
	//int m_block_topOffset;	//砖块整体区域相对游戏区域上部偏移
	//int m_block_leftOffset;	//砖块整体区域相对游戏区域左侧偏移
	#pragma endregion

};


#endif
