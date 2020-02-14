#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>

namespace useful {

	// function prototypes
	// ===================

	// return current datestring
	std::string getDateString();

	// generate random int over inclusive range
	int randomInt(const int &max, const int &min = 0);

} // end namespace
