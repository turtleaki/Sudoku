#ifndef SUDOKUBOARDSOLVER_H
#define SUDOKUBOARDSOLVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <iostream>
#include "Odometer.h"
#include "SudokuFile.h"
#include "SudokuBoardReader.h"
#include <chrono>
#include <ratio>
#include <ctime>
#include <map>
#include <tuple>
#include <list>
#include <algorithm>

class SudokuBoardSolver
{

public:
	SudokuBoardSolver(SudokuFile &sF, long timeout, bool ForwardCheck = false, bool DegreeHeuristic = false, bool LeastConstraintValue = false, bool MinimumRemainingValue = false);
	SudokuBoardSolver();
	bool ForwardCheck;
	bool DegreeHeuristic;
	bool LeastConstraintValue;
	bool MinimumRemainingValue;
	int backtrackingSearch(SudokuFile &sF, long timeout);
	int forwardChecking(SudokuFile &sF, long timeout);



	int returnResultType();
	std::vector<std::vector<int>> returnBoard();
	std::chrono::duration < std::chrono::system_clock::rep, std::chrono::system_clock::period> getStartTime();
	std::string toString(SudokuFile sf);
	int getNumNode();
	int getBacktrack();
	int getN();


private:
	Odometer o;
	int solutionType;
	SudokuFile sf;
	std::string checking = "None";
	std::chrono::duration < std::chrono::system_clock::rep, std::chrono::system_clock::period> startTime;

	//std::chrono::system_clock::time_point startTime;
	int backtrack = 0;
	int countOnce = 0;
	int countNode = 0;
	bool isComplete(SudokuFile &sF);

	bool checkConstraints(int row, int col, int value, SudokuFile &sf, std::vector<std::vector<int>> &board);
	bool checkRow(int row, int value, int N, std::vector<std::vector<int>> &board);
	bool checkColumn(int column, int value, int N, std::vector<std::vector<int>> &board);
	bool checkBox(int row, int column, int value, int p, int q, std::vector<std::vector<int>> &board);

	void makeConstraintMap(SudokuFile &sF);
	int returnNumberOfZeros(SudokuFile &sf);
	void removeValues(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N, int value);
	void removeFromRow(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int N, int value);
	void removeFromCol(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int col, int N, int value);
	void removeFromBox(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N, int value);
	std::map<std::tuple<int, int>, std::vector<int>> constraintBoard;

	std::vector<int> LCV(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N);
	int LCVcheckRow(int row, int col, int value, int N, std::map<std::tuple<int, int>, std::vector<int>> &constraint);
	int LCVcheckColumn(int row, int col, int value, int N, std::map<std::tuple<int, int>, std::vector<int>> &constraint);
	int LCVcheckBox(int row, int col, int value, int p, int q, std::map<std::tuple<int, int>, std::vector<int>> &constraint);

	std::vector<std::tuple<int, int>> makeVariables(int N);
	std::vector<std::tuple<int, int>> MRV(std::map<std::tuple<int, int>, std::vector<int>> &constraint);



	std::vector<std::tuple<int, int>> findHighestDegree(SudokuFile &sF);
	std::vector<std::tuple<int, int>> findHighestDegree(SudokuFile &sF, std::vector<std::tuple<int, int>> var);
	int checkRowForEmptyCells(int row, int col, int N, std::vector<std::vector<int>> &board);
	int checkColumnForEmptyCells(int row, int col, int N, std::vector<std::vector<int>> &board);
	int checkBoxForEmptyCells(int row, int column, int p, int q, std::vector<std::vector<int>> &board);
};

#endif // SUDOKUBOARDSOLVER_H