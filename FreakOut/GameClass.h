#pragma once
#include <Windows.h>
#include "BallClass.h"
#include "BrickClass.h"

class GameClass
{
public:
	GameClass();
	GameClass(const GameClass&);
	~GameClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, HDC hdc);
	void Shutdown();
	bool Frame(HDC, float);
	void InitiSences(HDC hdc);
	void Draw_Rect(HDC hdc, int horizontalOffset, int verticalOffset, int width = 50, int height = 25, int i = 0);
	void Draw_Ball(HDC hdc);
	void Delete_Ball(HDC hdc);
private:
	bool Render();
	void CheckHit(Vector2&);
	bool IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom);
	bool IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom);
	void Init_Block(HDC hdc);

	bool IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop);

	float m_spacingTime;
	BallClass* m_ball;
	HDC m_hdc;

	int m_screenHeight;
	int m_screenWidth;

	int m_game_rect_leftOffset;	//游戏区域相对窗口左侧偏移
	int m_game_rect_topOffset;	//游戏区域相对窗口上部偏移

	int m_game_rect_height;	//游戏区域高度
	int m_game_rect_width;	//游戏区域宽度

	int m_block_rows;		//砖块行数
	int m_block_columns;	//砖块列数

	int m_block_width;		//砖块宽度
	int m_block_height;		//砖块高度
	int m_block_dx;			//砖块左侧空白长度
	int m_block_dy;			//砖块下部空白长度
	int m_block_topOffset;	//砖块整体区域相对游戏区域上部偏移
	int m_block_leftOffset;	//砖块整体区域相对游戏区域左侧偏移

	int** m_blocks;
};

