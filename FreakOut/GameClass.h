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
		m_game_rect_leftOffset = 20,	//��Ϸ������Դ������ƫ��
		m_game_rect_topOffset = 20,		//��Ϸ������Դ����ϲ�ƫ��

		m_game_rect_height = 600,		//��Ϸ����߶�
		m_game_rect_width = 600,		//��Ϸ������

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
	Vector2 m_moveDir;	//�ײ�ש���ƶ�������
private:
	bool Render();

	#pragma region ��ײ���
	void CheckHit(Vector2&);
	bool IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom);
	bool IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom);
	bool IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop);
	#pragma endregion

	#pragma region ��ͼ

	void Init_Block(HDC hdc);

	void Draw_Ball(HDC hdc);
	void Delete_Ball(HDC hdc);

	void Draw_BottomBlock(HDC hdc);
	void Delete_BottomBlock(HDC hdc);

	void Draw_InitInfo(HDC hdc, string title);
	void Draw_PassInfo(HDC hdc, string title);

	#pragma endregion

	#pragma region ϵͳ��ز���
	float m_spacingTime;	//���ʱ��
	BallClass* m_ball;		//��
	BottomBlock* m_bottomBlock;		
	int** m_blocks;
	HDC m_hdc;

	#pragma endregion

	#pragma region λ�ò���
	int m_screenHeight;
	int m_screenWidth;

	//int m_game_rect_leftOffset;	//��Ϸ������Դ������ƫ��
	//int m_game_rect_topOffset;	//��Ϸ������Դ����ϲ�ƫ��

	//int m_game_rect_height;	//��Ϸ����߶�
	//int m_game_rect_width;	//��Ϸ������

	//int m_block_rows;		//ש������
	//int m_block_columns;	//ש������

	//int m_block_width;		//ש����
	//int m_block_height;		//ש��߶�
	//int m_block_dx;			//ש�����հ׳���
	//int m_block_dy;			//ש���²��հ׳���
	//int m_block_topOffset;	//ש���������������Ϸ�����ϲ�ƫ��
	//int m_block_leftOffset;	//ש���������������Ϸ�������ƫ��
	#pragma endregion

};


#endif
