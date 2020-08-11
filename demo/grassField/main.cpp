#include "item.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

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
	int W = 1900, H = 1500;
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, W, H, SDL_WINDOW_SHOWN);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	animation grass1;
	grass1.setRenderer(screen);
	grass1.loadAnimation("grass", "0", ".png");

	std::vector <grass *> field;
	for(int i = 0; i < 1000000; i ++)
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
		for(int i = 0; i < 50000; i ++)
		{
			field[i]->draw();
		}


		SDL_RenderPresent(screen);
		grass1.next();
		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
