#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <sstream>
#include "Odometer.h"
#include "SudokuFile.h"

class SudokuBoardReader
{

public:
	SudokuBoardReader();
	SudokuFile readFile(std::string filePath);
	void setSudokuFileParameters(SudokuFile &sf, std::vector<std::string> params);
	void parseLineOfSudokuBoard(SudokuFile &sf, std::vector<std::string> values, int rowNumber);
	std::string toString(SudokuFile sf);

private:

	Odometer o;

};

