#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <mutex>
#include <atomic>
#include <random>
#include "CMDBoard.h"
#include "CMDTetromino.h"
#include <Windows.h>
//------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------
CMDBoard* Board = new CMDBoard();

std::atomic<bool> IsFall = false;

CMDTetromino* G_Tetromino;

bool Exit = false;
bool GameOver = false;

uint64_t Score = 0;
//------------------------------------------------------------------------

const uint8_t KEY_ESC = 27;
const uint8_t KEY_UP_ARROW = 72;
const uint8_t KEY_DOWN_ARROW = 80;
const uint8_t KEY_LEFT_ARROW = 75;
const uint8_t KEY_RIGHT_ARROW = 77;

int key = -1;
//------------------------------------------------------------------------

bool TetrominoFall(CMDTetromino& tetromino)
{
	CMDPoint positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

	bool is_fall = true;
	for (size_t i = 0; i < TETROMINO_SIZE; ++i)
	{
		if (positions[i].y == BOARD_SIZE_Y - 1 || Board->Map[positions[i].y + 1][positions[i].x].CurrentState == BoardCell::State::Dead_tetromino)
		{
			is_fall = false;
		}

		Board->SetMapCell(positions[i], BoardCell(tetromino.CurrentColor, BoardCell::State::Live_tetromino));
	}

	return is_fall;
}
//------------------------------------------------------------------------

void SetTetrominoPosition(CMDTetromino&  tetromino)
{
	CMDPoint positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

	for (size_t i = 0; i < TETROMINO_SIZE; ++i)
	{
		Board->SetMapCell(positions[i], BoardCell(tetromino.CurrentColor, BoardCell::State::Live_tetromino));
	}
}
//------------------------------------------------------------------------

void CleanTetrominoPosition(CMDTetromino&  tetromino)
{
	CMDPoint positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

	for (size_t i = 0; i < TETROMINO_SIZE; ++i)
	{
		Board->SetMapCell(positions[i], BoardCell(CMDTetromino::Color::WhiteBlack, BoardCell::State::Empty));
	}
}
//------------------------------------------------------------------------

void AddTetrominoToBoard(CMDTetromino& tetromino)
{
	if (Board->Map[Board->GetCenter().y + 2][Board->GetCenter().x].CurrentState == BoardCell::State::Empty)
	{
		tetromino.SetPosition(CMDPoint(Board->GetCenter().x, Board->GetCenter().y));
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
			if (Board->Map[g_row][col].CurrentState == BoardCell::State::Empty)
			{
				is_line = false;
			}
		}

		if (is_line)
		{
			BoardCell board_copy[BOARD_SIZE_Y][BOARD_SIZE_X];
			for (size_t row = 0; row < BOARD_SIZE_Y; ++row)
			{
				for (size_t col = 0; col < BOARD_SIZE_X; ++col)
				{
					board_copy[row][col] = Board->Map[row][col];
				}
			}

			for (size_t row = 0; row < g_row; ++row)
			{
				for (size_t col = 0; col < BOARD_SIZE_X; ++col)
				{
					Board->Map[row + 1][col] = board_copy[row][col];
				}
			}

			Score += 150;
			Board->UpdateScore(Score);
		}

	}
}
//------------------------------------------------------------------------

void Fall()
{
	CMDTetromino tetrominos[7] =
	{
		I_Tetromino,
		L_Tetromino,
		J_Tetromino,
		O_Tetromino, 
		T_Tetromino,
		Z_Tetromino,
		S_Tetromino
	};

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 6);

	while (Exit == false && GameOver == false)
	{
		int random_integer = uni(rng);

		if (IsFall == false)
		{
			IsFall = true;

			ClearLine();

			int random_integer = uni(rng);

			G_Tetromino = &CMDTetromino(tetrominos[random_integer]);

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
				CMDPoint positions[TETROMINO_SIZE];
				G_Tetromino->GetPositions(positions);

				for (size_t i = 0; i < TETROMINO_SIZE; ++i)
				{
					Board->SetMapCell(positions[i], BoardCell(G_Tetromino->CurrentColor, BoardCell::State::Dead_tetromino));
				}
			}

			Board->Update();
		}
	}
}
//------------------------------------------------------------------------

void StartGame()
{
	auto correct_pos = [&](CMDTetromino& tetromino, CMDPoint vector)
	{
		CMDPoint positions[TETROMINO_SIZE];
		tetromino.GetPositions(positions);

		for (size_t i = 0; i < TETROMINO_SIZE; ++i)
		{
			CMDPoint pos(positions[i] + vector);

			if (pos.x < 0 ||
				pos.x >= BOARD_SIZE_X ||
				Board->Map[pos.y][pos.x].CurrentState == BoardCell::State::Live_tetromino ||
				Board->Map[pos.y][pos.x].CurrentState == BoardCell::State::Dead_tetromino)
			{
				return false;
			}
		}

		return true;
	};

	Board->Draw();
	Board->UpdateScore(Score);

	thread fall_thread(Fall);
	fall_thread.detach();

	while (key != KEY_ESC && GameOver == false)
	{
		key = _getch();

		if (IsFall && GameOver == false && G_Tetromino)
		{
			if (key != KEY_ESC)
			{
				CleanTetrominoPosition(*G_Tetromino);

				if (key == KEY_LEFT_ARROW)
				{
					if (correct_pos(*G_Tetromino, CMDPoint(-1, 0)))
					{
						G_Tetromino->Left();
					}				
				}
				else if (key == KEY_RIGHT_ARROW)
				{
					if (correct_pos(*G_Tetromino, CMDPoint(1, 0)))
					{
						G_Tetromino->Right();
					}
				}
				else if (key == KEY_UP_ARROW)
				{
					G_Tetromino->Rotate(BOARD_SIZE_X, BOARD_SIZE_Y, *Board);
				}

				SetTetrominoPosition(*G_Tetromino);
				Board->Update();
			}
			else if (key == KEY_ESC)
			{
				Exit = true;
			}

		}
		else if (GameOver)
		{
			system("cls");
			cout << "GAME OVER. SCORE: " << Score;
			_getch();
		}
	}
}
//------------------------------------------------------------------------

int main()
{
	StartGame();

	_getch();
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------