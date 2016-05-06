#include "SudokuBoardSolver.h"

SudokuBoardSolver::SudokuBoardSolver(){};

SudokuBoardSolver::SudokuBoardSolver(SudokuFile &sF, long timeout, bool ForwardCheck, bool DegreeHeuristic, bool LeastConstraintValue, bool MinimumRemainingValue)
:ForwardCheck(ForwardCheck), DegreeHeuristic(DegreeHeuristic), LeastConstraintValue(LeastConstraintValue), MinimumRemainingValue(MinimumRemainingValue)
{
	SudokuFile file(sF.getN(), sF.getP(), sF.getQ());
	if (ForwardCheck || DegreeHeuristic || LeastConstraintValue || MinimumRemainingValue){
		makeConstraintMap(sF);
		solutionType = forwardChecking(sF, timeout);
	}
	else{
		solutionType = backtrackingSearch(sF, timeout);
	}
	if (solutionType == 2 || solutionType == 3){
		sf.setN(sF.getN());
		sf.setBoard(file.getBoard());
	
	}
}

int SudokuBoardSolver::backtrackingSearch(SudokuFile &sF, long timeout)
{
	if (countOnce == 0)
	{
		startTime = std::chrono::system_clock::now().time_since_epoch();

	}

	countOnce++;

	bool timedOut = false;

	if (isComplete(sF))
	{
		sf = sF;
		return 1;
	}
	else
	{
		std::vector<std::vector<int>> board = sF.getBoard();

		for (int i = 0; i < sF.getN(); i++)
		{

			for (int j = 0; j < sF.getN(); j++)
			{

				for (int k = 1; k < sF.getN() + 1; k++)
				{

					if (board[i][j] == 0)
					{
		
						if (checkConstraints(i, j, k, sF, board))
						{
		
							board[i][j] = k;
							++countNode;
							sF.setBoard(board);
				

							if (backtrackingSearch(sF, timeout) == 1)
							{
								++countNode;
								return 1;
							}
							else
							{
								auto currentTime = std::chrono::system_clock::now().time_since_epoch();
								if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= timeout)
								{
									++countNode;
									return 3;
								}

								else
								{
									board[i][j] = 0;
									sF.setBoard(board);
		
									if (k == sF.getN())
									{
										++backtrack;
										return 2;
									}
								}
							}
						}
						else if (k == sF.getN())
						{
							++backtrack;
							return 2;
						}

						auto currentTime = std::chrono::system_clock::now().time_since_epoch();
						if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= timeout)
						{
							++countNode;
							return 3;
						}
					}
				}
			}
		}
	}

	return 2;

}


int SudokuBoardSolver::forwardChecking(SudokuFile &sF, long timeout)
{
	if (countOnce == 0)
	{
		startTime = std::chrono::system_clock::now().time_since_epoch();

	}

	countOnce++;

	bool timedOut = false;

	if (isComplete(sF)){
		sf = sF;

		return 1;
	}


	std::vector<std::vector<int>> board = sF.getBoard();

	std::map<std::tuple<int, int>, std::vector<int>> constraint = constraintBoard;
	std::vector<std::tuple<int, int>> variables;
	if (MinimumRemainingValue && DegreeHeuristic){
		auto temp = MRV(constraint);
		if (temp.size() > 1){
			variables = findHighestDegree(sF, temp);

		}
	}

	if (MinimumRemainingValue & !DegreeHeuristic){
		variables = MRV(constraint);
	}

	if (DegreeHeuristic & !MinimumRemainingValue){
		variables = findHighestDegree(sF);
	}
	else{
		variables = makeVariables(sF.getN());
	}
	for (int i = 0; i < variables.size(); i++){
		int row = std::get<0>(variables[i]);
		int col = std::get<1>(variables[i]);
		if (board[row][col] == 0){
			std::vector<int> temp_i;
			if (LeastConstraintValue){
				temp_i = LCV(constraint, row, col, sF.getP(), sF.getQ(), sF.getN());
			}
			else{
				temp_i = constraint[variables[i]];
			}
			for (int pos = 0; pos < temp_i.size(); pos++){

				board[row][col] = constraint[variables[i]][pos];
				++countNode;
				sF.setBoard(board);
				makeConstraintMap(sF);

				if (forwardChecking(sF, timeout) == 1){
					++countNode;
					return 1;
				}
				else
				{
					auto currentTime = std::chrono::system_clock::now().time_since_epoch();
					if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= timeout)
					{
						++countNode;
						return 3;
					}

					else{
						board[row][col] = 0;
						sF.setBoard(board);
						makeConstraintMap(sF);
						if (pos == temp_i.size() - 1)
						{
							++backtrack;
							return 2;
						}
					}
				}
			}

			if (constraintBoard[variables[i]].empty())
			{
				++backtrack;
				return 2;
			}
			auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= timeout)
			{
				++countNode;
				return 3;
			}
		}
	}

	return 2;
}

