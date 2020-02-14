#include "SDLw.h"
#include "game.h"
#include "useful.h"

#include "debug.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace useful {

// functions
// =========

// write data to file
template <class T>
bool writeToFile(const std::string &fileName, const T &data)
{
	std::ofstream file;
	file.open(fileName);

	if(!file) return false;

	file << data;

	return true;
}

// return current datestring
std::string getDateString()
{
	time_t t = std::time(NULL);
	std::string dateString = std::string(std::ctime(&t));
	dateString[dateString.size()-1] = '\0';
	return dateString;
}

// generate random int over inclusive range
int randomInt(const int &max, const int &min)
{
	std::srand(SDL_GetTicks()); 
	return std::rand() % max + min;
}


} // end namespace
