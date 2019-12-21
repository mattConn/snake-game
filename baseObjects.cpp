#include <map>
#include <string>

#include "baseObjects.h"
#include "animation.h"
#include "gameObj.h"

// base objects to copy

std::map<std::string, gameObj> baseBullets;

std::map<std::string, gameObj> baseEnemies;
/*
std::map<std::string, gameObj> baseEnemies = {
	{
		"bat",
		gameObj("enemy-bat", 6, 50, 46, 0, 0, "orange", 200)
	}
};
*/
