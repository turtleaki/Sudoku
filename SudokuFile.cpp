
#include "SudokuFile.h"
#include <iostream>
#include <vector>
#include <string>


SudokuFile::SudokuFile()
{

}

SudokuFile::SudokuFile(int N, int p, int q)
:N(N), p(p), q(q)
{
	if (N != p * q || N < 1)
	{
		std::cout << "Board parameters invalid. Creating a 9x9 sudoku file instead." << std::endl;
		setP(3);
		setQ(3);
		setN(9);
		setUpBoard(9);
	}
	else
	{
		setP(p);
		setQ(q);
		setN(N);

		setUpBoard(N);
	}
}

// Sets up the board.
void SudokuFile::setUpBoard(int N)
{
	board.resize(N);
	for (int row = 0; row < N; ++row){
		board[row].resize(N);
		for (int col = 0; col < N; ++col){
			board[row][col] = 0;
			//std::cout << board[row][col] << " ";
		}
		//std::cout << std::endl;
	}
}

// Returns p, the number of rows.
int SudokuFile::getP()
{
	return p;
}

// Sets p to the new number of rows.
void SudokuFile::setP(int newP)
{
	p = newP;
}

// Returns q, the number of columns.
int SudokuFile::getQ()
{
	return q;
}

// Sets q to the new number of columns.
void SudokuFile::setQ(int newQ)
{
	q = newQ;
}

// Returns N, the number of cells in a block.
int SudokuFile::getN()
{
	return N;
}

// Sets N to the new number of cells in a block.
void SudokuFile::setN(int newN)
{
	N = newN;
}

// Returns the board.
std::vector<std::vector<int>> SudokuFile::getBoard()
{
	return board;
}

// Sets board to be the new board
void SudokuFile::setBoard(std::vector<std::vector<int>> newBoard)
{
	board = newBoard;
}

std::string SudokuFile::toString(){
	std::string sb = "";
	sb += "N: ";
	sb.append(std::to_string(N));
	sb.append("\tP: ");
	sb.append(std::to_string(p));
	sb.append("\tQ: ");
	sb.append(std::to_string(q));
	sb.append("\n");

	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			sb.append(odometer.intToOdometer(board[i][j]) + " ");
			if ((j + 1) % q == 0 && j != 0 && j != N - 1){
				sb.append("| ");
			}
		}
		sb.append("\n");

		if ((i + 1) % p == 0 && i != 0 && i != N - 1){
			for (int k = 0; k < N + p - 1; ++k){
				sb.append("- ");
			}
			sb.append("\n");
		}
	}

	return sb;
}