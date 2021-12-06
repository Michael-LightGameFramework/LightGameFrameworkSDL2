#include "item.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char ** arg)
{
	srand(time(NULL));
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 600, 500, SDL_WINDOW_SHOWN);
	SDL_Surface * screen = SDL_GetWindowSurface(win);

	item bob;
	bob.loadImage("../resources/grass.png");

	bool run = true;
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					run = false;
					break;

			}
		}

		SDL_FillRect(screen, NULL, 0x00aaff);
		bob.draw(screen);
		SDL_UpdateWindowSurface(win);
				
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
