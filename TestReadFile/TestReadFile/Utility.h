#ifndef UTIL
#define UTIL

#include <algorithm>	//Find If
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time
#include "concol.h"		//Console Colors
#include <iostream>		//String

using namespace std;

namespace utility{
	bool is_number(const string& s);
	void printSeperator();
	double calculateElapsed(clock_t start, clock_t end);
}
#endif