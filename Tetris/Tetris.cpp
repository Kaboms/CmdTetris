#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <mutex>
#include <atomic>
#include "Tetromino.h"
#include <random>
//------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------
#define BOARD_SIZE_X 12
#define BOARD_SIZE_Y 14
//------------------------------------------------------------------------

enum BoardState
{
	Empty = 0,
	Live_tetromino = 1,
	Dead_tetromino = 2
};
const Point CenterBoard(BOARD_SIZE_X / 2, 0);

int board[BOARD_SIZE_Y][BOARD_SIZE_X]
{
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

//------------------------------------------------------------------------
std::atomic<bool> IsFall = false;

Tetromino* G_Tetromino;

std::mutex Mutex;

bool Exit = false;
bool GameOver = false;

uint64_t Score = 0;
//------------------------------------------------------------------------

const int KEY_ESC = 27;
const int KEY_UP_ARROW = 72;
const int KEY_DOWN_ARROW = 80;
const int KEY_LEFT_ARROW = 75;
const int KEY_RIGHT_ARROW = 77;

int key = -1;
//------------------------------------------------------------------------

void DrawBoard()
{
	Mutex.lock();

    system("cls");

    cout << "--------------\n";

    for (size_t row = 0; row < BOARD_SIZE_Y; ++row)
    {
        cout << "|";
        for (size_t col = 0; col < BOARD_SIZE_X; ++col)
        {
            if (board[row][col] == BoardState::Empty)
                cout << '0';
            else
                cout << '*';
        }
        cout << "|" << endl;
    }

    cout << "--------------\n";
	cout << "Score: " << Score;

	Mutex.unlock();
}
//------------------------------------------------------------------------

bool TetrominoFall(Tetromino& tetromino)
{
    Point positions[TETROMINO_SIZE];
    tetromino.GetPositions(positions);

    bool is_fall = true;
    for (size_t i = 0; i < TETROMINO_SIZE; ++i)
    {
        if (positions[i].y == BOARD_SIZE_Y - 1 || board[positions[i].y + 1][positions[i].x] == BoardState::Dead_tetromino)
        {
            is_fall = false;
        }

        board[positions[i].y][positions[i].x] = BoardState::Live_tetromino;
    }

    return is_fall;
}
//------------------------------------------------------------------------

void SetTetrominoPosition(Tetromino&  tetromino)
{
    Point positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

    for (size_t i = 0; i < TETROMINO_SIZE; ++i)
    {
        board[positions[i].y][positions[i].x] = BoardState::Live_tetromino;
    }
}
//------------------------------------------------------------------------

void CleanTetrominoPosition(Tetromino&  tetromino)
{
    Point positions[TETROMINO_SIZE];
    tetromino.GetPositions(positions);

    for (size_t i = 0; i < TETROMINO_SIZE; ++i)
    {
		board[positions[i].y][positions[i].x] = BoardState::Empty;
    }
}
//------------------------------------------------------------------------

void AddTetrominoToBoard(Tetromino& tetromino)
{
	if (board[CenterBoard.y + 2][CenterBoard.x] == BoardState::Empty)
	{
		tetromino.SetPosition(Point(CenterBoard.x, CenterBoard.y));
		SetTetrominoPosition(tetromino);
	}
	else
	{
		GameOver = true;
	}
}
//------------------------------------------------------------------------

void ClearLine()
{
	bool is_line;

	for (size_t g_row = 0; g_row < BOARD_SIZE_Y; ++g_row)
	{
		is_line = true;

		for (size_t col = 0; col < BOARD_SIZE_X; ++col)
		{
			if (board[g_row][col] == BoardState::Empty)
			{
				is_line = false;
			}
		}

		if (is_line)
		{
			int board_copy[BOARD_SIZE_Y][BOARD_SIZE_X];
			for (size_t row = 0; row < BOARD_SIZE_Y; ++row)
			{
				for (size_t col = 0; col < BOARD_SIZE_X; ++col)
				{
					board_copy[row][col] = board[row][col];
				}
			}

			for (size_t row = 0; row < g_row; ++row)
			{
				for (size_t col = 0; col < BOARD_SIZE_X; ++col)
				{
					board[row + 1][col] = board_copy[row][col];
				}
			}

			Score += 150;
		}

	}
}
//------------------------------------------------------------------------

void Fall()
{
	Tetromino tetrominos[4] =
	{
		I_Tetromino,
		L_Tetromino,
		J_Tetromino,
		O_Tetromino
	};

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, 3); // guaranteed unbiased

	while (Exit == false && GameOver == false)
	{
		int random_integer = uni(rng);

		if (IsFall == false)
		{
			IsFall = true;

			ClearLine();

			int random_integer = uni(rng);

			G_Tetromino = &Tetromino(tetrominos[random_integer]);

			AddTetrominoToBoard(*G_Tetromino);
		}

		if (GameOver || Exit)
			return;

		while (IsFall)
		{
			if (key != KEY_DOWN_ARROW)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			else
			{
				key = -1;
			}

			CleanTetrominoPosition(*G_Tetromino);

			G_Tetromino->Down();

			IsFall = TetrominoFall(*G_Tetromino);

			if (IsFall == false)
			{
				Point positions[TETROMINO_SIZE];
				G_Tetromino->GetPositions(positions);

				for (size_t i = 0; i < TETROMINO_SIZE; ++i)
				{
					board[positions[i].y][positions[i].x] = BoardState::Dead_tetromino;
				}
			}

			DrawBoard();
		}
	}
}
//------------------------------------------------------------------------

int main()
{
    auto correct_pos = [&](Tetromino& tetromino)
    {
        Point positions[TETROMINO_SIZE];
        tetromino.GetPositions(positions);

        for (size_t i = 0; i < TETROMINO_SIZE; ++i)
        {
            if (positions[i].x < 0 ||
				positions[i].x >= BOARD_SIZE_X ||
				board[positions[i].y][positions[i].x] == 1 ||
				board[positions[i].y][positions[i].x] == 2)
            {
                return false;
            }
        }

        return true;
    };

	thread fall_thread(Fall);
	fall_thread.detach();

    while (key != KEY_ESC && GameOver == false)
    {
        key = _getch();

        if (IsFall && GameOver == false && G_Tetromino)
        {
            if (key == KEY_LEFT_ARROW)
            {
                CleanTetrominoPosition(*G_Tetromino);

				G_Tetromino->Left();

                if (correct_pos(*G_Tetromino))
                {
                    SetTetrominoPosition(*G_Tetromino);
                    DrawBoard();
                }
                else
                {
					G_Tetromino->Right();
                }
            }
            else if (key == KEY_RIGHT_ARROW)
            {
                CleanTetrominoPosition(*G_Tetromino);

				G_Tetromino->Right();

                if (correct_pos(*G_Tetromino))
                {
                    SetTetrominoPosition(*G_Tetromino);
                    DrawBoard();
                }
                else
                {
					G_Tetromino->Left();
                }
            }
            else if (key == KEY_UP_ARROW)
            {
                CleanTetrominoPosition(*G_Tetromino);

				G_Tetromino->Rotate(BOARD_SIZE_X, BOARD_SIZE_Y);

                SetTetrominoPosition(*G_Tetromino);
                DrawBoard();
            }
			else if (key == KEY_ESC)
			{
				Exit = true;
			}
        }
		else if (GameOver)
		{
			system("clr");
			cout << "GAME OVER. SCORE: " << Score;
		}
    }

    //DrawBoard(board);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------