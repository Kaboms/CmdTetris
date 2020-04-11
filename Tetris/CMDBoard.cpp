#include "CMDBoard.h"
#include <iostream>
#include <Windows.h>
//------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//------------------------------------------------------------------------

CMDBoard::CMDBoard()
{
	CenterBoard = CMDPoint(BOARD_SIZE_X / 2, 0);
}
//------------------------------------------------------------------------

void CMDBoard::Draw()
{
	SetConsoleTextAttribute(hConsole, 15);

	system("cls");

	cout << "--------------\n";

	for (size_t row = 0; row < BOARD_SIZE_Y; ++row)
	{
		cout << "|";
		for (size_t col = 0; col < BOARD_SIZE_X; ++col)
		{
			if (Map[row][col].CurrentState == BoardCell::State::Empty)
				cout << '0';
			else
				cout << '*';
		}
		cout << "|" << endl;
	}

	cout << "--------------\n";
}
//------------------------------------------------------------------------

void CMDBoard::Update()
{
	Mutex.lock();

	COORD pos;
	for (size_t row = 0; row < BOARD_SIZE_Y; ++row)
	{
		for (size_t col = 0; col < BOARD_SIZE_X; ++col)
		{
			if (Map[row][col].CurrentState != PreviosMap[row][col].CurrentState)
			{				
				pos.X = col + 1;
				pos.Y = row + 1;
				SetConsoleCursorPosition(hConsole, pos);
				SetConsoleTextAttribute(hConsole, Map[row][col].Color);
				
				if (Map[row][col].CurrentState == BoardCell::State::Empty)
					cout << '0';
				else
					cout << '*';
			}
			PreviosMap[row][col] = Map[row][col];
		}
	}
	Mutex.unlock();
}
//------------------------------------------------------------------------

void CMDBoard::UpdateScore(uint64_t score)
{
	COORD pos;
	pos.X = 0;
	pos.Y = BOARD_SIZE_Y + 3;
	SetConsoleCursorPosition(hConsole, pos);
	SetConsoleTextAttribute(hConsole, 15);

	cout << "SCORE: " << score;
}
//------------------------------------------------------------------------

void CMDBoard::SetMapCell(const CMDPoint& position, const BoardCell& cell)
{
	Map[position.y][position.x] = cell;
}
//------------------------------------------------------------------------

const CMDPoint CMDBoard::GetCenter()
{
	return CenterBoard;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------