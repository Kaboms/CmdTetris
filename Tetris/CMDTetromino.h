#pragma once
//------------------------------------------------------------------------
#include "CMDPoint.h"
#include <mutex>
//------------------------------------------------------------------------
#define TETROMINO_SIZE 4
//------------------------------------------------------------------------

struct CMDTetromino
{
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

	uint8_t State = 0;
	uint8_t CurrentColor = 0;

	CMDPoint map[TETROMINO_SIZE][TETROMINO_SIZE];

	CMDTetromino()
	{
		Position = CMDPoint();

		for (size_t row = 0; row < TETROMINO_SIZE; ++row)
		{
			for (size_t col = 0; col < TETROMINO_SIZE; ++col)
			{
				map[row][col] = CMDPoint();
			}
		}
	}
	//------------------------------------------------------------------------

	CMDTetromino(CMDPoint map[TETROMINO_SIZE][TETROMINO_SIZE], Color color)
	{
		Position = CMDPoint();
		CurrentColor = color;

		for (size_t row = 0; row < TETROMINO_SIZE; ++row)
		{
			for (size_t col = 0; col < TETROMINO_SIZE; ++col)
			{
				this->map[row][col] = map[row][col];
			}
		}
	}
	//------------------------------------------------------------------------

	CMDTetromino(const CMDTetromino& tetromino)
	{
		Position = CMDPoint();
		CurrentColor = tetromino.CurrentColor;

		for (size_t row = 0; row < TETROMINO_SIZE; ++row)
		{
			for (size_t col = 0; col < TETROMINO_SIZE; ++col)
			{
				map[row][col] = tetromino.map[row][col];
			}
		}
	}
	//------------------------------------------------------------------------

	void Down()
	{
		SetPosition(CMDPoint(Position.x, Position.y + 1));
	}
	//------------------------------------------------------------------------

	void Left()
	{
		SetPosition(CMDPoint(Position.x - 1, Position.y));
	}
	//------------------------------------------------------------------------

	void Right()
	{
		SetPosition(CMDPoint(Position.x + 1, Position.y));
	}
	//------------------------------------------------------------------------

	void SetPosition(CMDPoint position)
	{
		Mutex.lock();

		Position = position;

		Mutex.unlock();
	}
	//------------------------------------------------------------------------

	void GetPositions(CMDPoint positions[TETROMINO_SIZE])
	{
		Mutex.lock();

		for (size_t i = 0; i < TETROMINO_SIZE; i++)
		{
			positions[i] = map[State][i] + Position;
		}

		Mutex.unlock();
	}
	//------------------------------------------------------------------------

	void Rotate(uint8_t max_x, uint8_t max_y)
	{
		uint8_t new_state = (State < 3) ? State + 1 : 0;

		for (size_t i = 0; i < TETROMINO_SIZE; i++)
		{
			if (map[new_state][i].x + Position.x < 0 ||
				map[new_state][i].x + Position.x >= max_x ||
				map[new_state][i].y + Position.y < 0 ||
				map[new_state][i].y + Position.y >= max_y)
			{
				return;
			}
		}

		State = new_state;
	}
	//------------------------------------------------------------------------

private:
	CMDPoint Position;
	std::mutex Mutex;
};
//------------------------------------------------------------------------
CMDPoint L_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(1, 2)},  // *
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(2, 0)},  // *
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(-1, 0)}, // **
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(2, -1)}  //
};
CMDTetromino L_Tetromino(L_TetrominoMap, CMDTetromino::Color::WhiteBlue);
//------------------------------------------------------------------------

CMDPoint J_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(-1, 2)}, //  *
	{CMDPoint(0, 0), CMDPoint(0, -1), CMDPoint(1, 0), CMDPoint(2, 0)}, //  *
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(1, 0)},  // **
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(2, 1)}   //
};
CMDTetromino J_Tetromino(J_TetrominoMap, CMDTetromino::Color::WhiteGreen);
//------------------------------------------------------------------------

CMDPoint I_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(0, 3)}, // *
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(3, 0)}, // *
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(0, 2), CMDPoint(0, 3)}, // *
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(3, 0)}, // *
};
CMDTetromino I_Tetromino(I_TetrominoMap, CMDTetromino::Color::WhiteRed);
//------------------------------------------------------------------------

CMDPoint O_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, 1)}, // 
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, 1)}, // **
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, 1)}, // **
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, 1)}, //
};
CMDTetromino O_Tetromino(O_TetrominoMap, CMDTetromino::Color::WhitePink);
//------------------------------------------------------------------------

CMDPoint T_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(1, -1)},// 
	{CMDPoint(1, 0), CMDPoint(1, 1), CMDPoint(1, 2), CMDPoint(2, 1)}, //  *
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(2, 0), CMDPoint(1, 1)}, // ***
	{CMDPoint(1, 0), CMDPoint(1, 1), CMDPoint(1, 2), CMDPoint(0, 1)}, //
};
CMDTetromino T_Tetromino(T_TetrominoMap, CMDTetromino::Color::WhiteCian);
//------------------------------------------------------------------------

CMDPoint Z_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(0, 1), CMDPoint(-1, 1)}, // 
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 1), CMDPoint(1, 2)},  //  **
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(0, 1), CMDPoint(-1, 1)}, // **
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 1), CMDPoint(1, 2)},  //
};
CMDTetromino Z_Tetromino(Z_TetrominoMap, CMDTetromino::Color::WhiteYellow);
//------------------------------------------------------------------------

CMDPoint S_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(1, 1), CMDPoint(2, 1)}, // 
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, -1)},// **
	{CMDPoint(0, 0), CMDPoint(1, 0), CMDPoint(1, 1), CMDPoint(2, 1)}, //  **
	{CMDPoint(0, 0), CMDPoint(0, 1), CMDPoint(1, 0), CMDPoint(1, -1)},//
};
CMDTetromino S_Tetromino(S_TetrominoMap, CMDTetromino::Color::WhiteBrown);
//------------------------------------------------------------------------