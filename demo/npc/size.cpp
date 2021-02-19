#include "item.h"

#include <iostream>

using namespace std;

int main()
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 300, 300, SDL_WINDOW_SHOWN);
	int count = 0;
	while(SDL_GetTicks() < 10000)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_MOUSEMOTION)
			{
				count ++;
			}

		}
		SDL_UpdateWindowSurface(win);
	}
	cout << count / 10;

	itemQuit();

}