std::vector<std::tuple<int, int>> SudokuBoardSolver::makeVariables(int N){
	std::vector<std::tuple<int, int>> result;
	for (int row = 0; row < N; row++){
		for (int col = 0; col < N; col++){
			result.push_back(std::make_tuple(row, col));
		}
	}
	return result;
}
void SudokuBoardSolver::makeConstraintMap(SudokuFile &sF)
{
	std::vector<std::vector<int>> board = sF.getBoard();
	std::map<std::tuple<int, int>, std::vector<int>> constraint;
	for (int row = 0; row < sF.getN(); row++)
	{
		for (int col = 0; col < sF.getN(); col++)
		{
			for (int k = 1; k < sF.getN() + 1; k++)
			{
				if (board[row][col] == 0 && checkConstraints(row, col, k, sF, board))
				{
					constraint[std::make_tuple(row, col)].push_back(k);
				}
			}
		}
	}
	constraintBoard = constraint;
}

void SudokuBoardSolver::removeValues(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N, int value){
	removeFromRow(constraint, row, N, value);
	//std::cout << std::endl;
	removeFromCol(constraint, col, N, value);
	//std::cout << std::endl;
	removeFromBox(constraint, row, col, p, q, N, value);
	//std::cout << std::endl;


}

void SudokuBoardSolver::removeFromRow(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int N, int value){
	for (int i = 0; i < N; i++){
		if (constraint.find(std::make_tuple(row, i)) != constraint.end()){
			std::vector<int> &val = constraint[std::make_tuple(row, i)];
			std::vector<int> temp;
			for (int pos = 0; pos < val.size(); pos++){
				if (val[pos] != value){
					temp.push_back(val[pos]);
				}
			}
			val = temp;
		}
	}
}

void SudokuBoardSolver::removeFromCol(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int col, int N, int value){
	for (int i = 0; i < N; i++){
		if (constraint.find(std::make_tuple(i, col)) != constraint.end()){
			std::vector<int> &val = constraint[std::make_tuple(i, col)];
			std::vector<int> temp;
			for (int pos = 0; pos < val.size(); pos++){
				if (val[pos] != value){
					temp.push_back(val[pos]);
				}
			}
			val = temp;
		}
	}
}

