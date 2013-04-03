#ifndef GAMECLASS_H   
#define GAMECLASS_H   

#include <Windows.h>
#include "BallClass.h"
#include "BottomBlock.h"
#include "PictureClass.h"
#include "log.h"

class GameClass
{
public:
	enum GameState
	{
		GameStateFirst = 10000,
		Game_Initialize,
		Game_Run,
		Game_Pause,
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
	void RebuildSences(HDC hdc);

	void ResetGame(HDC hdc);
	void RestartGame(HDC hdc);

	GameState m_state;
	Vector2 m_moveDir;	//底部砖块移动积累量
	Vector2 m_currentMoveDir;
private:
	bool Render();

	#pragma region 碰撞检测
	void CheckHit(Vector2&);
	bool IsInRect(Vector2& point, Vector2& leftTop, Vector2& rightBottom);
	bool IsInRect(Vector2& rectLeftTop, Vector2& rectRightBottom, Vector2& leftTop, Vector2& rightBottom);
	bool IsHitInLevel(Vector2& position, Vector2& speed, Vector2& blockLeftTop);
	#pragma endregion

	#pragma region 画图
	void Draw_Rect(HDC hdc, int horizontalOffset, int verticalOffset, int width = 50, int height = 25, int i = 0);
	void Draw_Rect(HDC hdc, int horizontalOffset, int verticalOffset, int width, int height, HBRUSH* hbrush);

	void Init_Block(HDC hdc);	//初始化砖块
	void Draw_Block(HDC hdc);	//重画现有砖块

	//画球
	void Draw_Ball(HDC hdc);
	void Delete_Ball(HDC hdc);

	//画挡板
	void Draw_BottomBlock(HDC hdc);
	void Delete_BottomBlock(HDC hdc);

	void Draw_InitInfo(HDC hdc, string title);	//初始化界面
	void Draw_PassInfo(HDC hdc);	//过关界面
	void Draw_PauseInfo(HDC hdc);				//暂停界面
	void Draw_DieInfo(HDC hdc);					//画失败界面
	void Draw_GameOverInfo(HDC hdc);			//GameOver界面

	void Draw_Pandle(HDC hdc, int width = 400, int height = 300, int leftOffset = 0, int topOffset = 0, HBRUSH* hbrush = NULL);	//画提示板
	#pragma endregion

	#pragma region 游戏性相关参数

	int m_currentLevel;		//等级情况
	int m_lifeNum;			//剩余生命
	int m_score;			//得分

	#pragma endregion 

	#pragma region 系统相关参数

	float m_ballTime;	//间隔时间
	float m_bottomTime;		//底部挡板用时间间隔
	BallClass* m_ball;		//球
	BottomBlock* m_bottomBlock;
	int** m_blocks;
	HDC m_hdc;

	#pragma region 笔刷

	HBRUSH m_backcolorBursh;	//背景色

	#pragma endregion

	#pragma region 状态参数
	#pragma endregion

	#pragma region 图片对象

	CPicture* m_ballPicture;
	CPicture* m_blockPicture;
	CPicture* m_bottomBlockPicture;

	#pragma endregion


	#pragma region 状态参数
	bool m_isPlayed;	//已经不是初始界面
	bool m_isPaused;	//上一个状态为暂停
	bool m_isPass;	//上个状态为过关
	bool m_isDie;		//上个状态为失败一次
	bool m_isOver;	//上个状态为结束游戏
    int m_blockLast;
    int m_ballSpeed;
	#pragma endregion

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
