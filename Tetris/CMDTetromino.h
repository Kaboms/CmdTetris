#pragma once
//------------------------------------------------------------------------
#include "CMDPoint.h"
#include "CMDBoard.h"
//------------------------------------------------------------------------
#define TETROMINO_SIZE 4
//------------------------------------------------------------------------

class CMDTetromino
{
public:
	//Console text color. Color1 = font, Color2 = background
	enum Color
	{
		WhiteBlack = 15,
		WhiteBlue = 31,
		WhiteGreen = 47,
		WhiteCian = 63,
		WhiteRed = 79,
		WhitePink = 95,
		WhiteBrown = 111,
		WhiteYellow = 239,
	};

	CMDTetromino();
	CMDTetromino(CMDPoint map[TETROMINO_SIZE][TETROMINO_SIZE], Color color);
	CMDTetromino(const CMDTetromino& tetromino);

	void Down();
	void Left();
	void Right();
	void SetPosition(CMDPoint position);
	void GetPositions(CMDPoint positions[TETROMINO_SIZE]);
	void Rotate(uint8_t max_x, uint8_t max_y, CMDBoard& board);

public:
	uint8_t State = 0;
	uint8_t CurrentColor = 0;

	CMDPoint map[TETROMINO_SIZE][TETROMINO_SIZE];

private:
	CMDPoint Position;
};
//------------------------------------------------------------------------