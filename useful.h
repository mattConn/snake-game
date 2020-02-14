#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

namespace useful {

	// function prototypes
	// ===================

	// write data to file
	template <class T>
	bool writeToFile(const std::string &fileName, const T &data);

	// return current datestring
	std::string getDateString();

	// generate random int over inclusive range
	int randomInt(const int &max, const int &min = 0);

} // end namespace
