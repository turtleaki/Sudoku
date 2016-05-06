#pragma once
#include "SudokuFile.h"
#include <vector>
class SudokuBoardGenerator{
public:
	SudokuBoardGenerator(int N, int p, int q, int numAssigments);
	~SudokuBoardGenerator(void);

	std::vector<std::vector<int>> returnBoard();

private:
	int N, p, q, numAssignments;
	std::vector<std::vector<int>> retBoard;

	SudokuFile& generateBoard(int N, int p, int q, int numAssignments, long timeout);
	static bool checkConstraints(int row, int col, int value, SudokuFile &sf, std::vector<std::vector<int>> &board);
	static bool checkRow(int row, int value, int N, std::vector<std::vector<int>> &board);
	static bool checkColumn(int column, int value, int N, std::vector<std::vector<int>> &board);
	static bool checkBox(int row, int column, int value, int p, int q, std::vector<std::vector<int>> &board);

};