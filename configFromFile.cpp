#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "global.h"
#include "gameObj.h"
#include "animation.h"
#include "configFromFile.h"

void bulletsFromFile(std::string fileName, std::map<std::string, gameObj> &objMap)
{
	std::ifstream infile(fileName);

	std::string line;
	std::string tok;

	std::vector<std::string> args;

	while(std::getline(infile, line)) // read file lines
	{
		// handle line
		if(line[0] != '#') // check for line comment
		{
			std::stringstream ss(line); // stringstream from line

			// tokenize lines
			while(std::getline(ss, tok, ' ')) // space delimiter
				args.push_back(tok);

			global::allTextures[args[1]] = global::loadTexture(args[1].c_str());
			objMap[args[0]] = gameObj(args[1], std::stoi(args[2]), std::stoi(args[3]), std::stoi(args[4]));
		}

		line = "";
		tok = "";
		args.clear();
	}

	infile.close();
}

void enemiesFromFile(std::string fileName, std::map<std::string, gameObj> &objMap)
{
	std::ifstream infile(fileName);

	std::string line;
	std::string tok;

	std::vector<std::string> args;

	while(std::getline(infile, line))
	{
		if(line[0] != '#') // check for line comment
		{
			std::stringstream ss(line); // stringstream from line

			while(std::getline(ss, tok, ' ')) // space delimiter
				args.push_back(tok);

			global::allTextures[args[1]] = global::loadTexture(args[1].c_str());

			objMap[args[0]] = gameObj(args[1], std::stoi(args[2]), std::stoi(args[3]), std::stoi(args[4]), 0, 0, args[5], std::stoi(args[6]));


		}

		line = "";
		tok = "";
		args.clear();
	}

	infile.close();

}

void wavesFromFile(std::string fileName, std::vector<std::vector<gameObj>> &objVec)
{
	std::ifstream infile(fileName);

	std::string line;
	std::string tok;

	std::vector<std::string> args;

	gameObj enemy;
	std::vector<bool (*)(gameObj*)> animSet;
	std::vector<gameObj> wave;

	bool onEnemy = true;

	// read file lines
	while(std::getline(infile, line))
	{
		// handle lines
		if(line.length() > 0 && line[0] != '#') // check for line comment
		{

			// blank line: end adding anims to enemy, store enemy in wave
			if(line == "ENDE")
			{
				wave.push_back(enemy);
				onEnemy = true;
			}
			else if(line == "ENDW") // end wave, store wave
			{
				objVec.push_back(wave);
				wave.clear();
			}
			else
			{
				std::stringstream ss(line); // stringstream from line

				// tokenize lines
				while(std::getline(ss, tok, ' ')) // space delimiter
					args.push_back(tok);

				// enemy data line
				if(onEnemy)
				{
					enemy = gameObj(baseEnemies[args[0]], std::stoi(args[1]), std::stoi(args[2]));
					onEnemy = false;
				}
				else // animation data line
				{
					for(auto token : args)
					{
						if(token == "up")
							animSet.push_back(animation::up);
						else if(token == "down")
							animSet.push_back(animation::down);
						else if(token == "left")
							animSet.push_back(animation::left);
						else if(token == "right")
							animSet.push_back(animation::right);
						else if(token == "fire")
							animSet.push_back(animation::fire);
					}

					enemy.addAnimationSet(animSet, std::stoi(args[0]));
					animSet.clear();
				}
				

				tok = "";
				args.clear();
			}
		} // end if line commented check
			
	}

	infile.close();

}
