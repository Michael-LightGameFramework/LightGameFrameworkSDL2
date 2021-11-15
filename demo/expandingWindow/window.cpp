#include <iostream>
#include <string>
#include "item.h"

using namespace std;

int main()
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Stretch Image", 10, 10, 300, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	item bkg;
	bkg.setRenderer(screen);
	bkg.loadImage("zoom.png");
	bkg.setSize(300, 300);
	bkg.setPos(0,0);

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
				case SDL_WINDOWEVENT:
					
					if(ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						// The new size is stored in ev.window, you could also use
						// SDL_GetWindowSize(&x, &y); to fetch the variables
						bkg.setSize(ev.window.data1, ev.window.data2);
					}
					break;
			}
		}

		SDL_RenderClear(screen);
		bkg.draw();
		SDL_RenderPresent(screen);
		SDL_Delay(30);
	}

	itemQuit();
	SDL_DestroyWindow(win);
}
