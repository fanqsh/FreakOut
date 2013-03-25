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

	float m_spacingTime;
	BallClass* m_ball;
	HDC m_hdc;

	int m_screenHeight;
	int m_screenWidth;

	int m_game_rect_leftOffset;	//��Ϸ������Դ������ƫ��
	int m_game_rect_topOffset;	//��Ϸ������Դ����ϲ�ƫ��

	int m_game_rect_height;	//��Ϸ����߶�
	int m_game_rect_width;	//��Ϸ������

	int m_block_rows;		//ש������
	int m_block_columns;	//ש������

	int m_block_width;		//ש����
	int m_block_height;		//ש��߶�
	int m_block_dx;			//ש�����հ׳���
	int m_block_dy;			//ש���²��հ׳���
	int m_block_topOffset;	//ש���������������Ϸ�����ϲ�ƫ��
	int m_block_leftOffset;	//ש���������������Ϸ�������ƫ��

	int** m_blocks;
};

