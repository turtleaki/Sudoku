#pragma once

#include <iostream>
#include <vector>
#include "Odometer.h"

class SudokuFile
{

public:
	SudokuFile();

	SudokuFile(int N, int p, int q);

	// Sets up the board.
	void setUpBoard(int N);

	// Returns p, the number of rows.
	int getP();

	// Sets p to the new number of rows.
	void setP(int newP);

	// Returns q, the number of columns.
	int getQ();

	// Sets q to the new number of columns.
	void setQ(int newQ);

	// Returns N, the number of cells in a block.
	int getN();

	// Sets N to the new number of cells in a block.
	void setN(int newN);

	// Returns the board.
	std::vector<std::vector<int>> getBoard();

	// Sets board to be the new board
	void setBoard(std::vector<std::vector<int>> newBoard);

	std::string toString();

private:

	int p; //number of rows in a block && number of blocks per row
	int q; //number of columns in a block && number of blocks per column
	int N; //number of cells in a block && edge length of a NxN puzzle

	std::vector<std::vector<int>> board;
	Odometer odometer;

};

