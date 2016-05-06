#ifndef SUDOKUBOARDREADER_CPP
#define SUDOKUBOARDREADER_CPP

/*
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <sstream>
#include "Odometer.h"
#include "SudokuFile.cpp"
*/

#include "SudokuBoardReader.h"

class InvalidParameters: public std::exception
{
	virtual const char* what() const throw()
	{
		return "Parameters are invalid in the file";
	}
};

SudokuBoardReader::SudokuBoardReader()
{
	o = Odometer();
}

SudokuFile SudokuBoardReader::readFile(std::string filePath)
{
	SudokuFile sF = SudokuFile();

	std::string line;
	std::ifstream file(filePath);

	int lineCounter = 0;
	std::vector<std::string> parameters;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			//std::cout << line << std::endl;
			std::vector<std::string> lineParts;
			std::string tempLine = line;

			std::istringstream iss(tempLine);

			do
			{
				std::string sub;
				iss >> sub;
				if (sub != "") lineParts.push_back(sub);
			} while (iss);

			//while (tempLine.find(" ") == 4294967295)
			/*
			while (lineCounter == 0 && lineParts.size()  != 3)
			{
				lineParts.push_back(tempLine.substr(0, tempLine.find(" ")));
				tempLine = tempLine.substr(tempLine.find(" ") + 1);
			}
			*/

			if (lineCounter == 0)
			{
				setSudokuFileParameters(sF, lineParts);
			}
			else
			{
				parseLineOfSudokuBoard(sF, lineParts, lineCounter-1);
			}

			lineCounter++;
		}

		if (lineCounter == 0)
		{
			std::cout << "Input file " << filePath << " was empty." << std::endl;
		}
		else if (lineCounter < sF.getN())
		{
			std::cout << "Incomplete or Empty board for file " << filePath << ". Please be advised." << std::endl;
		}


		file.close();
	}

	else
	{
		std::cout << filePath << std::endl;
		std::cout << "Unable to open file." << std::endl;
	}

	return sF;
}

void SudokuBoardReader::setSudokuFileParameters(SudokuFile &sf, std::vector<std::string> params)
{
	if (params.size() != 3)
	{
		throw InvalidParameters();
	}

	sf.setN(atoi(params[0].c_str()));
	sf.setP(atoi(params[1].c_str()));
	sf.setQ(atoi(params[2].c_str()));
}


void SudokuBoardReader::parseLineOfSudokuBoard(SudokuFile &sf, std::vector<std::string> values, int rowNumber)
{
	if (sf.getBoard().size() == 0)
	{
		std::vector<int> columns (sf.getN(),0);
		std::vector<std::vector<int>> rows (sf.getN(), columns);
		sf.setBoard(rows);
	}

	std::vector<std::vector<int>> board = sf.getBoard();

	for (int i = 0; i < values.size(); i++)
	{
		board[rowNumber][i] = o.odometerToInt(values[i]);
	}

	sf.setBoard(board);
}

std::string SudokuBoardReader::toString(SudokuFile sf)
{
	std::string sb = "";
	sb += "N: ";
	sb.append(std::to_string(sf.getN()));
	sb.append("\tP: ");
	sb.append(std::to_string(sf.getP()));
	sb.append("\tQ: ");
	sb.append(std::to_string(sf.getQ()));
	sb.append("\n");

	for (int i = 0; i < sf.getN(); ++i){
		for (int j = 0; j < sf.getN(); ++j){
			sb.append(o.intToOdometer(sf.getBoard()[i][j]) + " ");
			if ((j + 1) % sf.getQ() == 0 && j != 0 && j != sf.getN() - 1){
				sb.append("| ");
			}
		}
		sb.append("\n");

		if ((i + 1) % sf.getP() == 0 && i != 0 && i != sf.getN() - 1){
			for (int k = 0; k < sf.getN() + sf.getP() - 1; ++k){
				sb.append("- ");
			}
			sb.append("\n");
		}
	}

	return sb;
}


#endif SUDOKUBOARDREADER_CPP