#include "CMDTetromino.h"
//------------------------------------------------------------------------

CMDTetromino::CMDTetromino()
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

CMDTetromino::CMDTetromino(CMDPoint map[TETROMINO_SIZE][TETROMINO_SIZE], Color color)
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

CMDTetromino::CMDTetromino(const CMDTetromino& tetromino)
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

void CMDTetromino::Down()
{
	SetPosition(CMDPoint(Position.x, Position.y + 1));
}
//------------------------------------------------------------------------

void CMDTetromino::Left()
{
	SetPosition(CMDPoint(Position.x - 1, Position.y));
}
//------------------------------------------------------------------------

void CMDTetromino::Right()
{
	SetPosition(CMDPoint(Position.x + 1, Position.y));
}
//------------------------------------------------------------------------

void CMDTetromino::SetPosition(CMDPoint position)
{
	Mutex.lock();

	Position = position;

	Mutex.unlock();
}
//------------------------------------------------------------------------

void CMDTetromino::GetPositions(CMDPoint positions[TETROMINO_SIZE])
{
	Mutex.lock();

	for (size_t i = 0; i < TETROMINO_SIZE; i++)
	{
		positions[i] = map[State][i] + Position;
	}

	Mutex.unlock();
}
//------------------------------------------------------------------------

void CMDTetromino::Rotate(uint8_t max_x, uint8_t max_y, CMDBoard& board)
{
	uint8_t new_state = (State < 3) ? State + 1 : 0;

	for (size_t i = 0; i < TETROMINO_SIZE; i++)
	{
		uint8_t x = map[new_state][i].x + Position.x;
		uint8_t y = map[new_state][i].y + Position.y;

		if (x < 0 || x >= max_x || y < 0 || y >= max_y)
		{
			return;
		}
		else if (board.Map[x]->CurrentState != BoardCell::Empty)
		{
			return;
		}
	}

	State = new_state;
}
//------------------------------------------------------------------------