#include "item.h"

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 600, 500, SDL_WINDOW_SHOWN);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 60;
	int desiredDelta = 1000 / fps;
	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					run = false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							break;
						case SDLK_DOWN:
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							break;
						case SDLK_DOWN:
							break;
					}

					break;

			}
		}

		SDL_RenderClear(screen);


		SDL_RenderPresent(screen);
		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
