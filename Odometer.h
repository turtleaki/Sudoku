#pragma once
#include <iostream>

class Odometer{
public:
	Odometer();
	~Odometer(void);
	int odometerToInt(std::string value);
	std::string intToOdometer(int num);

private:
	std::string ODOMETER;
};