void SudokuBoardSolver::removeFromBox(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N, int value){
	int rDiv = row / p;
	int cDiv = col / q;
	for (int i = rDiv * p; i < (rDiv + 1) * p; ++i){
		for (int j = cDiv*q; j < (cDiv + 1)*q; ++j){
			if (constraint.find(std::make_tuple(i, j)) != constraint.end()){
				std::vector<int> &val = constraint[std::make_tuple(i, j)];
				std::vector<int> temp;
				for (int pos = 0; pos < val.size(); pos++){
					if (val[pos] != value){
						temp.push_back(val[pos]);
					}
				}
				val = temp;
			}
		}
	}
}
bool SudokuBoardSolver::isComplete(SudokuFile &sF)
{
	std::vector<std::vector<int>> board = sF.getBoard();

	for (int i = 0; i < sF.getN(); i++)
	{
		for (int j = 0; j < sF.getN(); j++)
		{
			if (board[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool SudokuBoardSolver::checkConstraints(int row, int col, int value, SudokuFile &sf, std::vector<std::vector<int>> &board){
	if (checkRow(row, value, sf.getN(), board)
		&& checkColumn(col, value, sf.getN(), board)
		&& checkBox(row, col, value, sf.getP(), sf.getQ(), board))
	{
		return true;
	}
	return false;
}

bool SudokuBoardSolver::checkRow(int row, int value, int N, std::vector<std::vector<int>> &board){

	for (int i = 0; i < N; ++i){
		if (board[row][i] == value){
			return false;
		}
	}

	return true;
}

bool SudokuBoardSolver::checkColumn(int column, int value, int N, std::vector<std::vector<int>> &board){
	for (int i = 0; i < N; ++i){
		if (board[i][column] == value){
			return false;
		}
	}
	return true;
}

bool SudokuBoardSolver::checkBox(int row, int column, int value, int p, int q, std::vector<std::vector<int>> &board){
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

std::vector<int> SudokuBoardSolver::LCV(std::map<std::tuple<int, int>, std::vector<int>> &constraint, int row, int col, int p, int q, int N){
	std::tuple<int, int> key = std::make_tuple(row, col);
	std::vector<int> result;
	std::vector<std::pair<int, int>> toBeSorted;

	for (int value : constraint[key]){
		int lcv = LCVcheckRow(row, col, value, N, constraint) + LCVcheckColumn(row, col, value, N, constraint) + LCVcheckBox(row, col, value, p, q, constraint);
		toBeSorted.push_back(std::make_pair(value, lcv));
	}
	std::sort(toBeSorted.begin(), toBeSorted.end(), [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs){
		return lhs.second > rhs.second; });

		for (std::pair<int, int> v : toBeSorted){
			result.push_back(v.first);

		}

		return result;
}

int SudokuBoardSolver::LCVcheckRow(int row, int col, int value, int N, std::map<std::tuple<int, int>, std::vector<int>> &constraint){
	int result = 0;
	for (int i = 0; i < N; ++i){
		std::tuple<int, int> key = std::make_tuple(row, i);
		if (i != col){

			if (std::find(constraint[key].begin(), constraint[key].end(), value) != constraint[key].end()){
				result += 1;
			}
		}
	}

	return result;
}

int SudokuBoardSolver::LCVcheckColumn(int row, int col, int value, int N, std::map<std::tuple<int, int>, std::vector<int>> &constraint){
	int result = 0;
	for (int i = 0; i < N; ++i){
		std::tuple<int, int> key = std::make_tuple(i, col);
		if (i != row){
			if (std::find(constraint[key].begin(), constraint[key].end(), value) != constraint[key].end()){
				result += 1;
			}
		}
	}
	return result;
}

int SudokuBoardSolver::LCVcheckBox(int row, int column, int value, int p, int q, std::map<std::tuple<int, int>, std::vector<int>> &constraint){
	int result = 0;
	int rDiv = row / p;
	int cDiv = column / q;
	for (int i = rDiv * p; i < (rDiv + 1) * p; ++i){
		for (int j = cDiv*q; j < (cDiv + 1)*q; ++j){
			if (i != row && j != column){
				try{
					std::tuple<int, int> key = std::make_tuple(i, j);
					if (std::find(constraint[key].begin(), constraint[key].end(), value) != constraint[key].end()){
						result += 1;
					}
				}
				catch (std::exception &e){
					std::cout << p << " " << q << std::endl;
				}
			}
		}
	}
	return result;
}

std::vector<std::tuple<int, int>> SudokuBoardSolver::MRV(std::map<std::tuple<int, int>, std::vector<int>> &constraint){
	std::vector < std::tuple<int, int, int> > toBeSorted;
	std::vector<std::tuple<int, int>> result;
	for (auto key : constraint){
		toBeSorted.push_back(std::make_tuple(std::get<0>(key.first), std::get<1>(key.first), key.second.size()));
	}
	std::sort(toBeSorted.begin(), toBeSorted.end(), [](const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs){
		return std::get<2>(lhs) < std::get<2>(rhs); });

		int lowest = std::get<2>(toBeSorted[0]);
		for (auto v : toBeSorted){

			if (std::get<2>(v) <= lowest){
				result.push_back(std::make_tuple(std::get<0>(v), std::get<1>(v)));
				lowest = std::get<2>(v);
			}
		}
		return result;
}

std::vector<std::tuple<int, int>> SudokuBoardSolver::findHighestDegree(SudokuFile &sF, std::vector<std::tuple<int, int>> var){
	std::vector<std::tuple<int, int>> result;
	std::vector<std::vector<int>> board = sF.getBoard();
	std::vector<std::tuple<int, int, int>> toBeSorted;

	int N = sF.getN();
	for (int pos = 0; pos < var.size(); pos++){
		int row = std::get<0>(var[pos]);
		int col = std::get<1>(var[pos]);
		int degree = checkRowForEmptyCells(row, col, N, board) + checkColumnForEmptyCells(row, col, N, board) + checkBoxForEmptyCells(row, col, sF.getP(), sF.getQ(), board);
		toBeSorted.push_back(std::make_tuple(row, col, degree));
	}
	std::sort(toBeSorted.begin(), toBeSorted.end(), [](const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs){
		return std::get<2>(lhs) > std::get<2>(rhs); });

		int highest = -1;
		for (auto v : toBeSorted){
			if (std::get<2>(v) >= highest){
				result.push_back(std::make_tuple(std::get<0>(v), std::get<1>(v)));
				highest = std::get<2>(v);
			}
		}
		return result;
}


std::vector<std::tuple<int, int>> SudokuBoardSolver::findHighestDegree(SudokuFile &sF)
{
	std::vector<std::tuple<int, int>> DH;
	std::vector<std::vector<int>> board = sF.getBoard();
	std::vector<std::tuple<int, int, int>> toBeSorted;

	for (int row = 0; row < sF.getN(); row++)
	{
		for (int col = 0; col < sF.getN(); col++)
		{
			if (board[row][col] == 0)
			{
				int degree = checkRowForEmptyCells(row, col, sF.getN(), board) + checkColumnForEmptyCells(row, col, sF.getN(), board) + checkBoxForEmptyCells(row, col, sF.getP(), sF.getQ(), board);
				toBeSorted.push_back(std::make_tuple(row, col, degree));
			}
		}
	}
	std::sort(toBeSorted.begin(), toBeSorted.end(), [](const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs){
		return std::get<2>(lhs) < std::get<2>(rhs); });

		int highest = -1;
		for (auto v : toBeSorted){
			if (std::get<2>(v) >= highest){
				DH.push_back(std::make_tuple(std::get<0>(v), std::get<1>(v)));
				highest = std::get<2>(v);
			}
		}

		return DH;
}


int SudokuBoardSolver::checkRowForEmptyCells(int row, int col, int N, std::vector<std::vector<int>> &board){
	int numberOfEmptyCellsInRow = 0;
	for (int i = 0; i < N; ++i)
	{
		if (i != col){
			if (board[row][i] == 0)
			{
				numberOfEmptyCellsInRow++;
			}
		}
	}
	return numberOfEmptyCellsInRow;
}

int SudokuBoardSolver::checkColumnForEmptyCells(int row, int col, int N, std::vector<std::vector<int>> &board){
	int numberOfEmptyCellsInColumn = 0;
	for (int i = 0; i < N; ++i)
	{
		if (i != row){
			if (board[i][col] == 0)
			{
				numberOfEmptyCellsInColumn++;
			}
		}
	}
	return numberOfEmptyCellsInColumn;
}

int SudokuBoardSolver::checkBoxForEmptyCells(int row, int column, int p, int q, std::vector<std::vector<int>> &board){
	int numberOfEmptyCellsInBox = 0;
	int rDiv = row / p;
	int cDiv = column / q;
	for (int i = rDiv * p; i < (rDiv + 1) * p; ++i){
		for (int j = cDiv * q; j < (cDiv + 1) * q; ++j){
			try{
				if (i != row && j != column && board[i][j] == 0){
					numberOfEmptyCellsInBox++;
				}
			}
			catch (std::exception &e){
				std::cout << p << " " << q << std::endl;
			}
		}
	}
	return numberOfEmptyCellsInBox;
}




std::string SudokuBoardSolver::toString(SudokuFile sf)
{
	o = Odometer();

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

std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> SudokuBoardSolver::getStartTime(){
	return startTime;
}

int SudokuBoardSolver::returnResultType(){
	return solutionType;
}

std::vector<std::vector<int>> SudokuBoardSolver::returnBoard(){
	return sf.getBoard();
}

int SudokuBoardSolver::getN(){
	return sf.getN();
}
int SudokuBoardSolver::getNumNode(){
	return countNode;
}
int SudokuBoardSolver::getBacktrack(){
	return backtrack;
}
int SudokuBoardSolver::returnNumberOfZeros(SudokuFile &sf)
{
	std::vector<std::vector<int>> board = sf.getBoard();

	int counter = 0;

	for (int i = 0; i < sf.getN(); i++)
	{
		for (int j = 0; j < sf.getN(); j++)
		{
			if (board[i][j] == 0)
			{
				counter++;
			}
		}
	}
	return counter;
}