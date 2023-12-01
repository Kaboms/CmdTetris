#include "CMDTetris.h"
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <conio.h>
#include <random>
#include "Key.h"
//------------------------------------------------------------------------
using namespace std;
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

CMDBoard* Board = new CMDBoard();

bool IsFall = false;
bool Exit = false;
bool GameOver = false;

uint64_t Score = 0;

CMDTetromino CurrentTetromino;
//------------------------------------------------------------------------

bool CMDTetris::TetrominoFall(CMDTetromino& tetromino)
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

void CMDTetris::SetTetrominoPosition(CMDTetromino&  tetromino)
{
	CMDPoint positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

	for (size_t i = 0; i < TETROMINO_SIZE; ++i)
	{
		Board->SetMapCell(positions[i], BoardCell(tetromino.CurrentColor, BoardCell::State::Live_tetromino));
	}
}
//------------------------------------------------------------------------

void CMDTetris::CleanTetrominoPosition(CMDTetromino&  tetromino)
{
	CMDPoint positions[TETROMINO_SIZE];
	tetromino.GetPositions(positions);

	for (size_t i = 0; i < TETROMINO_SIZE; ++i)
	{
		Board->SetMapCell(positions[i], BoardCell(CMDTetromino::Color::WhiteBlack, BoardCell::State::Empty));
	}
}
//------------------------------------------------------------------------

void CMDTetris::AddTetrominoToBoard(CMDTetromino& tetromino)
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

void CMDTetris::ClearLine()
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

bool CMDTetris::CanMoveToDirection(CMDTetromino& tetromino, CMDPoint vector)
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
}
//------------------------------------------------------------------------

void CMDTetris::StartGame()
{
	Board->Draw();
	Board->UpdateScore(Score);

	static CMDTetromino tetrominos[7] =
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

	auto delta_time = std::chrono::nanoseconds();
	auto draw_delta_time = std::chrono::nanoseconds();
	auto input_delta_time = std::chrono::nanoseconds();

	int default_fall_timeout = 500;
	int fall_timeout = default_fall_timeout;

	Key left_arrow(VK_LEFT);
	Key right_rrow(VK_RIGHT);
	Key up_urrow(VK_UP);
	Key down_arrow(VK_DOWN);

	while (GameOver == false && Exit == false)
	{
		auto call_start_timestamp = std::chrono::system_clock::now();

		if (IsFall == false)
		{
			IsFall = true;

			ClearLine();

			int random_integer = uni(rng);

			CurrentTetromino = tetrominos[random_integer];

			AddTetrominoToBoard(CurrentTetromino);
		}

		if (GameOver == false)
		{
			if (Key::KeyPressed(VK_ESCAPE))
			{
				Exit = true;
			}

			left_arrow.Handle();
			right_rrow.Handle();
			up_urrow.Handle();
			down_arrow.Handle();

			fall_timeout = down_arrow.IsPressed() ? 100 : default_fall_timeout;

			if ((delta_time - input_delta_time) > std::chrono::milliseconds(100))
			{
				input_delta_time = delta_time;

				if (left_arrow.HandlePressedOnce())
				{
					CleanTetrominoPosition(CurrentTetromino);

					if (CanMoveToDirection(CurrentTetromino, CMDPoint(-1, 0)))
					{
						CurrentTetromino.Left();
					}
				}

				if (right_rrow.HandlePressedOnce())
				{
					// Move to left
					CleanTetrominoPosition(CurrentTetromino);

					if (CanMoveToDirection(CurrentTetromino, CMDPoint(1, 0)))
					{
						CurrentTetromino.Right();
					}
				}
				else if (up_urrow.HandlePressedOnce())
				{
					CleanTetrominoPosition(CurrentTetromino);

					CurrentTetromino.Rotate(BOARD_SIZE_X, BOARD_SIZE_Y, *Board);
				}


				SetTetrominoPosition(CurrentTetromino);
				Board->Update();
			}

			if ((delta_time - draw_delta_time) > std::chrono::milliseconds(fall_timeout))
			{
				draw_delta_time = delta_time;

				CleanTetrominoPosition(CurrentTetromino);

				CurrentTetromino.Down();

				IsFall = TetrominoFall(CurrentTetromino);

				if (IsFall == false)
				{
					CMDPoint positions[TETROMINO_SIZE];
					CurrentTetromino.GetPositions(positions);

					for (size_t i = 0; i < TETROMINO_SIZE; ++i)
					{
						Board->SetMapCell(positions[i], BoardCell(CurrentTetromino.CurrentColor, BoardCell::State::Dead_tetromino));
					}
				}

				Board->Update();
			}
		}

		if (GameOver || Exit)
		{
			system("cls");
			cout << "GAME OVER. SCORE: " << Score;
		}

		delta_time += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - call_start_timestamp);

		std::this_thread::sleep_for(std::chrono::seconds(0));
	}
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

