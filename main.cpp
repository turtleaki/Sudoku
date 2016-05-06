#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include "SudokuFile.h"
#include "SudokuBoardReader.h"
#include "SudokuBoardGenerator.h"
#include "SudokuBoardSolver.h"
#include "Odometer.h"


int main(int argc, const char* argv[])
{
	auto time_start = std::chrono::system_clock::now().time_since_epoch();

	if (argc < 4){
		std::cout << "error" << std::endl;
		return 1;  //Should throw an exception here (?)

	}
	std::string token = "";
	for (int j = 2; j < argc; j++){
		token += argv[j];
	}
	bool FC = false;
	bool MRV = false;
	bool LCV = false;
	bool DH = false;

	if (token.find("GEN") != std::string::npos){
		std::string output_name = argv[1];
		int N = atoi(argv[3]);
		int p = atoi(argv[4]);
		int q = atoi(argv[5]);
		int numAssignments = atoi(argv[6]);

		SudokuBoardGenerator sudokuGenerator(N, p, q, numAssignments);
		SudokuBoardReader sudokuReader = SudokuBoardReader();
		std::vector<std::vector<int>> board = sudokuGenerator.returnBoard();
		Odometer o;
		int end = N - 1;
		std::ofstream output_file(output_name);
		if (output_file.is_open()){
			output_file << N << " " << p << " " << q << " " << numAssignments << "\r\n";
			for (int row = 0; row < N; ++row){
				for (int col = 0; col < N; ++col){
					output_file << o.intToOdometer(board[row][col]) << " ";
				}
				output_file << "\r\n";
			}
		}

	}
	else{

		SudokuBoardReader sudokuReader = SudokuBoardReader();
		SudokuFile input_sf;
		
		std::string input_file = argv[1];
		std::string output_name = argv[2];
		long timeout = atoi(argv[3]);
		std::cout << token << "; " << input_file <<  ", " << output_name << ", " << timeout << std::endl; 
		if (token.find("FC") != std::string::npos){
			FC = true;
		}
		if (token.find("MRV") != std::string::npos){
			MRV = true;
		}
		if (token.find("LCV") != std::string::npos){
			LCV = true;
		}
		if (token.find("DH") != std::string::npos){
			DH = true;
		}

		input_sf = sudokuReader.readFile(input_file);

		std::ofstream output_file(output_name);

		if (output_file.is_open()){
			output_file << "TOTAL_START=";
			//output_file << total_start time here
			output_file << std::chrono::duration_cast<std::chrono::seconds>(time_start).count();
			output_file << std::endl;

			output_file << "\r\nPREPROCESSING_START=";
			output_file << " 4"; //skip for now

			output_file << "\r\nPREPROCESSING_DONE=";
			output_file << "4"; //skip for now

			output_file << "\r\nSEARCH_START=";
			//start timer here
			std::cout << timeout << FC << DH << LCV << MRV << std::endl;
			SudokuBoardSolver SBS(input_sf, timeout, FC, DH, LCV, MRV);
			auto search_start = SBS.getStartTime();
			output_file << std::chrono::duration_cast<std::chrono::seconds>(search_start).count();
			//std::cout << input_sf.toString() << std::endl;

			output_file << "\r\nSEARCH_DONE=";
			//get timer here math
			auto search_done = std::chrono::system_clock::now().time_since_epoch();
			output_file << std::chrono::duration_cast<std::chrono::seconds>(search_done).count() << "\n";

			output_file << "\r\nSOLUTION_TIME=";
			//do search_done - search start for now
			output_file << std::chrono::duration_cast<std::chrono::seconds>(search_done - search_start).count() << "\n";

			output_file << "\r\nSTATUS=";
			//if success|timeout|error
			if (SBS.returnResultType() == 1){
				output_file << "success";
			} 
			else if (SBS.returnResultType() == 3){
				output_file << "timeout";
			}
			else if (SBS.returnResultType() == 4){
				output_file << "error";
			}


			output_file << "\r\nSOLUTION=(";
			//print solution
			std::vector<std::vector<int>> board = SBS.returnBoard();
			Odometer o;
			int N = SBS.getN();
			int end = N-1;

			for (int row = 0; row < N; ++row){
				for (int col = 0; col < N; ++col){
					output_file << o.intToOdometer(board[row][col]);
					if ((row == end) && (col == end)){
						output_file << ")";
					}
					else{
						output_file << ",";
					}
				}

			}

			output_file << "\r\nCOUNT_NODES=";
			//get node
			output_file << SBS.getNumNode();

			output_file << "\r\nCOUNT_DEADENDS=";
			//get num of backtracks
			output_file << SBS.getBacktrack();


		}

	}

	return 0;
}