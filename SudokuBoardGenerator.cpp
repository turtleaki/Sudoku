#include "SudokuBoardGenerator.h"
#include <iostream>
#include <string>
#include <random>
#include <chrono>


SudokuBoardGenerator::SudokuBoardGenerator(int N, int p, int q, int numAssigments)
:N(N), p(p), q(q), numAssignments(numAssigments)
{
	generateBoard(N, p, q, numAssignments, 5000);

}

SudokuBoardGenerator::~SudokuBoardGenerator(){

}

SudokuFile& SudokuBoardGenerator::generateBoard(int N, int p, int q, int numAssignments, long timeout){
	//given a SudokuFile with N, P, Q, creates a board with the given params
	//and assigns it to the board of the SudokuFile.
	//timeout represents the time in ms allowed to created the SudokuFile
	SudokuFile sf = SudokuFile(N, p, q);
	std::vector<std::vector<int>> tempBoard = sf.getBoard();
	if (numAssignments > sf.getN()*sf.getN())
	{
		std::cout << "Number of assignments exceeds available spaces in board. Returning SudokuFile with an empty board" << std::endl;
		return sf;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis1(0, sf.getN() - 1);
	std::uniform_int_distribution<> dis2(1, sf.getN());

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numAssignments; i++)
	{
		int randomRow = dis1(gen); //0 to N-1
		int randomColumn = dis1(gen); //0 to N-1
		int randomAssignment = dis2(gen); //1 to N
		if (tempBoard[randomRow][randomColumn] == 0 && checkConstraints(randomRow, randomColumn, randomAssignment, sf, tempBoard))
		{
			tempBoard[randomRow][randomColumn] = randomAssignment;
		}
		else
		{
			i--;

			auto currentTime = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() > timeout)
			{
				std::cout << "Timeout at " << i << " elements" << std::endl;
				std::vector<std::vector<int>> tempboard;
				break;
			}
		}
	}
	sf.setBoard(tempBoard);
	retBoard = tempBoard;
	return sf;
}


bool SudokuBoardGenerator::checkConstraints(int row, int col, int value, SudokuFile &sf, std::vector<std::vector<int>> &board){
	if (checkRow(row, value, sf.getN(), board)
		&& checkColumn(col, value, sf.getN(), board)
		&& checkBox(row, col, value, sf.getP(), sf.getQ(), board))
	{
		return true;
	}
	return false;
}

bool SudokuBoardGenerator::checkRow(int row, int value, int N, std::vector<std::vector<int>> &board){
	for (int i = 0; i < N; ++i){
		if (board[row][i] == value){
			return false;
		}
	}
	return true;
}

bool SudokuBoardGenerator::checkColumn(int column, int value, int N, std::vector<std::vector<int>> &board){
	for (int i = 0; i < N; ++i){
		if (board[i][column] == value){
			return false;
		}
	}
	return true;
}

bool SudokuBoardGenerator::checkBox(int row, int column, int value, int p, int q, std::vector<std::vector<int>> &board){
	int rDiv = row / p;
	int cDiv = column / q;
	for (int i = rDiv * p; i < (rDiv + 1) * p; ++i){
		for (int j = cDiv*q; j < (cDiv + 1)*q; ++j){
			try{
				if (board[i][j] == value){
					return false;
				}
			}
			catch (std::exception &e){
				std::cout << p << " " << q << std::endl;
			}
		}
	}
	return true;
}

std::vector<std::vector<int>> SudokuBoardGenerator::returnBoard(){
	return retBoard;
};