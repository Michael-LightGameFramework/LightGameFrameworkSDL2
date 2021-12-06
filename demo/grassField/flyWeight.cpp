#include "item.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

class grass : public item
{
	public:
	void draw();
	void setBlade(item * gb);
	private:
	item * blade;
};
void grass::setBlade(item * gb)
{
	blade = gb;
}

void grass::draw()
{
	SDL_RenderCopy(ren, blade->getImage(), NULL, &pos);	
}



int main(int argc, char ** arg)
{
	itemInit();
	int W = 1900, H = 1200;
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, W, H, SDL_WINDOW_SHOWN);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	animation grass1;
	grass1.setRenderer(screen);
	grass1.loadAnimation("../../resources/grass", "0", ".png");

	animation bob;
	bob.setRenderer(screen);
	bob.setSize(100, 150);
	bob.loadAnimation("run/JK_P_Sword__Run_", "000", ".png");

	std::vector <grass *> field;
	int bladeCount = 1000000;

	for(int i = 0; i < bladeCount; i ++)
	{
		field.push_back(new grass);
		field[i]->setRenderer(screen);
		field[i]->setBlade(&grass1);
		field[i]->setSize(25, 50);
		field[i]->setPos(rand() % W, rand() % H);

	}

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 30;
	int desiredDelta = 1000 / fps;
	int shown = 100;
	int bspeedy = 0;
	int maxItemsForFrameRate = shown;
	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					std::cout << "On hitting exit you had " << shown << " blades of grass\n";
					run = false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							bspeedy = -1;
							break;
						case SDLK_DOWN:
							bspeedy = 1;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							bspeedy = 0;
							break;
						case SDLK_DOWN:
							bspeedy = 0;
							break;
					}

					break;

			}
		}

		SDL_RenderClear(screen);
		for(int i = 0; i < shown; i ++)
		{
			field[i]->draw();
		}
		bob.draw();
		int add = shown / 100;
		if(shown < bladeCount - add)
		{
			shown += add;
		}
		else
		{
			shown = bladeCount;
		}


		bob.next();
		bob.move(0, bspeedy);
		SDL_RenderPresent(screen);
		grass1.next();
		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			maxItemsForFrameRate = shown;
		}
	}

	std::cout << "For your desired frame rate of " << fps << " your max items is about " << maxItemsForFrameRate << "\n";

	SDL_DestroyWindow(win);
	itemQuit();

}
