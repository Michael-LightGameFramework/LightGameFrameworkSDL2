#include "item.h"
#include <ctime>
#include <cstdlib>
#include <vector>

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
	bob.setCenter(43, 68, 27);

	std::vector <item *> gems;
	int maxGems = 200;
	for(int i = 0; i < maxGems; i ++)
	{
		gems.push_back(new item());
		gems[i]->setRenderer(screen);
		gems[i]->loadImage("../resources/crystal.png");
		gems[i]->setPos(rand() % 550, rand() % 450);
		gems[i]->setSize(50, 50);
		gems[i]->setCenter(24, 24, 8);
	}

	bool run = true;
	int speedx = 0;
	int speedy = 0;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	double ang = 0;
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

		SDL_RenderClear(screen);
		bob.draw();
		for(int i = 0; i < maxGems; i ++)
		{
			if(bob.getCollision(gems[i]))
			{
				gems.erase(gems.begin() + i);
				maxGems --;
			}
			else
			{
				gems[i]->draw(ang);
			}
		}
		SDL_RenderPresent(screen);
		ang ++;

		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
