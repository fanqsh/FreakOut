#pragma once 
#ifndef _BOTTOMBLOCK_H_
#define _BOTTOMBLOCK_H_   
#include "Vector2.h"

class BottomBlock
{
public:
	BottomBlock();
	BottomBlock(const BottomBlock&);
	~BottomBlock();
	
	bool Initilaize(Vector2& position, Vector2& size);

	void makeMove(Vector2&);	//∫·œÚ“∆∂Ø
	bool CheckHit(Vector2& position, Vector2& speed, int ballSize = 0);

	Vector2 GetPosition(Vector2& position);
	Vector2 GetSize(Vector2& size);
private:
	Vector2 m_position;
	Vector2 m_size;
};

#endif