#include "Odometer.h"
#include <iostream>
#include <algorithm>
#include <exception>

Odometer::Odometer(){
	ODOMETER = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

Odometer::~Odometer(){

}

int Odometer::odometerToInt(std::string value){
	std::transform(value.begin(), value.end(), value.begin(), ::toupper);
	int retVal = ODOMETER.find(value);
	if (retVal < 0){
		std::cout << "odometerToInt failed. Invalid value " << value.c_str() << " Replaced with 0" << std::endl;
		retVal = 0;
	}
	return retVal;
}

std::string Odometer::intToOdometer(int num){
	std::string retVal;
	try{
		retVal = ODOMETER[num];
	}
	catch (std::out_of_range& e){
		std::cout << "intToOdometer failed. Invalid number " << num << "Replaced with 0" << std::endl;
		retVal = "0";
	}
	return retVal;
}