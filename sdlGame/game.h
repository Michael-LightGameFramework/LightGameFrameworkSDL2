// treat every level as a plugin.


#ifndef THEMAINGAMEWRAPPER
#define THEMAINGAMEWRAPPER

#include <item.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

typedef board * (*spawnType)(SDL_Renderer * screen);
typedef void (*destroyType)(board *);

class game : public item
{
	public:
	game(SDL_Renderer * screen);
	~game();
	void addLevel(std::string filepath);
	void init();
	void gameLoop();
	void clear();

	
	public:
	board * currentLvl;
	std::vector <std::string> pluginPaths;
	std::vector <void *> handle;
	std::vector <spawnType> spawner;
	std::vector <destroyType> destroyer;
	std::vector <board *> vlevels;
	
	// <name, level>
	std::unordered_map<std::string, board*> levels;
	// these aren't drawn, but their imges are referenced
	// by other items for resource sharing. 
	group assets;

};



#endif
