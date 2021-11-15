#include <item.h>
#include <string>

int main()
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Fetch google html", 10, 10, 800, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	bool run = true;
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
			{
				run = false;
			}
		}

		SDL_Delay(80);
	}


	SDL_DestroyWindow(win);

	itemQuit();
}
