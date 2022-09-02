#include <item.h>
#include <level1_arrival.h>


int main()
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Where are We", 0,0,1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

	level1 start(screen);

	board * current = &start;
	// init board to unplayed state
	current->init();

	bool run = true;
	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
			{
				run = false;
			}
			if(ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.sym == SDLK_ESCAPE)
				{
					run = false;
				}
			}
			current->handleEvent(&ev);
		}
		current->update(startLoop);
		SDL_RenderClear(screen);
		current->draw();
		SDL_RenderPresent(screen);
	}

	itemQuit();
	SDL_DestroyWindow(win);
}
