#include <map>
#include <string>

#include "animation.h"
#include "gameObj.h"

// base objects to copy
class gameObj;

extern std::map<std::string, gameObj> baseBullets;
extern std::map<std::string, gameObj> baseEnemies;
