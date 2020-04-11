#pragma once
//------------------------------------------------------------------------
#include "CMDPoint.h"
#include <mutex>
//------------------------------------------------------------------------
#define BOARD_SIZE_X 12
#define BOARD_SIZE_Y 14
//------------------------------------------------------------------------

struct BoardCell
{
	enum State
	{
		Empty = 0,
		Live_tetromino = 1,
		Dead_tetromino = 2
	};

	uint8_t Color;
	uint8_t CurrentState;

	BoardCell()
	{
		Color = 0;
		CurrentState = State::Empty;
	}
	BoardCell(uint8_t color, uint8_t state)
	{
		Color = color;
		CurrentState = state;
	}
};
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class CMDBoard
{
public:
	CMDBoard();
	void Draw();
	void Update();
	void UpdateScore(uint64_t score);
	void SetMapCell(const CMDPoint& position, const BoardCell& cell);
	const CMDPoint GetCenter();

public:	
	BoardCell Map[BOARD_SIZE_Y][BOARD_SIZE_X];

private:
	CMDPoint CenterBoard;

	BoardCell PreviosMap[BOARD_SIZE_Y][BOARD_SIZE_X];
	std::mutex Mutex;

};
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------