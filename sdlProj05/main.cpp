#include "item.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char ** arg)
{
	srand(time(NULL));
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 600, 500, SDL_WINDOW_SHOWN);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	item bob;
	bob.setRenderer(screen);
	bob.loadImage("../resources/Hero-Guy-PNG/_Mode-Sword/02-Run/JK_P_Sword__Run_004.png");
	bob.setPos(30, 30);
	bob.setSize(100, 100);

	bool run = true;
	int speedx = 0;
	int speedy = 0;
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
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							speedy = -1;
							break;
						case SDLK_DOWN:
							speedy = 1;
							break;
						case SDLK_RIGHT:
							speedx = 1;
							break;
						case SDLK_LEFT:
							speedx = -1;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							speedy = 0;
							break;
						case SDLK_DOWN:
							speedy = 0;
							break;
						case SDLK_RIGHT:
							speedx = 0;
							break;
						case SDLK_LEFT:
							speedx = 0;
							break;
					}


					break;

			}
		}
		bob.move(speedx, speedy);

		bob.draw();
		SDL_RenderPresent(screen);
		SDL_Delay(20);
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
