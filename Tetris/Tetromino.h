#pragma once
//------------------------------------------------------------------------
#include <atomic>
//------------------------------------------------------------------------

struct Point
{
    int8_t x, y;

    Point()
    {
        x = 0;
        y = 0;
    }
    //------------------------------------------------------------------------

    Point(int8_t x, int8_t y)
    {
        this->x = x;
        this->y = y;
    }
    //------------------------------------------------------------------------
    
    Point operator+(Point& p)
    {
        Point point(this->x, this->y);

        point.x += p.x;
        point.y += p.y;

        return point;
    }
};
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define TETROMINO_SIZE 4
//------------------------------------------------------------------------

struct Tetromino
{
    uint8_t State = 0;

    Point map[TETROMINO_SIZE][TETROMINO_SIZE];

	std::mutex Mutex;

    Tetromino()
    {
        Position = Point();

        for (size_t row = 0; row < TETROMINO_SIZE; ++row)
        {
            for (size_t col = 0; col < TETROMINO_SIZE; ++col)
            {
                map[row][col] = Point();
            }
        }
    }
    //------------------------------------------------------------------------

    Tetromino(Point map[TETROMINO_SIZE][TETROMINO_SIZE])
    {
        Position = Point();

        for (size_t row = 0; row < TETROMINO_SIZE; ++row)
        {
            for (size_t col = 0; col < TETROMINO_SIZE; ++col)
            {
                this->map[row][col] = map[row][col];
            }
        }
    }
    //------------------------------------------------------------------------

	Tetromino(const Tetromino& tetromino)
	{
		Position = Point();

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
		SetPosition(Point(Position.x, Position.y + 1));
    }
    //------------------------------------------------------------------------

    void Left()
    {
		SetPosition(Point(Position.x - 1, Position.y));
    }
    //------------------------------------------------------------------------

    void Right()
    {
		SetPosition(Point(Position.x + 1, Position.y));
    }
    //------------------------------------------------------------------------

    void SetPosition(Point position)
    {
		Mutex.lock();

        Position = position;

		Mutex.unlock();
    }
    //------------------------------------------------------------------------

    void GetPositions(Point positions[TETROMINO_SIZE])
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
    Point Position;

};
//------------------------------------------------------------------------
Point L_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
    {Point(0, 0), Point(0, 1), Point(0, 2), Point(1, 2)},  // *
    {Point(0, 0), Point(0, 1), Point(1, 0), Point(2, 0)},  // *
    {Point(0, 0), Point(0, 1), Point(0, 2), Point(-1, 0)}, // **
    {Point(0, 0), Point(1, 0), Point(2, 0), Point(2, -1)}  //
};
Tetromino L_Tetromino(L_TetrominoMap);
//------------------------------------------------------------------------

Point J_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{Point(0, 0), Point(0, 1), Point(0, 2), Point(-1, 2)}, //  *
	{Point(0, 0), Point(0, -1), Point(1, 0), Point(2, 0)}, //  *
	{Point(0, 0), Point(0, 1), Point(0, 2), Point(1, 0)},  // **
	{Point(0, 0), Point(1, 0), Point(2, 0), Point(2, 1)}   //
};
Tetromino J_Tetromino(J_TetrominoMap);
//------------------------------------------------------------------------

Point I_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
    {Point(0, 0), Point(0, 1), Point(0, 2), Point(0, 3)}, // *
    {Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0)}, // *
    {Point(0, 0), Point(0, 1), Point(0, 2), Point(0, 3)}, // *
    {Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0)}, // *
};
Tetromino I_Tetromino(I_TetrominoMap);
//------------------------------------------------------------------------

Point O_TetrominoMap[TETROMINO_SIZE][TETROMINO_SIZE]
{
	{Point(0, 0), Point(0, 1), Point(1, 0), Point(1, 1)}, // 
	{Point(0, 0), Point(0, 1), Point(1, 0), Point(1, 1)}, // **
	{Point(0, 0), Point(0, 1), Point(1, 0), Point(1, 1)}, // **
	{Point(0, 0), Point(0, 1), Point(1, 0), Point(1, 1)}, //
};
Tetromino O_Tetromino(O_TetrominoMap);
//------------------------------------------------------------------------