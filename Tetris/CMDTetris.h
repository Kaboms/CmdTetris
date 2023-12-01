#pragma once
//----------------------------------------------------------------------
#include "CMDTetromino.h"
//----------------------------------------------------------------------

namespace CMDTetris
{
	bool TetrominoFall(CMDTetromino& tetromino);
	void SetTetrominoPosition(CMDTetromino&  tetromino);
	void CleanTetrominoPosition(CMDTetromino&  tetromino);
	void AddTetrominoToBoard(CMDTetromino& tetromino);
	void ClearLine();
	bool CanMoveToDirection(CMDTetromino& tetromino, CMDPoint vector);
	void StartGame();
};
//----------------------------------------------------------------------

