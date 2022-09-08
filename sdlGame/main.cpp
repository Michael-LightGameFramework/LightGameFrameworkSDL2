#include <item.h>
#include <game.h>

#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char ** argv)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Where are We", 0,0,1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

	game gm(screen);
	std::ifstream ifile("packages");
	std::string temp;
	while(std::getline(ifile, temp))
	{
		gm.addLevel(temp);
	}

	gm.init();
	gm.gameLoop();
	gm.clear();
	SDL_DestroyWindow(win);
	itemQuit();
}
