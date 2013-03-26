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

	void Draw_BottomBlock(HDC hdc);
	void Delete_BottomBlock(HDC hdc);

	GameState m_state;
	Vector2 m_moveDir;	//�ײ�ש���ƶ�������
private:
	bool Render();
	void CheckHit(Vector2&);
	bool IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom);
	bool IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom);
	void Init_Block(HDC hdc);

	bool IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop);

	float m_spacingTime;
	BallClass* m_ball;
	BottomBlock* m_bottomBlock;	
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


#endif
