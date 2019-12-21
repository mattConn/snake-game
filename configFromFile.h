#include <string>
#include <map>
#include "global.h"
#include "gameObj.h"

void bulletsFromFile(std::string fileName, std::map<std::string, gameObj> &objMap);

void enemiesFromFile(std::string fileName, std::map<std::string, gameObj> &objMap);

void wavesFromFile(std::string fileName, std::vector<std::vector<gameObj>> &objMap);
